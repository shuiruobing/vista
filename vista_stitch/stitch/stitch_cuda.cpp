#include "stitch_cuda.h"
#include "merge_dll/MergeImg.h"
#include <algorithm>
#include <QDebug>

#define MAX_INPUT 16
#define CUDA_ALIGN 16

StitchCuda::StitchCuda(const char *stitchRoot)
    : root_(stitchRoot)
    , nv12s_(new CUdeviceptr[MAX_INPUT])
{
    memset(nv12s_, 0, MAX_INPUT);
}

StitchCuda::~StitchCuda()
{
    delete[] nv12s_;
}

bool StitchCuda::setHardware(const std::string &device)
{
    if(hasHW())
        return true;

    auto ret = cuDeviceGet(&device_, std::stoi(device));
    if(ret != CUDA_SUCCESS)
    {
        return false;
    }
    ret = cuCtxCreate(&pCtx_, CU_CTX_SCHED_AUTO, device_);
    if(ret != CUDA_SUCCESS)
    {
        return false;
    }
    return true;
}

bool StitchCuda::setInput(int count, int width, int height)
{
    while(!inputs_.empty())
    {
        if(!freeMem(inputs_.back()))
            return false;
        inputs_.pop_back();
    }

    inputs_.insert(std::end(inputs_), count, CudaFrame(width, height));
    for(CudaFrame& cf : inputs_)
    {
        if(!allocMem(cf))
            return false;
    }
    if(count > MAX_INPUT)
    {
        delete[] nv12s_;
        nv12s_ = new CUdeviceptr[count];
        memset(nv12s_, 0 , count);
    }

    return true;
}

bool StitchCuda::setOutput(int width, int height)
{
    if(pOutput_)
    {
        if(!freeMem(*pOutput_))
            return false;
        pOutput_->width = width;
        pOutput_->height = height;
    }
    else
        pOutput_ = new CudaFrame(width, height);
    return allocMem(*pOutput_);
}

bool StitchCuda::open()
{
    return pOutput_ && !inputs_.empty() && hasHW();
}

bool StitchCuda::stitch(const std::vector<FramePtr> &ins, FramePtr out)
{
    if(ins.size() != inputs_.size())
        return false;
    for(int i = 0; i < ins.size(); i++)
    {
        if((ins[i]!= nullptr) && !fromAVframeNv12(ins[i],inputs_[i]))
            return false;
        nv12s_[i] = inputs_[i].data;
    }

    if(!stitchImpl(nv12s_, pOutput_->data))
        return false;

    if(!toAVFrameNV12(*pOutput_,out))
        return false;

    return true;
}

bool StitchCuda::resize(const FramePtr &src, FramePtr dst)
{
    Q_UNUSED(src);
    Q_UNUSED(dst);
    return false;
}

bool StitchCuda::hasHW()
{
    return (pCtx_ != nullptr);
}

bool StitchCuda::allocMem(CudaFrame &cf)
{
    CUcontext cur = nullptr;
    auto ret = cuCtxPushCurrent(pCtx_);
    if(ret != CUDA_SUCCESS)
        return false;
    ret = cuMemAllocPitch(&cf.data, &cf.pitch, cf.width, cf.height*3/2, CUDA_ALIGN);
    if(ret != CUDA_SUCCESS)
        return false;
    ret = cuCtxPopCurrent(&cur);
    if(ret != CUDA_SUCCESS)
        return false;
    return true;
}

bool StitchCuda::freeMem(CudaFrame& cf)
{
    CUcontext cur = nullptr;
    auto ret = cuCtxPushCurrent(pCtx_);
    if(ret != CUDA_SUCCESS)
        return false;
    if(cf.data)
        cuMemFree(cf.data);
    ret = cuCtxPopCurrent(&cur);
    if(ret != CUDA_SUCCESS)
        return false;
    return true;
}

bool StitchCuda::fromAVframeNv12(const FramePtr &fp, CudaFrame &cf)
{
    CUcontext cur = nullptr;
    auto ret = cuCtxPushCurrent(pCtx_);
    if(ret != CUDA_SUCCESS)
        return false;

    size_t offset = 0;
    for(int i = 0; i < 2; i++)
    {
        CUDA_MEMCPY2D cpy;
        memset(&cpy,0,sizeof(cpy));
        cpy.srcMemoryType = fp->hw_frames_ctx ? CU_MEMORYTYPE_DEVICE : CU_MEMORYTYPE_HOST;
        cpy.dstMemoryType = CU_MEMORYTYPE_DEVICE;
        if(fp->hw_frames_ctx)
            cpy.srcDevice = (CUdeviceptr)fp->data[i];
        else
            cpy.srcHost = (const void*)fp->data[i];
        cpy.dstDevice = cf.data;
        cpy.srcPitch = fp->linesize[i];
        cpy.dstPitch = cf.pitch;
        cpy.dstY = offset;
        cpy.WidthInBytes = std::min(cf.pitch, (size_t)fp->linesize[i]);
        cpy.Height = fp->height>>(i?1:0);
        auto ret = cuMemcpy2D(&cpy);
        if(ret != CUDA_SUCCESS)
        {
            cuCtxPopCurrent(&cur);
            return false;
        }
        offset += fp->height;
    }

    ret = cuCtxPopCurrent(&cur);
    if(ret != CUDA_SUCCESS)
        return false;
    return true;
}

bool StitchCuda::toAVFrameNV12(const CudaFrame &cf, FramePtr &fp)
{
    CUcontext cur = nullptr;
    auto ret = cuCtxPushCurrent(pCtx_);
    if(ret != CUDA_SUCCESS)
        return false;

    size_t offset = 0;
    for(int i = 0; i < 2; i++)
    {
        CUDA_MEMCPY2D cpy;
        memset(&cpy,0,sizeof(cpy));
        cpy.srcMemoryType = CU_MEMORYTYPE_DEVICE;
        cpy.dstMemoryType = fp->hw_frames_ctx ? CU_MEMORYTYPE_DEVICE : CU_MEMORYTYPE_HOST;
        cpy.srcDevice = cf.data;
        if(fp->hw_frames_ctx)
            cpy.dstDevice = (CUdeviceptr)fp->data[i];
        else
            cpy.dstHost = (void*)fp->data[i];
        cpy.srcPitch = cf.pitch;
        cpy.dstPitch = fp->linesize[i];
        cpy.srcY = offset;
        cpy.WidthInBytes = std::min(cf.pitch, (size_t)fp->linesize[i]);
        cpy.Height = cf.height>>(i?1:0);
        auto ret = cuMemcpy2D(&cpy);
        if(ret != CUDA_SUCCESS)
        {
            cuCtxPopCurrent(&cur);
            return false;
        }
        offset += cf.height;
    }

    ret = cuCtxPopCurrent(&cur);
    if(ret != CUDA_SUCCESS)
        return false;
    return true;
}

bool StitchCuda::stitchImpl(CUdeviceptr *in, CUdeviceptr &out)
{
    CUcontext cur = nullptr;
    CudaFrame& icf = inputs_.front();
    CudaFrame& ocf = *pOutput_;

    auto ret = cuCtxPushCurrent(pCtx_);
    if(ret != CUDA_SUCCESS)
        return false;

    auto ret1 = merge_nv12((unsigned char**)in,
                           (int)icf.width,
                           (int)icf.pitch,
                           (int)icf.height,
                           (int)inputs_.size(),
                           (unsigned char*)out,
                           (int)ocf.width,
                           (int)ocf.pitch,
                           (int)ocf.height,
                           device_);
    cuCtxPopCurrent(&cur);
    return (ret1 == 0);
}
