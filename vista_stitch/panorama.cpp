#include "panorama.h"
#include "configure.h"
#include <QDebug>
#include <algorithm>
#include <QCoreApplication>

//[image width] x [image height] x [framerate] x [motion rank] x 0.07 = [desired bitrate].
#define H264_COMPRESS_RATIO 0.07f
#define HEVC_COMPRESS_RATIO 0.04f


#define CUDA_ALIGN 16

#define ARRAY_ELEMS(a) (sizeof(a) / sizeof((a)[0]))

namespace{
AVHWDeviceType nameToType(const char* name)
{
    if(strcmp(name,"nvida") == 0)
        return AV_HWDEVICE_TYPE_CUDA;
    return AV_HWDEVICE_TYPE_NONE;
}

bool setEncodeParam(const cfg::OutputNode& on, const cfg::StitchNode& sn, const StreamBase& inputInfo, const int iiCount,EncodeParam& ep)
{
    if(!inputInfo.hasData() || iiCount <= 0)
        return false;

    std::string codecId = on.encode.codecId;
    std::string profile = on.encode.profile;
    std::string mode = sn.mode;
    std::transform(std::begin(codecId), std::end(codecId), std::begin(codecId), ::tolower);
    std::transform(std::begin(profile), std::end(profile), std::begin(profile), ::tolower);
    std::transform(std::begin(mode),std::end(mode),std::begin(mode),::tolower);

    int width = inputInfo.width;
    int height = inputInfo.height;
    if(mode.find("horizontal") != std::string::npos)
        width = inputInfo.width * iiCount;
    else if(mode.find("vertical") != std::string::npos)
        height = inputInfo.height * iiCount;
    else
        return false;

    float compress = 0;
    std::string codecName;
    if(codecId.find("h264") != std::string::npos)
    {
        compress = H264_COMPRESS_RATIO;
        codecName = on.encode.hardware ? "h264_nvenc" : "libx264";
    }else if(codecId.find("hevc") != std::string::npos
             ||codecId.find("h265") != std::string::npos ){
        compress = HEVC_COMPRESS_RATIO;
        codecName = on.encode.hardware ? "hevc_nvenc" : "libx265";
    }else
        return false;

    int bitrate = (int)(width * height * inputInfo.fps * compress);
    int bitrateMax = bitrate * 8;
    int bitrateMin = bitrate / 8;
    if(profile.find("baseline") != std::string::npos)
        bitrate /= 2;
    else if(profile.find("high") != std::string::npos)
        bitrate *= 2;
    else if(profile.find("main") != std::string::npos)
        bitrate;//do nothing
    else
        return false;

    if(on.encode.bitrate < bitrateMin
            || on.encode.bitrate > bitrateMax)
        ep.bitrate = bitrate;
    else
        ep.bitrate = on.encode.bitrate;
    ep.codecName.swap(codecName);
    ep.fps = inputInfo.fps;
    if(ep.gop <= 0)
        ep.gop = inputInfo.fps;
    ep.height = height;
    ep.width = width;
    ep.profile.swap(profile);
    return true;
}

}

Panorama::Panorama(const cfg::PanoNode &pn, QObject *parent)
    : QObject(parent)
    , param_(pn)
    , c_inputCount_((int)pn.input.devices.size())
    , c_decodeHW_(pn.input.hardware)
    , c_encodeHW_(pn.output.encode.hardware)
    , c_deviceNo_(pn.hardwareNo)
    , pStitchTimer_(new QTimer(this))
    , pPanoFrame_(av_frame_alloc())
    , stdinThread_(&Panorama::stdinThreadFunc,this)
{
    pStitchTimer_->setInterval(40);
    pStitchTimer_->setTimerType(Qt::PreciseTimer);

    cameraFrames_.insert(std::end(cameraFrames_),
                         c_inputCount_, FramePtr(nullptr));
    streamList_.insert(std::end(streamList_), c_inputCount_, StreamInfo());

    decoders_.reserve(c_inputCount_);
    for(int i = 0 ; i < c_inputCount_; i++)
    {
        Decoder* pd = new Decoder(i, param_.input.devices.at(i).url
                                  , param_.input.maxGopCached
                                  , param_.input.maxGopCached);
        connect(pd,&Decoder::opened,this,&Panorama::onDecoderOpened);
        decoders_.push_back(pd);
    }

    mainEncoder_ = new Encoder({param_.output.multicast,"mpegts"},param_.output.unicast);
    pStitcher_ = new StitchCuda("");
    connect(pStitchTimer_, &QTimer::timeout, this, &Panorama::onStitch);
}

Panorama::~Panorama()
{
    stdinRunning_.store(false);
    stdinThread_.join();

    pStitchTimer_->stop();
    pStitchTimer_->deleteLater();

    if(pStitcher_) delete pStitcher_;

    if(mainEncoder_)
    {
        mainEncoder_->close();
        delete mainEncoder_;
    }

    for(Decoder* p : decoders_)
    {
        if(p)
        {
            p->close();
            delete p;
        }
    }

    if(pHWEncCtx_)
        av_buffer_unref(&pHWEncCtx_);
    if(pDevice_)
        av_buffer_unref(&pDevice_);
}

bool Panorama::open()
{
    if(!libInit())
        return false;
    if((c_decodeHW_ || c_encodeHW_)
            && !this->createDevice(c_deviceNo_.c_str()))
    {
        return false;
    }

    for(Decoder* pd : decoders_)
        pd->open(c_decodeHW_ ? pDevice_ : nullptr);
    return true;
}

void Panorama::onDecoderOpened(int id)
{
#define F_ASSERT(COND,ERR_STR)do{     \
    if(!(COND))                       \
    {                                 \
        this->onFatalError(ERR_STR);  \
        return;                       \
    }}while(0)

    streamList_[id] = decoders_[id]->info();
    if(!inputInfo_.hasData())
    {
        inputInfo_ = streamList_[id];
        EncodeParam ep;
        F_ASSERT(setEncodeParam(param_.output, param_.stitch,inputInfo_,c_inputCount_,ep)
                 ,"Check Encode parameter error!");
        outputInfo_.setData(ep.width,ep.height,ep.fps);

        if(c_encodeHW_)
            F_ASSERT(createEncHWCtx(ep.width,ep.height),"Encode Hardware failed created!");
        F_ASSERT(pStitcher_->setHardware(c_deviceNo_),"Stitcher set hardware error!");
        F_ASSERT(pStitcher_->setInput(c_inputCount_, inputInfo_.width, inputInfo_.height)
                 , "stitcher set input error!");
        F_ASSERT(pStitcher_->setOutput(ep.width,ep.height)
                 ,"stitcher set output error!");
        F_ASSERT(pStitcher_->open(),"stitcher open failed!");
        F_ASSERT(mainEncoder_->open(ep,pHWEncCtx_),"Main encoder open failed!");

        qDebug()<<"all modual start success!!!";
        mainEncoder_->run();
        pStitchTimer_->start();
    }
    if(!inputInfo_.equal(streamList_[id]))
    {
        QString str("Camera not matching-->[");
        for(StreamInfo& si : streamList_)
        {
            str += (QString("{width:%1,height:%2,fps:%3}")
                    .arg(si.width).arg(si.height).arg(si.fps));
        }
        qCritical()<<str+"]";
        this->onFatalError("Input device don't matching with each other.");
    }
}

void Panorama::onDecoderError(int id, QString errStr)
{
    Q_UNUSED(id);
    Q_UNUSED(errStr);
}

void Panorama::onStitch()
{
    for(int i = 0 ; i < c_inputCount_; i++)
    {
        auto pFrame = decoders_[i]->getFrame();
        if(pFrame)
            cameraFrames_[i].swap(pFrame);
    }
    FramePtr pPano = this->getPanoFrame();
    if(pPano && pStitcher_->stitch(cameraFrames_, pPano))
        mainEncoder_->send(pPano);
}

void Panorama::onFatalError(const QString &str)
{
    qCritical()<<str;
    qApp->quit();
}

void Panorama::onChangeMuxerUrl(QString outUrl)
{
    qCritical()<<"ChangeMuxerUrl--->["<<outUrl<<"]";
    if(mainEncoder_)
    {
        mainEncoder_->changeMuxerUrl(outUrl.toStdString());
    }
}

bool Panorama::libInit()
{
    avdevice_register_all();
    avformat_network_init();

    void* hHandleDriver = nullptr;
    CUresult ret = cuInit(0, __CUDA_API_VERSION, hHandleDriver);
    if(ret != CUDA_SUCCESS)
    {
        return false;
    }
    return true;
}

bool Panorama::createDevice(const char* deviceNo)
{
    auto ret = av_hwdevice_ctx_create(&pDevice_, AV_HWDEVICE_TYPE_CUDA, deviceNo, nullptr, 0);
    if(ret != 0)
    {
        qCritical()<<"av_hwdevice_ctx_create error:"<<ret;
        return false;
    }
    return true;
}

bool Panorama::createEncHWCtx(int width, int height)
{
    if(!pDevice_)
        return false;

    pHWEncCtx_ = createHWCtx(width, height, pDevice_);
    if(!pHWEncCtx_)
    {
        return false;
    }
    if(!fillhw(pHWEncCtx_, pPanoFrame_))
    {
        return false;
    }
    return true;
}

AVBufferRef *Panorama::createHWCtx(int width, int height, AVBufferRef *pd)
{
    AVBufferRef* pCtx = av_hwframe_ctx_alloc(pd);
    if(!pCtx)
        return nullptr;

    AVHWFramesContext* pfc = reinterpret_cast<AVHWFramesContext*>(pCtx->data);
    pfc->format = AV_PIX_FMT_CUDA;
    pfc->sw_format = AV_PIX_FMT_NV12;
    pfc->width = width;
    pfc->height = height;
    auto ret = av_hwframe_ctx_init(pCtx);
    if(ret != 0)
        av_buffer_unref(&pCtx);

    return pCtx;
}

FramePtr Panorama::getPanoFrame()
{
    if(!outputInfo_.hasData())
        return FramePtr(nullptr);

    FramePtr pPano(av_frame_alloc());

    if(pHWEncCtx_)
    {
        if(!fillhw(pHWEncCtx_,pPano))
        {
            qCritical()<<"PanoFrame alloc hardware resource error! it means no data sended after encode.";
            return FramePtr(nullptr);
        }
    }
    else
    {
        pPano->width = outputInfo_.width;
        pPano->height = outputInfo_.height;
        pPano->format = AV_PIX_FMT_NV12;
        int ret = av_frame_get_buffer(pPano.get(),CUDA_ALIGN);
        if(ret != 0)
        {
            qCritical()<<"av_frame_get_buffer:"<<AV_ERR(ret);
            return FramePtr(nullptr);
        }
    }
    return pPano;
}

bool Panorama::fillhw(AVBufferRef *pCtx, FramePtr pFrame)
{
    auto ret = av_hwframe_get_buffer(pCtx, pFrame.get(), 0);
    if(ret != 0)
    {
        qDebug()<<"err:"<<AV_ERR(ret);
        return false;
    }
    return true;
}

void Panorama::stdinThreadFunc()
{
    quint64 max_read = 1024;
    while(stdinRunning_)
    {
        QTextStream qtin(stdin);
        QString content = qtin.readLine(max_read);
        if(content.isEmpty())
            std::this_thread::sleep_for(std::chrono::seconds(1));
        else
            processStdin(content);
    }
}

void Panorama::processStdin(const QString &info)
{
    if(info.isEmpty())
        return ;

    std::string content = info.toStdString();
    try
    {
        cfg::cmd::CmdType type = cfg::cmd::getType(content);
        if(type == cfg::cmd::ChangeMulticast)
        {
            cfg::cmd::NewMuxerOut nm = cfg::cmd::toNewMuxerOut(content);
            QMetaObject::invokeMethod(this, "onChangeMulticast", Qt::QueuedConnection
                                      , Q_ARG(QString, QString::fromStdString(nm.muxerUrl)));
        }
    }catch(std::exception& e){
        qCritical()<<e.what();
    }
}




//    AVPixelFormat getPixel(AVHWDeviceType type)
//    {
//        AVPixelFormat pix = AV_PIX_FMT_NONE;
//        for(int i = 0; ; i++)
//        {
//            const AVCodecHWConfig* config = avcodec_get_hw_config(pc,i);
//            if(!config)
//                break;
//            //qDebug()<<(int)config->device_type; //4,0,7,2
//            if(config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX
//                    && config->device_type == type)
//            {
//                pix = config->pix_fmt;
//                break;
//            }
//        }
//        return pix;
//    }
