//#ifndef STITCH_NVIDA_H
//#define STITCH_NVIDA_H

//#include <string>
//#include <vector>
//#include "codec/codec.h"

//struct CudaFrame
//{
//    CudaFrame(int w, int h)
//        :width(w),height(h),pitch(0),data(0){}
//    size_t width;
//    size_t height;
//    size_t pitch;
//    CUdeviceptr data;
//};

//class TrstPano;
//class StitchNvida
//{
//public:

//    StitchNvida(const std::string &device, const char* stitchRoot);

//    ~StitchNvida();

//    bool initHardware();

//    bool setInput(int count, int width, int height);

//    bool setOutput(int width, int height);

//    bool addChildOuput(int width, int height);

//    bool start();

//    void stop();

//    bool isStarted();

//    bool stitch(const std::vector<FramePtr>& inVec, FramePtr out);

//    void correctChroma(unsigned char* src[]);

//private:

//    bool hasHWInited();

//    bool allocHWMem(CudaFrame &cf);

//    bool freeHWMem(CudaFrame& cf);

//    bool dllInit();

//    void dllUninit();

//    bool isInited();

//    bool dllStitch(unsigned char **ins, unsigned char *out);

//    bool copyFromFFmpeg_NV12(const FramePtr& fp, CudaFrame& cf);

//    bool copyToFFmpeg_NV12(const CudaFrame& cf, FramePtr& fp);

//private:
//    CUcontext pCtx_{nullptr};
//    CUdevice pDevice_{0};
//    const int no_;
//    const std::string root_;

//    TrstPano* pPano_{nullptr};
//    int dllHandle_{-1};

//    std::vector<CudaFrame> inputs_;
//    std::vector<CudaFrame> outputs_;
//    unsigned char** inputNv12s_;

//    unsigned char** chroma_{nullptr};
//};

//#endif // STITCH_NVIDA_H
