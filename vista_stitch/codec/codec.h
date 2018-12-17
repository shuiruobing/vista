#ifndef CODEC_H
#define CODEC_H

#include <memory>
#include <string>
#include <map>

extern "C"
{
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libavutil/hwcontext_cuda.h"
#include "libswscale/swscale.h"
#include <cuda.h>
}

struct StreamBase
{
    StreamBase();

    StreamBase(int width, int height, int fps);

    bool hasData();

    bool hasData() const;

    void setData(int width, int height, int fps);

    bool equal(const StreamBase _other);

    int width;
    int height;
    int fps;
};

struct StreamInfo : public StreamBase
{
    StreamInfo();
    StreamInfo(int width, int height, int fps, int nearFps, int avgFps);
    int nearFps{0};
    int avgFps{0};
};


class FramePtr : public std::shared_ptr<AVFrame>
{
public:
    explicit FramePtr(AVFrame* p);
};

class PacketPtr : public std::shared_ptr<AVPacket>
{
public:
    explicit PacketPtr(AVPacket* p);
};

std::string avErrStr(int code);
#define AV_ERR(code) avErrStr(code).c_str()

//codec parameter for ffmpeg options
using Options = std::map<std::string, std::string>;
using OptionItem = std::pair<const std::string, std::string>;


#endif // CODEC_H
