#ifndef CONFIGURE_H
#define CONFIGURE_H


/// 这个文件用于解析和校验配置的内容
/// 使用:
/// cfg::Configure c;
/// c.parseAndCheck();
/// string use "utf-8"


#include <string>
#include <vector>
#include <QString>

namespace cfg {

struct DeviceNode
{
    std::string cameraID;
    std::string url;
};

struct StitchNode
{
    std::string mode;   //vtyp:horizontal/vertical
};

struct EncodeNode
{
    std::string codecId; //"H264/H265"
    bool hardware;
    int qp;
    int bitrate;
    int gop;
    std::string profile;
};

struct InputNode
{
    int maxGopCached;
    int maxFrameCached;
    bool hardware;
    std::vector<DeviceNode> devices;
};

struct OutputNode
{
    std::string multicast;
    std::string unicast;
    EncodeNode encode;
};

struct PanoNode
{
    std::string hardware;//"Nvidia"
    std::string hardwareNo;
    StitchNode stitch;
    InputNode input;
    OutputNode output;
    std::vector<OutputNode> outChildren;
};

std::string readFromFile(const QString &filePath);

bool parseConfigure(const std::string &jsonData, PanoNode& pn);

std::string genExpample();

void saveConfigureToFile(const std::string& jsonData, const QString& filepath);
}

namespace cfg{
namespace cmd {
enum CmdType
{
    Unkown = -1,
    ChangeMulticast = 0,
};

struct NewMuxerOut
{
    std::string muxerUrl;
};

struct NewNaluOut
{
    std::string naluUrl;
};

CmdType getType(const std::string& jsonData);

NewMuxerOut toNewMuxerOut(const std::string& jsonData);
}
}

#endif // CONFIGURE_H
