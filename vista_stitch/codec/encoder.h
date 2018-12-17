#ifndef ENCODER_H
#define ENCODER_H

#include <queue>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <functional>
#include <shared_mutex>
#include "codec.h"

//enum EncodeType{
//    ABV,
//    QP
//};
//template <EncodeType ET> class EncoderParam;
//template<> class EncoderParam<ABV>
//{
//public:
//    static bool Set(AVCodecContext* avctx)
//    {

//    }
//};

//struct CRF
//{
//    char scale;     //from 0-51, defalut 23
//    std::string preset;//ultrafast superfast veryfast faster fast
//                       //medium default
//                       //slow slower veryslow /*placebo*/
//    std::string tune;  //film animation grain stillimage fastdecode
//                       //zero latency psnr ssim
//    std::string profile;//baseline main high high10 high422 high444
//};

//struct TwoPass
//{

//};

struct EncodeParam
{
    std::string codecName;
    int width{0};
    int height{0};
    int gop{0};
    int fps{0};
    int bitrate{0};
    std::string profile;
};

struct OutParam
{
    std::string url;
    std::string muxer;
};

class NaluStream;
class MuxerStream;
class Encoder
{
public:

    Encoder(const OutParam& muxerParam, const std::string& naluUrl, const std::string& fileName, int maxCached = 20);

    ~Encoder();

    bool open(const EncodeParam& param, AVBufferRef *pDevice = nullptr);

    void close();

    bool isRunning();

    void run();

    void send(FramePtr pFrame);

    bool changeMuxerUrl(const std::string& url);

private:

    bool createEncoder(const char* name);

    void freeEncoder();

    inline bool encoderExist();

    bool setParam(const EncodeParam& param, AVBufferRef *pHWCtx = nullptr);

    bool openEncoder();

    void closeEncoder();

    bool openRemoveBsf();

    void closeRemoveBsf();

    inline bool encoderOpened();

    bool openEncoder(AVBufferRef *pDevice);

    void encode();

private:
    const std::string fileName_;
    int c_maxCached_{20};
    AVCodecContext* pCodecCtx_{nullptr};
    AVCodec* pCodec_{nullptr};
    AVDictionary* pCodecDict_{nullptr};
    AVBSFContext* pBsfCtx_{nullptr};
    std::mutex frameMtx_;
    std::queue<FramePtr> frameQue_;
    std::thread encodeThread_;
    std::atomic_bool encoding_{false};

    NaluStream* pNaluStream_;
    MuxerStream* pMuxerStream_;

    std::size_t frameNo_{0};

    int dropNo_{0};

    FILE* pFile_{nullptr};
};

#endif // ENCODER_H
