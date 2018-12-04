#ifndef STITCH_CUDA_H
#define STITCH_CUDA_H

#include "codec/codec.h"
#include <vector>

struct CudaFrame
{
    CudaFrame(int w, int h)
        :width(w),height(h),pitch(0),data(0){}
    size_t width;
    size_t height;
    size_t pitch;
    CUdeviceptr data;
};

class StitchCuda
{
public:
    explicit StitchCuda(const char* stitchRoot);

    ~StitchCuda();

    bool setHardware(const std::string &device);

    bool setInput(int count, int width, int height);

    bool setOutput(int width, int height);

    bool open();

    bool stitch(const std::vector<FramePtr>& ins, FramePtr out);

    bool resize(const FramePtr& src, FramePtr dst);

private:
    inline bool hasHW();

    bool allocMem(CudaFrame& cf);

    bool freeMem(CudaFrame& cf);

    bool fromAVframeNv12(const FramePtr& fp, CudaFrame& cf);

    bool toAVFrameNV12(const CudaFrame& cf, FramePtr& fp);

    bool stitchImpl(CUdeviceptr* in, CUdeviceptr& out);

private:
    const std::string root_;
    int device_{0};
    CUcontext pCtx_{nullptr};
    std::vector<CudaFrame> inputs_;
    CudaFrame* pOutput_{nullptr};
    CUdeviceptr* nv12s_;
};

#endif // STITCH_CUDA_H
