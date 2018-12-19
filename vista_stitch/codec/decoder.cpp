#include "decoder.h"
#include <QDebug>

#define ERR_OUT(FUNC_NAME, CODE) qCritical()\
    <<"camera["<<c_id_<<"]"\
    <<#FUNC_NAME<<" error:("<<ret<<")"<<AV_ERR(CODE)

#define WARN_OUT(FUNC_NAME, CODE) qWarning()\
    <<"camera["<<c_id_<<"]"\
    <<#FUNC_NAME<<" error:("<<ret<<")"<<AV_ERR(CODE)

#define DEBUG_OUT() qDebug()<<"camera["<<c_id_<<"] "

namespace {
    struct HWContent
    {
        AVBufferRef* device; //ref AVHWDeviceContext
    };

    AVPixelFormat getPixFmt(AVHWDeviceType type, AVCodec* pc)
    {
        if(!pc || type == AV_HWDEVICE_TYPE_NONE)
            return AV_PIX_FMT_NONE;

        AVPixelFormat pix = AV_PIX_FMT_NONE;
        for(int i = 0; ; i++)
        {
            const AVCodecHWConfig* config = avcodec_get_hw_config(pc,i);
            if(!config)
                break;
            if(config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX
                    && config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_FRAMES_CTX
                    && config->device_type == type)
            {
                pix = config->pix_fmt;
                break;
            }
        }
        return pix;

        //qDebug()<<(int)config->device_type; //4,0,7,2
    }
}

Decoder::Decoder(int id, const std::string &url, int cachePacketGops, int cacheFrameCount, const std::string &fileName)
    : c_id_(id)
    , c_url_(url)
    , c_cachedMaxGops_(cachePacketGops)
    , c_cachedMaxFrames_(cacheFrameCount)
    , fileName_(fileName)
{
//    if(! av_dict_get(pFmtDict_, "rtsp_transport", nullptr, 0))
//    {
//        av_dict_set(&pFmtDict_, "rtsp_transport", "tcp",0);
//    }
//    av_dict_set(&pFmtDict_, "stimeout", "5000000", 0);   //tcp 5s
//    av_dict_set(&pFmtDict_, "max_delay", "5000000", 0);
//    av_dict_set(&pFmtDict_, "fflags", "nobuffer", 0);
//    av_dict_set(&pFmtDict_, "recv_buffer_size", "100000", 0);
//    av_dict_set(&pFmtDict_, "probesize", "500000", 0);
//    av_dict_set(&pFmtDict_, "analyzeduration", "10000", 0);
    lastPacketTime_ = steady_clock::now();
}

Decoder::~Decoder()
{
    this->close();
}

void Decoder::open(const Options& fmtOpts, const Options& decodeOpts, AVBufferRef *pd)
{
    qDebug()<<"["<<c_id_<<"]Will open url:"<<c_url_.c_str();

    //设置format参数和解码参数
    for(auto e : fmtOpts)
        av_dict_set(&pFmtDict_, e.first.c_str(), e.second.c_str(), 0);
    for(auto e : decodeOpts)
        av_dict_set(&pCodecDict_, e.first.c_str(), e.second.c_str(), 0);

    //启动线程
    working_ = true;
    workThread_ = std::thread(&Decoder::workThreadFunc, this, pd);
}

void Decoder::close()
{
    if(working_)
        working_ = false;
    if(workThread_.joinable())
        workThread_.join();
}

const StreamInfo Decoder::info()
{
    return StreamInfo{width_.load()
                ,height_.load()
                ,fps_.load()
                ,nearFps_.load()
                ,avgFps_.load()
    };
}

FramePtr Decoder::getFrame()
{
    if(frameQue_.empty())
        return FramePtr(nullptr);

    frameMtx_.lock();
    FramePtr pFrame = frameQue_.front();
    frameQue_.pop();
    frameMtx_.unlock();
    return pFrame;
}

void Decoder::workThreadFunc(AVBufferRef *pd)
{
#define SEND_SUCCESS() do{\
    QMetaObject::invokeMethod(this, "opened" \
    ,Qt::QueuedConnection ,Q_ARG(int,c_id_));}while(0)
#define SEND_FAILED() do{\
    QMetaObject::invokeMethod(this, "error", Qt::QueuedConnection\
    ,Q_ARG(int,c_id_), Q_ARG(QString, "decoder error!"));}while(0)


    std::atomic_bool decoding{false};
    do
    {
        if(openInput(c_url_.c_str()) && createDecoder() /*&& createBSF()*/
           && (pd ? setDevice(pd):true) && openDecoder()
           && (!pd ? createSws(): true))
        {
            SEND_SUCCESS();
            decoding.store(true);
            std::thread decodeThread(&Decoder::decodeThreadFunc,this, std::ref(decoding));
            while(working_ && decoding && readPacket());

            qCritical()<<"Decoder["<<c_id_<<"] decoder thread will exit.";
            decoding.store(false);
            decodeThread.join();
            qCritical()<<"Decoder["<<c_id_<<"] decoder thread exited.";
        }

        freeSws();
        closeDecoder();
//        freeBSF();
        freeDecoder();
        closeInput();

        if(working_)
            SEND_FAILED();

    }while(working_);
    qCritical()<<"Decoder["<<c_id_<<"] reading thread will exit.";
}

void Decoder::decodeThreadFunc(std::atomic_bool &running)
{
    while(running)
    {
        if(packetQue_.empty())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        packetMtx_.lock();
        PacketPtr pkt = packetQue_.front();
        packetQue_.pop();
        packetMtx_.unlock();

        if(!decodePacket(pkt))
        {
            qCritical()<<"["<<c_id_<<"] decoder failed.";
            running.store(false);
        }
    }
}


bool Decoder::openInput(const char *url)
{
    auto ret = avformat_open_input(&pFmtCtx_, url, nullptr, &pFmtDict_);
    if(ret)
    {
        ERR_OUT(avformat_open_input, ret);
        return false;
    }
    this->lastPacketTime_ = steady_clock::now();
    pFmtCtx_->interrupt_callback.callback = &Decoder::interruptVideoStream;
    pFmtCtx_->interrupt_callback.opaque = this;

    pFmtCtx_->flags |= AVFMT_FLAG_NONBLOCK;
    qDebug()<<"["<<c_id_<<"]"<<__FUNCTION__<<"success!";
    return true;
}

void Decoder::closeInput()
{
    if(inputOpened())
        avformat_close_input(&pFmtCtx_);
}

bool Decoder::inputOpened()
{
    return (pFmtCtx_ != nullptr);
}

bool Decoder::createDecoder()
{
    if(!inputOpened())
        return false;

    auto ret = avformat_find_stream_info(pFmtCtx_, nullptr);
    if(ret != 0)
    {
        ERR_OUT(avformat_find_stream_info,ret);
        return false;
    }

    int vi = av_find_best_stream(pFmtCtx_, AVMEDIA_TYPE_VIDEO, -1, -1, &pCodec_, 0);
    if(vi < 0 || nullptr == pCodec_)
    {
        ERR_OUT(av_find_best_stream,vi);
        return false;
    }

    pStream_ = pFmtCtx_->streams[vi];
    pCodecCtx_ = avcodec_alloc_context3(pCodec_);
    ret = avcodec_parameters_to_context(pCodecCtx_, pStream_->codecpar);
    if(ret != 0)
    {
        ERR_OUT(avcodec_parameters_to_context, ret);
        avcodec_free_context(&pCodecCtx_);
        return false;
    }

    pCodecCtx_->sw_pix_fmt = pCodecCtx_->pix_fmt;
    pCodecCtx_->pkt_timebase = pStream_->time_base;
    av_dump_format(pFmtCtx_, 0, c_url_.c_str(), 0);

    width_.store(pCodecCtx_->width);
    height_.store(pCodecCtx_->height);
    fps_.store(getFps(pStream_));

    std::string fileName = fileName_ + "." + pCodec_->name;
    fopen_s(&pFile_,fileName.c_str(), "wb+");

    qDebug()<<"["<<c_id_<<"]"<<__FUNCTION__<<"success!";
    return true;
}


void Decoder::freeDecoder()
{
    if(decoderExist())
    {
        avcodec_free_context(&pCodecCtx_);
        pCodec_ = nullptr;
        pStream_ = nullptr;
    }

    if(pFile_)
    {
        fclose(pFile_);
        pFile_ = nullptr;
    }
}

bool Decoder::decoderExist()
{
    return (pCodecCtx_ != nullptr && pCodec_ != nullptr && pStream_ != nullptr);
}

bool Decoder::openDecoder()
{
    auto ret = avcodec_open2(pCodecCtx_, pCodec_, &pCodecDict_);
    if(ret != 0)
    {
        qCritical()<<"avcodec_open2 failed:"<<ret;
        return false;
    }
    qDebug()<<"["<<c_id_<<"]"<<__FUNCTION__<<"success! decoder name:"<<pCodec_->name;
    return true;
}

void Decoder::closeDecoder()
{
    if(decoderOpened())
    {
        avcodec_close(pCodecCtx_);
        pCodecCtx_ = nullptr;
    }
}

bool Decoder::decoderOpened()
{
    return (decoderExist() &&(avcodec_is_open(pCodecCtx_) > 0));
}

bool Decoder::createBSF()
{
//    void* bsfIter = nullptr;
//    while(true)
//    {
//        const AVBitStreamFilter* pBsf = av_bsf_next(&bsfIter);
//        if( pBsf == nullptr )
//        {
//            DEBUG_OUT()<<"Can not find bsf.";
//            break;
//        }

//        if( nullptr == pBsf->codec_ids
//                || *(pBsf->codec_ids) != pCodecCtx_->codec_id)
//            continue;

//        auto ret = av_bsf_alloc(pBsf, &pBsfCtx_);
//        if(0 != ret)
//        {
//            ERR_OUT(av_bsf_alloc,ret);
//            break;
//        }

//        avcodec_parameters_from_context(pBsfCtx_->par_in, pCodecCtx_);
//        pBsfCtx_->time_base_in = pCodecCtx_->time_base;

//        ret = av_bsf_init(pBsfCtx_);
//        if(ret != 0 )
//        {
//            ERR_OUT(av_bsf_init,ret);
//            break;
//        }
//        DEBUG_OUT()<<"Create bsf success, name:"<<pBsf->name;
//    }

//    return pBsfCtx_ != nullptr;
    return false;
}

void Decoder::freeBSF()
{
    if(bsfExist())
        av_bsf_free(&pBsfCtx_);
}

bool Decoder::createSws()
{
    if(!decoderOpened())
        return false;
    pSwsCtx_ = sws_getContext(pCodecCtx_->width, pCodecCtx_->height, pCodecCtx_->pix_fmt
                              ,pCodecCtx_->width, pCodecCtx_->height, AV_PIX_FMT_NV12
                              ,SWS_BICUBIC, nullptr, nullptr, nullptr);
    return pSwsCtx_ != nullptr;
}

void Decoder::freeSws()
{
    if(pSwsCtx_)
    {
        sws_freeContext(pSwsCtx_);
        pSwsCtx_ = nullptr;
    }
}

FramePtr Decoder::swsFrame(FramePtr pFrame)
{
    if(!pFrame || !pSwsCtx_)
        return FramePtr(nullptr);

    FramePtr newFrame(av_frame_alloc());
    newFrame->width = pFrame->width;
    newFrame->height = pFrame->height;
    newFrame->format = AV_PIX_FMT_NV12;
    av_frame_get_buffer(newFrame.get(), 16);

    sws_scale(pSwsCtx_, pFrame->data, pFrame->linesize, 0, pFrame->height
              , newFrame->data, newFrame->linesize);
    return newFrame;
}

bool Decoder::bsfExist()
{
    return (pBsfCtx_ != nullptr);
}

bool Decoder::setDevice(AVBufferRef *pd)
{
    if(!pCodecCtx_ || !pCodec_ || !pd)
        return false;

    auto phwdc = reinterpret_cast<AVHWDeviceContext*>(pd->data);
    if(!phwdc)
        return false;

    auto pix = getPixFmt(phwdc->type, pCodec_);
    if(pix == AV_PIX_FMT_NONE)
        return false;

    pCodecCtx_->pix_fmt = pix;
    pCodecCtx_->hw_device_ctx = av_buffer_ref(pd);
    pCodecCtx_->sw_pix_fmt = AV_PIX_FMT_NV12;

    return true;
}

bool Decoder::readPacket()
{
    PacketPtr pkt(av_packet_alloc());
    int ret = av_read_frame(pFmtCtx_, pkt.get());
    if(ret != 0)
    {
        WARN_OUT(av_read_frame,ret);
        return false;
    }

    int maxPkts = pCodecCtx_->gop_size * c_cachedMaxGops_;
    if(pkt->stream_index == pStream_->index)
    {
        this->lastPacketTime_ = steady_clock::now();
        packetMtx_.lock();
        if(packetQue_.size() > static_cast<size_t>(maxPkts))
            qCritical()<<"["<<c_url_.c_str()<<"]"<<" drop packet.";
        else
            packetQue_.push(pkt);
        packetMtx_.unlock();
    }
    return true;

}

bool Decoder::decodePacket(PacketPtr pkt)
{
    auto ret = avcodec_send_packet(pCodecCtx_, pkt.get());
    if(ret != 0 && ret != AVERROR(EAGAIN))
    {
        ERR_OUT(avcodec_send_packet,ret);
        return false;
    }
    int ret1 = 0;
    while(true)
    {
        FramePtr pFrame(av_frame_alloc());
        ret1 = avcodec_receive_frame(pCodecCtx_, pFrame.get());
        if(ret1 != 0)
            break;
        if(!pFrame->hw_frames_ctx)
            pFrame = swsFrame(pFrame);

        frameMtx_.lock();
        if(frameQue_.size() > static_cast<size_t>(c_cachedMaxFrames_))
            qWarning()<<"decoder id:"<<c_id_<<"drop some frame";
        else
            frameQue_.push(pFrame);
        frameMtx_.unlock();
    }
    if(ret1 != AVERROR(EAGAIN))
    {
        ERR_OUT(avcodec_receive_frame,ret1);
        return false;
    }
    return true;
}

int Decoder::getFps(AVStream *st)
{
    //fps
    if(st->avg_frame_rate.den && st->avg_frame_rate.num)
        return static_cast<int>(av_q2d(st->avg_frame_rate));

    //tbr
    if(st->r_frame_rate.den && st->r_frame_rate.num)
        return static_cast<int>(av_q2d(st->r_frame_rate));

//    int tbn = st->time_base.den && st->time_base.num;
//    if(tbn) return tbn;
//    int tbc = st->codec->time_base.den && st->codec->time_base.num;
//    if(tbc) return tbc;
    return 0;
}

int Decoder::interruptVideoStream(void *opauqe)
{
    Decoder* p = static_cast<Decoder*>(opauqe);
    if(!p)
        return 1;

    seconds span = duration_cast<seconds>(steady_clock::now() - p->lastPacketTime_);
    if(span.count() > 15)
    {
        qWarning()<<"Decoder["<<p->c_id_<<"] time out(5s)";
        return 1;
    }
    return 0;
}
