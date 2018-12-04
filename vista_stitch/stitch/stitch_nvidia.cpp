//#include "stitch_nvidia.h"
//#include "TrstPano/TrstPano.h"
//#include "nvida/include/cuda.h"
//#include <algorithm>

//#define CUDA_ALIGN 16

//StitchNvida::StitchNvida(const std::string& device
//                         , const char *stitchRoot)
//    : no_(std::stoi(device))
//    , root_(stitchRoot)
//    , pPano_(new TrstPano)
//{
//}

//StitchNvida::~StitchNvida()
//{
//    if(pPano_)
//        delete pPano_;
//}

//bool StitchNvida::initHardware()
//{
//    if(hasHWInited())
//        return true;

//    auto ret = cuDeviceGet(&pDevice_, 0);
//    if(ret != CUDA_SUCCESS)
//    {
//        return false;
//    }
//    ret = cuCtxCreate(&pCtx_, CU_CTX_SCHED_AUTO, pDevice_);
//    if(ret != CUDA_SUCCESS)
//    {
//        return false;
//    }

//    return true;
//}

//bool StitchNvida::setInput(int count, int width, int height)
//{
//    while(!inputs_.empty())
//    {
//        if(!freeHWMem(inputs_.back()))
//            return false;
//        inputs_.pop_back();
//    }

//    for(int i = 0; i < count; i++)
//    {
//        CudaFrame cf(width, height);
//        if(!allocHWMem(cf))
//            return false;
//        inputs_.push_back(cf);
//    }

//    inputNv12s_ = new unsigned char*[count];
//    for(int i = 0; i < count; i++)
//        inputNv12s_[i] = reinterpret_cast<unsigned char*>(inputs_.at(i).data);
//    return true;
//}

//bool StitchNvida::setOutput(int width, int height)
//{
//    if(!outputs_.empty())
//    {
//        if(!freeHWMem(outputs_.back()))
//            return false;
//        outputs_.pop_back();
//    }

//    CudaFrame cf(width, height);
//    if(!allocHWMem(cf))
//        return false;
//    outputs_.push_back(cf);
//    return true;
//}

//bool StitchNvida::addChildOuput(int width, int height)
//{
//    if(outputs_.empty())
//        return false;

//    CudaFrame cf(width, height);
//    if(!allocHWMem(cf))
//        return false;
//    outputs_.push_back(cf);
//    return true;
//}

//bool StitchNvida::start()
//{
//    if(isInited())
//        return true;

//    if(!hasHWInited())
//        return false;

//    if(inputs_.empty() || outputs_.empty())
//        return false;

//    return dllInit();
//}

//void StitchNvida::stop()
//{
//    if(isInited())
//        this->dllUninit();
//}

//bool StitchNvida::isStarted()
//{
//    return isInited();
//}

//bool StitchNvida::stitch(const std::vector<FramePtr> &inVec, FramePtr out)
//{
//    if(!isInited())
//        return false;

//    if(inVec.size() != inputs_.size())
//        return false;

//    for(int i = 0 ; i < inVec.size(); i++)
//    {
//        if(!copyFromFFmpeg_NV12(inVec.at(i), inputs_.at(i)))
//            return false;
//        inputNv12s_[i] = (unsigned char*)inputs_.at(i).data;
//    }

//    unsigned char* outData = (unsigned char*)outputs_.front().data;
//    if(!dllStitch(inputNv12s_,outData))
//        return false;

//    if(!copyToFFmpeg_NV12(outputs_.front(),out))
//        return false;

//    return true;
//}

//void StitchNvida::correctChroma(unsigned char *src[])
//{

//}

//bool StitchNvida::hasHWInited()
//{
//    return (pCtx_ != nullptr);
//}

//bool StitchNvida::allocHWMem(CudaFrame &cf)
//{
//    if(!hasHWInited())
//        return false;

//    CUcontext cur = nullptr;
//    auto ret = cuCtxPushCurrent(pCtx_);
//    if(ret != CUDA_SUCCESS)
//        return false;
//    ret = cuMemAllocPitch(&cf.data, &cf.pitch, cf.width, cf.height*3/2, CUDA_ALIGN);
//    if(ret != CUDA_SUCCESS)
//        return false;
//    ret = cuCtxPopCurrent(&cur);
//    if(ret != CUDA_SUCCESS)
//        return false;
//    return true;
//}

//bool StitchNvida::freeHWMem(CudaFrame &cf)
//{
//    if(!hasHWInited())
//        return false;
//    CUcontext cur = nullptr;
//    auto ret = cuCtxPushCurrent(pCtx_);
//    if(ret != CUDA_SUCCESS)
//        return false;
//    if(cf.data)
//        cuMemFree(cf.data);
//    ret = cuCtxPopCurrent(&cur);
//    if(ret != CUDA_SUCCESS)
//        return false;
//    return true;
//}

//bool StitchNvida::dllInit()
//{
//    if(inputs_.empty() || outputs_.empty())
//        return false;

//    CudaFrame& cfi = inputs_.front();
//    CudaFrame& cfo = outputs_.front();

//    int ret = pPano_->InitPanoSDK_Th4K(const_cast<char*>(root_.c_str())
//                               , static_cast<int>(inputs_.size())
//                               , static_cast<int>(cfi.width)
//                               , static_cast<int>(cfi.pitch)
//                               , static_cast<int>(cfi.height)
//                               , static_cast<int>(cfo.width)
//                               , static_cast<int>(cfo.pitch)
//                               , static_cast<int>(cfo.height));

//    if(ret <= 0 || ret >= 100)
//    {
//        return false;
//    }

//    dllHandle_ = ret;
//    return true;
//}

//void StitchNvida::dllUninit()
//{
//    if(isInited())
//    {
//        pPano_->UnInitPanoSDK(dllHandle_);
//        dllHandle_ = -1;
//    }
//}

//bool StitchNvida::isInited()
//{
//    return dllHandle_ > 0 && dllHandle_ < 100;
//}

//bool StitchNvida::dllStitch(unsigned char** ins, unsigned char* out)
//{
//    if(!isInited())
//        return false;

//    int ret = pPano_->UpdateData_4K(dllHandle_, ins, nullptr, out);
//    if(ret != TR_OK)
//    {
//        return false;
//    }
//    return true;
//}

//bool StitchNvida::copyFromFFmpeg_NV12(const FramePtr &fp, CudaFrame &cf)
//{
//    CUcontext cur = nullptr;
//    auto ret = cuCtxPushCurrent(pCtx_);
//    if(ret != CUDA_SUCCESS)
//        return false;

//    int offset = 0;
//    for(int j = 0; j < 2; j++)
//    {
//        CUDA_MEMCPY2D cpy;
//        memset(&cpy,0,sizeof(cpy));
//        cpy.srcMemoryType = CU_MEMORYTYPE_DEVICE;
//        cpy.dstMemoryType = CU_MEMORYTYPE_DEVICE;
//        cpy.srcDevice = (CUdeviceptr)fp->data[j];
//        cpy.dstDevice = cf.data;
//        cpy.srcPitch = fp->linesize[j];
//        cpy.dstPitch = cf.pitch;
//        cpy.dstY = offset;
//        cpy.WidthInBytes = std::min(cf.pitch, (size_t)fp->linesize[j]);
//        cpy.Height = fp->height>>(j?1:0);
//        auto ret = cuMemcpy2D(&cpy);
//        if(ret != CUDA_SUCCESS)
//        {
//            cuCtxPopCurrent(&cur);
//            return false;
//        }
//        offset += fp->height;
//    }

//    ret = cuCtxPopCurrent(&cur);
//    if(ret != CUDA_SUCCESS)
//        return false;
//    return true;
//}

//bool StitchNvida::copyToFFmpeg_NV12(const CudaFrame &cf, FramePtr &fp)
//{
//    CUcontext cur = nullptr;
//    auto ret = cuCtxPushCurrent(pCtx_);
//    if(ret != CUDA_SUCCESS)
//        return false;

//    int offset = 0;
//    for(int j = 0; j < 2; j++)
//    {
//        CUDA_MEMCPY2D cpy;
//        memset(&cpy,0,sizeof(cpy));
//        cpy.srcMemoryType = CU_MEMORYTYPE_DEVICE;
//        cpy.dstMemoryType = CU_MEMORYTYPE_DEVICE;
//        cpy.srcDevice = cf.data;
//        cpy.dstDevice = (CUdeviceptr)fp->data[j];
//        cpy.srcPitch = cf.pitch;
//        cpy.dstPitch = fp->linesize[j];
//        cpy.srcY = offset;
//        cpy.WidthInBytes = std::min(cf.pitch, (size_t)fp->linesize[j]);
//        cpy.Height = cf.height>>(j?1:0);
//        auto ret = cuMemcpy2D(&cpy);
//        if(ret != CUDA_SUCCESS)
//        {
//            cuCtxPopCurrent(&cur);
//            return false;
//        }
//        offset += cf.height;
//    }

//    ret = cuCtxPopCurrent(&cur);
//    if(ret != CUDA_SUCCESS)
//        return false;
//    return true;
//}
