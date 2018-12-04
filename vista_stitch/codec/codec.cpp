#include "codec.h"
#include <mutex>
#include <stdio.h>

StreamBase::StreamBase()
    : StreamBase(0,0,0)
{

}

StreamBase::StreamBase(int width, int height, int fps)
    : width(width)
    , height(height)
    , fps(fps)
{

}

bool StreamBase::hasData()
{
    return (width > 0 && height > 0 && fps > 0);
}

bool StreamBase::hasData() const
{
    return (width > 0 && height > 0 && fps > 0);
}

void StreamBase::setData(int width, int height, int fps)
{
    this->width = width;
    this->height = height;
    this->fps = fps;
}

bool StreamBase::equal(const StreamBase _other)
{
    return (this->width == _other.width
            && this->height == _other.height
            && this->fps == _other.fps);
}


StreamInfo::StreamInfo()
    :StreamBase()
{

}

StreamInfo::StreamInfo(int width, int height, int fps, int nearFps, int avgFps)
    : StreamBase(width, height, fps)
    , nearFps(nearFps)
    , avgFps(avgFps)
{

}


FramePtr::FramePtr(AVFrame *p)
    : std::shared_ptr<AVFrame>(p, [](AVFrame* p)
    {
        if(p)
            av_frame_free(&p);

    })
{
}

PacketPtr::PacketPtr(AVPacket *p)
    : std::shared_ptr<AVPacket>(p,[](AVPacket* p)
    {
        if(p)
            av_packet_free(&p);
    })
{
}

std::string avErrStr(int code)
{
    static std::mutex mtx;
    static char str[256] = {0};

    std::lock_guard<std::mutex> lg(mtx);
    (void)lg;

    memset(str,0,sizeof(str));
    av_strerror(code,str,sizeof(str));
    return std::string(str);
}
