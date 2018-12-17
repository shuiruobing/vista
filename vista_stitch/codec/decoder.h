#ifndef DECODER_H
#define DECODER_H

#include <QObject>
#include <string>
#include <thread>
#include <atomic>
#include <memory>
#include <mutex>
#include <queue>
#include "codec.h"
#include <iostream>

using namespace std::chrono;

enum class CodecType
{
    Software = 0,
    Nvidia = 1,
    Amd = 2,
    Intel = 3
};


///
/// \brief The Decoder class
///
///
class Decoder : public QObject
{
    Q_OBJECT
public:

    Decoder(int id, const std::string& url, int cachePacketGops,
            int cacheFrameCount, const std::string& fileName);

    ~Decoder();

    void open(AVBufferRef* pd);

    void close();

    const StreamInfo info();

    FramePtr getFrame();

signals:

    Q_INVOKABLE void opened(int id);

    Q_INVOKABLE void error(int id, QString errStr);

private:

    void workThreadFunc(AVBufferRef* pd);

    void decodeThreadFunc(std::atomic_bool& running);

    bool openInput(const char* url);

    void closeInput();

    inline bool inputOpened();

    bool createDecoder();

    void freeDecoder();

    inline bool decoderExist();

    bool openDecoder();

    void closeDecoder();

    bool decoderOpened();

    bool createBSF();

    void freeBSF();

    bool createSws();

    void freeSws();

    FramePtr swsFrame(FramePtr pFrame);

    inline bool bsfExist();

    bool setDevice(AVBufferRef *pd);

    bool readPacket();

    bool decodePacket(PacketPtr pkt);

    int getFps(AVStream* st);

    static int interruptVideoStream(void* opauqe);

private:
    const int c_id_;
    const std::string c_url_;
    const int c_cachedMaxGops_;
    const int c_cachedMaxFrames_;
    const std::string fileName_;

    std::thread workThread_;
    std::atomic_bool working_{false};

    AVFormatContext* pFmtCtx_{nullptr};
    AVStream* pStream_{nullptr};
    AVCodecContext* pCodecCtx_{nullptr};
    AVDictionary* pFmtDict_{nullptr};
    AVDictionary* pCodecDict_{nullptr};
    AVBSFContext* pBsfCtx_{nullptr};
    AVCodec* pCodec_{nullptr};
    SwsContext* pSwsCtx_{nullptr};

    std::mutex packetMtx_;
    std::mutex frameMtx_;
    std::queue<PacketPtr> packetQue_;
    std::queue<FramePtr> frameQue_;

    QString errorStr_;

    //statistics
    std::atomic_int width_{0};
    std::atomic_int height_{0};
    std::atomic_int fps_{0};
    std::atomic_int nearFps_{0};
    std::atomic_int avgFps_{0};

    FILE* pFile_{nullptr};
    system_clock::time_point lastPacketTime_;
};

#endif // DECODER_H
