#include "configure.h"
#include <QFile>
#include <QDebug>
#include "json.hpp"


namespace cfg {
    using nlohmann::json;

    void from_json(const json& j, DeviceNode& dn)
    {
        j.at("CameraChannelID").get_to(dn.cameraID);
        j.at("RtspUrl").get_to(dn.url);
    }

    void from_json(const json& j, StitchNode& sn)
    {
        j.at("Mode").get_to(sn.mode);
    }

    void from_json(const json& j, EncodeNode& en)
    {
        j.at("CodecId").get_to(en.codecId);
        j.at("Hardware").get_to(en.hardware);
        j.at("QP").get_to(en.qp);
        j.at("Bitrate").get_to(en.bitrate);
        j.at("GOP").get_to(en.gop);
        j.at("Profile").get_to(en.profile);
    }

    void from_json(const json& j, InputNode& in)
    {
        j.at("MaxGopCached").get_to(in.maxGopCached);
        j.at("MaxFrameCached").get_to(in.maxFrameCached);
        j.at("Devices").get_to(in.devices);
        j.at("Hardware").get_to(in.hardware);
    }

    void from_json(const json& j, OutputNode& on)
    {
        j.at("Multicast").get_to(on.multicast);
        j.at("Unicast").get_to(on.unicast);
        j.at("Encode").get_to(on.encode);
    }

    void from_json(const json& j, PanoNode& pn)
    {
        j.at("Hardware").get_to(pn.hardware);
        j.at("HardwareNo").get_to(pn.hardwareNo);
        j.at("Input").get_to(pn.input);
        j.at("Output").get_to(pn.output);
        j.at("Stitch").get_to(pn.stitch);
    }

    std::string readFromFile(const QString& filePath)
    {
        QFile file(filePath);
        if(!file.exists())
            return "";

        if(!file.open(QFile::ReadOnly))
            return "";

        QByteArray data = file.readAll();
        file.close();
        return data.toStdString();
    }

    bool parseConfigure(const std::string& jsonData, PanoNode &pn)
    {
        try
        {
            json j = json::parse(jsonData);
            pn = j;
        }catch(std::exception& e){
            qWarning()<<"parse json:"<<e.what();
            return false;
        }
        return true;
    }

    std::string genExpample()
    {
        json j = {
            {"Hardware", "Nvidia"},
            {"HardwareNo", "0"},
            {"Stitch",{
                {"Mode","horizontal"}
            }},
            {"Input",{
                 {"MaxGopCached",3},
                 {"MaxFrameCached", 20},
                 {"Hardware", true},
                 {"Devices",json::array({
                      {{"Name", "Camera1"},{"Url", "rtsp://host:port/path"}}
                      ,{{"Name", "Camera2"},{"Url", "rtsp://host:port/path"}}
                 })}
            }},
            {"Output",{
                 {"Multicast", "udp://233.233.233.233:55555"},
                 {"Unicast", "udp://127.0.0.1:55556"},
                 {"Encode",{
                      {"CodecId", "H264"},
                      {"hardware", true},
                      {"QP", 35},
                      {"Bitrate", -1},
                      {"GOP", -1},
                      {"Profile", "main"}
                  }}
            }}
        };
        return j.dump(1,'\t');
    }

    void saveConfigureToFile(const std::string &jsonData, const QString &filepath)
    {
        QFile file(filepath);
        if(!file.open(QFile::WriteOnly))
            return;

        file.write(QByteArray::fromStdString(jsonData));
        file.close();
    }

    cmd::CmdType cmd::getType(const std::string &jsonData)
    {
          json j = json::parse(jsonData);
          std::string cmdStr;
          j.at("Command").get_to(cmdStr);
          if(cmdStr.find("ChangeMulticast") != std::string::npos)
              return cmd::ChangeMulticast;
          return cmd::Unkown;
    }

    cmd::NewMuxerOut cmd::toNewMuxerOut(const std::string &jsonData)
    {
        cmd::NewMuxerOut ccm;
        json j = json::parse(jsonData);
        j.at("MuxerUrl").get_to(ccm.muxerUrl);
        return ccm;
    }

}

