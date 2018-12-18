#include "encoder.h"
#include <QDebug>
#include <future>

//general configure: width, height, time_base, framerate,
//gop_size, pix_fmt, max_b_frames

//h264 configure: refs, qmax, qmin, bit_rate_tolerance,
//rc_min_rate, rc_max_rate, rc_buffer_size, rc_initial_buffer_occupancy,
//rc_initial_buffer_occpupancy, rc_buffer_assgressiveity, rc_initial_cplx, delay
//preset, tune, profile

//h265 configure:

#define BUF_SIZE 32768


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

class OutStream
{
public:
    OutStream(const std::string& url)
        : url_(url)
    {
    }

    virtual ~OutStream()
    {
        this->stopThread();
    }


    bool changeUrl(const std::string& url)
    {
        url_ = url;
        this->stopThread();
        if(!url_.empty())
            this->startThread();
        return true;
    }

    bool writeData(PacketPtr pkt)
    {
        mtx_.lock();
        pktQue_.push_back(pkt);
        mtx_.unlock();
        return true;
    }

protected :

    void startThread()
    {
        qCritical()<<"start output Thread url:"<<url_.c_str();
        running_.store(true);
        thread_ = std::thread(&OutStream::run, this, url_);
    }

    void stopThread()
    {
        if(!running_)
            return;

        qCritical()<<"stop output Thread url:"<<url_.c_str();
        running_.store(false);
        if(thread_.joinable())
            thread_.join();
    }

    PacketPtr getPacket()
    {
        mtx_.lock();
        PacketPtr pkt(nullptr);
        if(!pktQue_.empty())
        {
            pkt = pktQue_.front();
            pktQue_.pop_front();
        }
        mtx_.unlock();
        return pkt;
    }

    void clearPacket()
    {
        mtx_.lock();
        pktQue_.clear();
        mtx_.unlock();
    }

protected:
    virtual bool run(const std::string& url) = 0;

protected :
    std::string url_;
    std::atomic_bool running_{false};
    std::mutex mtx_;
    std::thread thread_;
    std::list<PacketPtr> pktQue_;

};

class NaluStream : public OutStream
{
public:
    using OutStream::OutStream;

    virtual ~NaluStream()
    {
        this->close();
    }

    bool open()
    {
        this->startThread();
        return true;
    }

    void close()
    {
        this->stopThread();
    }

protected:
    virtual bool run(const std::string& url) override
    {
        qCritical()<<"Output nalu url:"<<url.c_str();
        AVIOContext* pb = nullptr;
        while(running_)
        {
            if(!pb)
                avio_open(&pb, url.c_str(), AVIO_FLAG_WRITE);

            if(pb)
            {
                PacketPtr pkt = this->getPacket();
                if(pb->error)
                    avio_closep(&pb);
                else if(pkt != nullptr)
                {
                    avio_write(pb, pkt->data, pkt->size);
                }
            }
            else
                this->clearPacket();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        if(pb)
            avio_closep(&pb);
        qCritical()<<"Output nalu Exit url:"<<url.c_str();
        return true;
    }
};

class MuxerStream : public OutStream
{
public:
    MuxerStream(const std::string& url, const std::string& muxer)
        : OutStream(url)
        , muxer_(muxer){}

    virtual ~MuxerStream()
    {
        this->close();
    }

    bool open(AVCodec* avc, AVCodecContext* avctx)
    {
        AVOutputFormat* pout = av_guess_format(muxer_.c_str(), nullptr, nullptr);
        int ret = avformat_alloc_output_context2(&pFmt_, pout, nullptr, nullptr);
        if(ret != 0)
        {
            qCritical()<<"Url["<<url_.c_str()
                      <<"]avformat_alloc_output_context2 faild:"<<AV_ERR(ret);
            return false;
        }

        AVStream* os = avformat_new_stream(pFmt_,avc);
        ret = avcodec_parameters_from_context(os->codecpar, avctx);
        if(ret != 0)
        {
            qCritical()<<"Url["<<url_.c_str()
                      <<"]avcodec_parameters_from_context faild:"<<AV_ERR(ret);
            return false;
        }

        if(pFmt_->oformat->flags & AVFMT_GLOBALHEADER)
            pFmt_->oformat->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
        pFmt_->start_time_realtime = AV_NOPTS_VALUE;

        this->startThread();
        return true;
    }

    void close()
    {
        this->stopThread();
        avformat_free_context(pFmt_);
        pFmt_ = nullptr;
    }

protected:
    bool run(const std::string& url)
    {
        qCritical()<<"output muxer url:"<<url.c_str()<<", running:"<<running_;
        while(running_)
        {
            if(!pFmt_->pb)
            {
                auto ret = avio_open(&pFmt_->pb, url.c_str(), AVIO_FLAG_WRITE);
                if(ret == 0)
                {
                    qCritical()<<"output muxer url:"<<url.c_str()<<" success!";
                    avformat_write_header(pFmt_, nullptr);
                }
            }

            if(pFmt_->pb)
            {
                PacketPtr pkt = this->getPacket();
                if( pkt != nullptr && 0 > av_write_frame(pFmt_, pkt.get()))
                    avio_closep(&pFmt_->pb);
            }
            else
                this->clearPacket();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        if(pFmt_->pb)
            avio_closep(&pFmt_->pb);

        qCritical()<<"output muxer exit:"<<url.c_str();
        return true;
    }

private:
    AVFormatContext* pFmt_{nullptr};
    std::string muxer_;
};

/****************************************************************
 * **************************************************************
 * **************************************************************/


Encoder::Encoder(const OutParam &muxerParam, const std::string &naluUrl, const std::string &fileName, int maxCached)
    : fileName_(fileName)
    , c_maxCached_(maxCached)
    , pMuxerStream_(new MuxerStream(muxerParam.url,muxerParam.muxer))
    , pNaluStream_(new NaluStream(naluUrl))
{
    pFile_ = fopen(fileName_.c_str(), "wb+");
}

Encoder::~Encoder()
{
    this->close();
    if(pFile_)
    {
        fclose(pFile_);
        pFile_ = nullptr;
    }
}

bool Encoder::open(const EncodeParam& param, AVBufferRef *pDevice)
{
    if(createEncoder(param.codecName.c_str())
            && setParam(param, pDevice)
            && openEncoder()
            && openRemoveBsf()
            && pMuxerStream_->open(pCodec_, pCodecCtx_)
            && pNaluStream_->open())
    {
        return true;
    }

    this->close();
    return false;
}

void Encoder::close()
{
    if(encoding_)
        encoding_.store(false);
    if(encodeThread_.joinable())
        encodeThread_.join();

    pMuxerStream_->close();
    pNaluStream_->close();

    this->closeEncoder();
    this->freeEncoder();
}

bool Encoder::isRunning()
{
    if(!encoderOpened())
        return false;

    return encoding_;
}

void Encoder::run()
{
    if(encoding_)
        return ;

    if(!encoderOpened())
        return ;

    encoding_ = true;
    encodeThread_ = std::thread(&Encoder::encode,this);
}

void Encoder::send(FramePtr pFrame)
{
    frameMtx_.lock();
    if(frameQue_.size() < c_maxCached_)
        frameQue_.push(pFrame);
    else if(++dropNo_ > c_maxCached_)
    {
        qWarning()<<"Encoder drop no:"<<c_maxCached_;
        dropNo_ = 0;
    }
    frameMtx_.unlock();
}

bool Encoder::changeMuxerUrl(const std::string &url)
{
    if(url.empty())
        return false;
    return pMuxerStream_->changeUrl(url);
}

bool Encoder::createEncoder(const char *name)
{
    pCodec_ = avcodec_find_encoder_by_name(name);
    if(!pCodec_)
    {
        qCritical()<<"avcodec_find_encoder_by_name error,name:"<<name;
        return false;
    }

    pCodecCtx_ = avcodec_alloc_context3(pCodec_);

    if(!pCodecCtx_)
    {
        pCodec_ = nullptr;
        qCritical()<<"avcodec_alloc_context3 error!";
        return false;
    }
    return true;
}

void Encoder::freeEncoder()
{
    if(pCodecCtx_)
        avcodec_free_context(&pCodecCtx_);
    if(pCodec_)
        pCodec_ = nullptr;
}

bool Encoder::encoderExist()
{
    return (pCodecCtx_ && pCodec_);
}

bool Encoder::setParam(const EncodeParam& param, AVBufferRef *pHWCtx)
{
    if(param.codecName.find("hevc") != std::string::npos)
    {
        av_dict_set(&pCodecDict_, "x265-params", "crf=25", 0);
        av_dict_set(&pCodecDict_, "preset", "fast", 0);
        av_dict_set(&pCodecDict_, "tune", "zero-latency", 0);
    }
    else
    {
        if(!pHWCtx)
            av_dict_set(&pCodecDict_, "preset", "ultrafast", 0);
        else
            av_dict_set(&pCodecDict_, "preset", "fast", 0);
        av_dict_set(&pCodecDict_, "tune", "zerolatency", 0);
        av_dict_set(&pCodecDict_, "profile", param.profile.c_str(), 0);
        av_dict_set(&pCodecDict_, "vbr","1",0);
    }
    av_dict_set(&pCodecDict_, "threads", "4", 0);

    pCodecCtx_->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecCtx_->pix_fmt = AV_PIX_FMT_NV12;
    pCodecCtx_->width = param.width;
    pCodecCtx_->height = param.height;
    pCodecCtx_->gop_size = param.gop;
    pCodecCtx_->time_base.num = 1;
    pCodecCtx_->time_base.den = param.fps;
    pCodecCtx_->framerate.num = param.fps;
    pCodecCtx_->framerate.den = 1;
    pCodecCtx_->bit_rate = param.bitrate;
    pCodecCtx_->max_b_frames = 0;
    {
        pCodecCtx_->refs = 1;
        pCodecCtx_->qmax = 51;
        pCodecCtx_->qmin = 10;
        pCodecCtx_->bit_rate_tolerance = pCodecCtx_->bit_rate;
        pCodecCtx_->rc_min_rate = pCodecCtx_->bit_rate;
        pCodecCtx_->rc_max_rate = pCodecCtx_->bit_rate*3/2;
        pCodecCtx_->rc_buffer_size = pCodecCtx_->bit_rate;
        pCodecCtx_->rc_initial_buffer_occupancy = pCodecCtx_->rc_buffer_size * 3 / 4;
        pCodecCtx_->delay = 0;
    }

    if(pHWCtx)
    {
        AVHWFramesContext* pfc = reinterpret_cast<AVHWFramesContext*>(pHWCtx->data);
        pCodecCtx_->pix_fmt = pfc->format;
        pCodecCtx_->sw_pix_fmt = pfc->sw_format;
        pCodecCtx_->hw_frames_ctx = av_buffer_ref(pHWCtx);
    }

    return true;
}

bool Encoder::openEncoder()
{
    if(!encoderExist())
    {
        qCritical()<<"can not open Encoder before created.";
        return false;
    }

    auto ret = avcodec_open2(pCodecCtx_, pCodec_, &pCodecDict_);
    if ( ret < 0)
    {
        qCritical()<<"avcodec_open2 failed:"<<AV_ERR(ret);
        return false;
    }
    return true;
}

void Encoder::closeEncoder()
{
    if(encoderOpened())
        avcodec_close(pCodecCtx_);
}

bool Encoder::openRemoveBsf()
{
    std::string removeType;
    if(pCodec_->id == AV_CODEC_ID_H264)
        removeType.assign("6");
    else if(pCodec_->id == AV_CODEC_ID_HEVC)
        removeType.assign("39-40");
    else
    {
        qCritical()<<"Encoder do not support id:"<<pCodec_->id;
        return false;
    }

    const AVBitStreamFilter* f = av_bsf_get_by_name("filter_units") ;
    if(!f)
    {
        qCritical()<<"Encoder av_bsf_get_by_name(filter_units) failed!";
        return false;
    }
    int ret = av_bsf_alloc(f,&pBsfCtx_);
    if(ret != 0)
    {
        qCritical()<<"Encoder av_bsf_alloc failed:"<<AV_ERR(ret);
        return false;
    }

    ret = avcodec_parameters_from_context(pBsfCtx_->par_in, pCodecCtx_);
    if(ret != 0)
    {
        qCritical()<<"Encoder avcodec_parameters_from_context failed:"<<AV_ERR(ret);
        return false;
    }
    pBsfCtx_->time_base_in = pCodecCtx_->time_base;
    av_opt_set(pBsfCtx_->priv_data, "remove_types", removeType.c_str(), 0);
    ret = av_bsf_init(pBsfCtx_);
    if(ret != 0)
    {
        qCritical()<<"Encoder av_bsf_init failed:"<<AV_ERR(ret);
        return false;
    }

    return true;
}

void Encoder::closeRemoveBsf()
{
    if(pBsfCtx_)
    {
        av_bsf_free(&pBsfCtx_);
        pBsfCtx_ = nullptr;
    }
}

bool Encoder::encoderOpened()
{
    return (pCodecCtx_ && avcodec_is_open(pCodecCtx_) > 0);
}

void Encoder::encode()
{
    while(encoding_)
    {
        if(frameQue_.empty())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }

        frameMtx_.lock();
        FramePtr pFrame = frameQue_.front();
        frameQue_.pop();
        frameMtx_.unlock();
        auto now = av_gettime_relative();
        pFrame->pts = av_rescale_q(now, AVRational{1, AV_TIME_BASE},pCodecCtx_->time_base);
        //pFrame->pts = (++frameNo_)* (int)(1000*av_q2d(pCodecCtx_->time_base));

        int ret = avcodec_send_frame(pCodecCtx_, pFrame.get());
        if(ret != AVERROR(EAGAIN) && ret != 0)
        {
            qCritical()<<"error:"<<AV_ERR(ret);
            encoding_.store(false);
            break;
        }

        while(true)
        {
            PacketPtr pkt(av_packet_alloc());
            auto ret1 = avcodec_receive_packet(pCodecCtx_, pkt.get());
            if(ret1 != 0)
                break;

            if(0 == av_bsf_send_packet(pBsfCtx_, pkt.get())
                    && 0 == av_bsf_receive_packet(pBsfCtx_,pkt.get()))
            {
                pMuxerStream_->writeData(pkt);
                pNaluStream_->writeData(pkt);
            }
        }
    }
}
