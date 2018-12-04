#include "device.h"
#include <QDebug>

Device::Device(const char *device)
    :device_(device)
    , pDevice_(nullptr)
    , pCtx_(nullptr)
{
}

void Device::close()
{
    pCtx_ = nullptr;
    av_buffer_unref(&pDevice_);
}

AVPixelFormat Device::pixFmt(AVCodec* pc)
{
    if(!pCtx_ || !pc)
        return AV_PIX_FMT_NONE;

    AVPixelFormat pix = AV_PIX_FMT_NONE;
    for(int i = 0; ; i++)
    {
        const AVCodecHWConfig* config = avcodec_get_hw_config(pc,i);
        if(!config)
            break;
        if(config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX
                && config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_FRAMES_CTX
                && config->device_type == pCtx_->type)
        {
            pix = config->pix_fmt;
            break;
        }
    }
    return pix;
}

Device::operator AVBufferRef *()
{
    return pDevice_;
}
