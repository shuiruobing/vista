var WebServer = {
    "ip": "http://127.0.0.1:5000/"
}
var AllTasksInfo = WebServer.ip + "api/v1/task/GetAll";
var SaveTaskInfoUrl = WebServer.ip + "api/v1/task/TaskSave";
var TaskStartUrl = WebServer.ip + "api/v1/task/Start";
var TaskStopUrl = WebServer.ip + "api/v1/task/Stop";
var TaskRemoveUrl = WebServer.ip + "api/v1/task/Remove";
var WebVersionUrl = WebServer.ip + "api/v1/web/GetVersion";
var MachineCodeUrl = WebServer.ip + "api/v1/authorization/GetMachineCode";
var RegisterStatusUrl = WebServer.ip + "api/v1/authorization/RegistrerStatus";
var RegisteUrl = WebServer.ip + "api/v1/authorization/Registration";
var ComputerUrl = WebServer.ip + "api/v1/computer/Computer";
var TaskParamChinaName = {
    "ChannelID": "通道号",
    "ChannelName": "通道名称",
    "Multicast": "组播地址",
    "StitchParam": {
        "StitchParam": "拼接参数",
        "Hardware": "显卡",
        "HardwareNo": "显卡ID",
        "Input": {
            "Input": "输入",
            "Devices": {
                "Devices": "设备",
                "CameraChannelID": "相机ID",
                "RtspUrl": "Rtsp地址"
            },
            "Hardware": "硬件加速",
            "MaxFrameCached": "最大Frame缓存",
            "MaxGopCached": "最大GOP缓存"
        },
        "Output": {
            "Output": "输出",
            "Encode": {
                "Encode": "编码",
                "Bitrate": "码率",
                "CodecId": "编码类型",
                "GOP": "GOP",
                "Profile": "Profile",
                "QP": "QP",
                "Hardware": "硬件加速"
            },
            "Multicast": "组播地址"
        },
        "Stitch": {
            "Stitch": "拼接",
            "Mode": "状态"
        }
    },
    "StitchPID": "拼接进程ID",
    "StitchPort": "拼接端口号",
    "RtspPID": "Rtsp进程ID",
    "RtspPort": "Rtsp端口号",
    "Status": "任务状态",
    "RtspParamOut": "Rtsp输出地址",
    "RtspParamIn": "Rtsp输入参数",
    "RtspWorkDir": "Rtsp工作路径",
    "StitchWorkDir": "拼接工作路径"
};