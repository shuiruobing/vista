#ifndef DEVICE_H
#define DEVICE_H

#include <QDebug>
extern "C"
{
#include "libavdevice/avdevice.h"
}

template<AVHWDeviceType type> class DeviceTrait;

template<> class DeviceTrait<AV_HWDEVICE_TYPE_NONE>
{
    using PF = AVPixelFormat;
    static PF const PixFmt = AV_PIX_FMT_NV12;
    static PF const SWPixFmt = AV_PIX_FMT_NV12;
};

template<> class DeviceTrait<AV_HWDEVICE_TYPE_CUDA>
{
public:
    using PF = AVPixelFormat;
    static PF const PixFmt = AV_PIX_FMT_CUDA;
    static PF const SWPixFmt = AV_PIX_FMT_NV12;
};

template<AVHWDeviceType type>
bool checkUseful(AVCodec* pc)
{
    if(!pc)
        return false;

    for(int i = 0; ; i++)
    {
        const AVCodecHWConfig* config = avcodec_get_hw_config(pc,i);
        if(!config)
            break;
        if(config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX
                && config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_FRAMES_CTX
                && config->device_type == pCtx_type)
        {
            if(config->pix_fmt == DeviceTrait<type>::PixFmt)
                return true;
        }
    }
    return false;
}


class Device
{
public:
    Device(const char* device);

    template<AVHWDeviceType type>
    bool open();

    void close();

    operator AVBufferRef*();

    AVPixelFormat pixFmt(AVCodec *pc);

private:
    const char* device_;
    AVBufferRef* pDevice_;
    AVHWDeviceContext* pCtx_;
};

template<AVHWDeviceType type>
bool Device::open()
{
    auto ret = av_hwdevice_ctx_create(&pDevice_, type, device_, nullptr, 0);
    if(ret != 0)
    {
        qCritical()<<"av_hwdevice_ctx_create error:"<<ret;
        return false;
    }
    pCtx_ = reinterpret_cast<AVHWDeviceContext*>(pDevice_->data);

    return true;
}

#endif // DEVICE_H
