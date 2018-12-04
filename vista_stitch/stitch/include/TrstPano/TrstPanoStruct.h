//
#pragma once

#define MAXCAMERAS		16	//最大像机数量
#define MAXCAMERAGROUP	9	//最大8组并行拼接,第0个保留
#define MAXPANOWIDTH	9600	//全景视频最大宽度
#define MAXPANOHEIGHT	4096	//全景视频最大高度
#define MAXNAMELENGTH	128		//命名长度
#define MAXPATHLENGTH	512		//路径长度
#define MAXIDLENGTH		64		//ID长度
#define MAXURLLENGTH	128		//url长度
#define MAXOSDCHANNELNAME	8	//通道数量
//融合方式
typedef enum _BlenderMode
{	
	nBlenderNo			= 0,
	nBlenderFeather		= 1,
	nBlenderMultiband	= 2
}BlenderMode;

//加速方式
typedef enum _TBBMode
{	
	NON_USE_TBBMode		= 0,
	USE_TBBMode			= 1
}TBBMode;

//是否使用全画幅
typedef enum _RectMode
{	
	SHOW_FULLPANO		= 0,
	SHOW_FULLHEIGHT		= 1,
	SHOW_FULLWIDTH		= 2
}RectMode;

//是否输出时间测试结果
typedef enum _TimeMode
{	
	SHOW_TIMETEST		= 0,
	NON_TIMETEST		= 1
}TimeMode;

//二维点结构
typedef struct _Trpano_Point
{
	double x;
	double y;
}Trpano_Point;
//二维点序列
typedef struct _Trpano_CaliPoints
{
	int nPointGroup;
	int nPointNums[2];
	int nPanoWidth;
	int nPanoHeight;
	float nRoiScaleRatio;
	Trpano_Point TPoint[2][128];
}Trpano_CaliPoints;
//色彩校正
typedef struct _Trpano_ColorCorrect{
	int nEnable;		//色彩校正开关
	int nEdgeNums;		//边界点数量
	Trpano_Point EdgePoint[2][MAXCAMERAS];
}Trpano_ColorCorrect;

//OSD-Position
typedef struct _OSDPos
{
	int nOffsetX;//水平偏移
	int nOffsetY;//垂直偏移
	int nLeftTopMode;//顶左模式
	int nFontSize;	//字体大小
	int nColorMode;	//颜色模式
	float fDiaphaneity;//透明度
	int nInterval;		//显示间隔
	int nEndX;//水平偏移
	int nEndY;//垂直偏移
}OSDPos;
//OSD-timestamp
typedef struct _STRTimeStamp
{
	int nEnable;	//开关
	OSDPos _POS;	//显示位置
	int nDayMode;	//日期模式
	int nHourMode;	//时间模式
	int nWeekMode;	//星期模式
}STRTimeStamp;
//OSD-channel name
typedef struct _STRChannelName
{
	int nEnable;	//开关
	OSDPos _POS;	//显示位置
	char strName[MAXNAMELENGTH];	//通道名称
}STRChannelName;
//OSD
typedef struct _STROSDPano
{
	STRTimeStamp TimeStamp;
	STRChannelName ChannelName[MAXOSDCHANNELNAME];
	int OSD_Flashing;
	int Channel_Num;
}STROSDPano;

//编码参数节点
typedef struct _tagEncodePara{
	int enable;
	int frame_rate;
	int code_rate;
	int width;
	int height;
}EncodePara;


//定义拼接参数结构体
typedef struct _PanoParam
{
	int nStitchProcessID;			//拼接器id，仅用于标识不同的拼接组
	int nFrameWidth;				//图像宽度
	int nFrameHeight;				//图像高度
	int nFramePitch;
	int nPanoWidth;					//全景图像宽度
	int nPanoHeight;				//全景图像高度
	int nPanoPitch;
	int nCameraNum;					//图像数量
	int nInputFrameType;			//输入图像数据格式
	int nOutFrameType;				//输出图像数据格式
	float fInterScale;				//内部缩放比例
	float fOutScale;				//输出图像缩放比例
	bool bPreview;					//预览
	int nMaskMode;					//掩码模式
	char strMaskPre[MAXIDLENGTH];			//掩码文件名前缀
	char strCameraID[MAXIDLENGTH];			//虚拟像机名称
	char strCameraName[MAXNAMELENGTH];		//虚拟像机名称
	char strMasks[MAXCAMERAS][MAXNAMELENGTH];	//mask文件名，序号从低到高对应拼接序列从左到右
	int nArrVectorIndex[MAXCAMERAS];//像机排序
	float fSharpness;				//羽化参数
	int nBlenderMode;				//融合模式
	int nUseTBBMode;				//TBB加速模式
	int nRectMode;					//边缘裁剪模式
	int nTimeMode;					//处理耗时显示模式
	int nUVMode;					//UV模式
	double dHArray[MAXCAMERAS][9];	//拼接矩阵
	bool nEnableCalib;				//视频校正
	Trpano_CaliPoints CaliPoint;	//校正点序列
	Trpano_ColorCorrect ColorCorrect;//色彩校正
	STROSDPano OSDInfo;				//OSD参数
	EncodePara OutMainstream;
	EncodePara OutSubstream;
}PanoParam;


struct Adjust
{
	float x;
	float y;
	float rx;
	float ry;
};

struct StitchParamter
{
	int initFlag;
	float featureScale;
	int seamType;
	int projectType;
	int dehazerEnable;
	int dehazerRadius;
	float dehazerRizeratio;
	int distort;
	int ajustNumber;
	Adjust adjustArray[10];
};



//物理像机节点
typedef struct _tagPhyCamera{
	int cameraid;
	int width;
	int height;
	int port;
	int uv_mode;
	char user_name[32];
	char user_pwd[32];
	char ip[16];
	char url[MAXURLLENGTH];
}PhyCamera;



//网络端口节点
typedef struct _tagNetPort{
	int main_port;
	int rtsp_port;
	int rtp_port;
	int sip_port;
	int onvif_port;
	int gb28181_port;
	int netprocess_port;
	int reserved1;
	int reserved2;
}NetPort;

//视频平台节点
typedef struct _tagVideoPlatform{
	int server_type;
	int server_port;
	char ip[16];
	char user_name[32];
	char user_pwd[32];
	char server_url[MAXURLLENGTH];
}VideoPlatform;

typedef struct _tagVrtCamera{
	char strCameraID[MAXIDLENGTH];			//虚拟像机名称
	char strCameraName[MAXNAMELENGTH];		//虚拟像机名称
	int bEnable;					//启用
	int nPanoWidth;					//全景图像宽度
	int nPanoHeight;				//全景图像高度
	int nCameraNum;					//图像数量
}VrtCamera;	//虚拟像机属性

//图像数据头
typedef struct _PanoDataHeader
{
	int nStitchProcessID;
	int nPanoWidth;
	int nPanoHeight;
	int nCodeType;
}PanoDataHeader;


typedef enum _TrstErrorCode
{
	// sip错误码，保留
	//01) 100 Trying   说明caller正在呼叫，但还没联系上callee。 
	//180 Ringing 说明callee已经被联系上,callee的铃正在响.收到这个信息后，等待200 OK  
	//02) 181 Call is being forwarded  说明call被重新路由到另外一个目的地  
	//03) 182 Queued   说明callee当前是不可获得的，但是对方不想直接拒绝呼叫，而是选择放在呼叫队列中  
	//04) 183 Session progress   用来警告caller频段(inband)错误。当从PSTN收到一个ISDN消息，SIP gateway 产生183 Session progress 。  

	//2xx successful Responses   
	//200 OK   

	//3xx Redirection Responses   
	//05) 300 Multiple choices  说明呼叫的地址被解析成多个地址，所有的地址都被提供出来，用户或用户代理可以从                           中选择联系哪个。   
	//06) 301 Moved permanently   说明指定地址的用户已经永远不可用，在头中已经用另外一个地址替换了.  
	//07) 302 Moved temporarily   说明指定地址的用户临时不可用，在头中已经用另外一个地址代替了.  
	//08) 305 Use proxy   说明caller必须用一个proxy来联系callee. 
	//09) 380 Alternative service   说明call不成功，但是可选择其他的服务   

	//4xx Request Failure Responses    
	//10) 400 Bad Request   说明由于非法格式，请求不能被理解。  
	//11) 401 Unauthorized 说明请求需要用户认证。  
	//12) 402 Payment required  说明完成会话需要付费.  
	//13) 403 Forbidden   说明server已经收到并能理解请求但不提供服务。  
	//14) 404 Not Found   说明server有明确的信息在指定的域中用户不存在.  
	//15) 405 Method Not Allowed   说明请求中指定的方法是不被允许的。将返回一个允许的方法列表。  
	//16) 406 Not Acceptable 说明被请求的资源只对某些特殊的请求作出响应，对目前头(header)中指定的请求不接受.  
	//17) 407 Proxy authentication required   和401 Unauthorized response相似.但是，它说明client必须首先在proxy上认证自己。  
	//18) 408 Request timeout   说明在timeout时间过期前，server不能产生响应.  
	//19) 409 Conflict   说明由于和当前资源状态产生冲突，请求不能被处理。  
	//20) 410 Gone   说明请求资源在server上永远不可用，也没有转发的地址。  
	//21) 411 Length required  说明用户拒绝接受没有定义content长度的请求。  
	//22) 413 Request entity too large   说明server拒绝处理请求，因为它太大，超过了server能处理的大小。  
	//23) 414 Request-URI too long   说明server拒绝处理请求，因为请求的URI太长，server不能解释它。  
	//24) 415 Unsupported media   说明server拒绝处理请求，因为body格式不被目的终端支持  
	//25) 420 Bad extension   说明server不能理解在header中指出的扩展协议。  
	//26) 480 Temporarily unavailable   说明callee已经被联系上，但是暂时不可用。  
	//27) 481 Call leg/transaction does not exist  说明server正在忽略请求，由于它是一个没有匹配legID的BYE或     者是一个没有匹配事务的CANCEL。   
	//28) 482 Loop detected   说明server收到了一个包含它自己路径的请求.  
	//29) 483 Too many hops   说明server收到了一个请求，它需要的hop数超过了在header中允许的最大hop数.  
	//30) 484 Address incomplete   说明server收到一个地址不完整的请求. 
	//31) 485 Ambiguous   说明server收到一个请求，其中callee的地址是不明确的，也没有可能备用的地址供选择。  
	//32) 486 Busy here   说明callee已经被联系上，但是它们的系统不能承受额外的call.   
	//488  （暂时不能进行）。  

	//5xx Server Failure Responses   
	//33) 500 Server internal error   说明server或gateway发生意外错误从而不能处理请求.  
	//34) 501 Not implemented   说明server或gateway不支持完成请求所需的功能.  
	//35) 502 Bad gateway   说明server或gateway从下游server收到一个非法响应.
	//36) 503 Service unavailable   说明由于超负载或维护问题，server或gateway不能处理请求.  
	//37) 504 Gateway timeout  说明server或gateway没有从另外一个server(如location server)收到一个及时的响应.     
	//38) 505 Version not supported   说明server或gateway不支持在请求中用到的SIP版本。   

	//6xx Global Responses   
	//39) 600 Busy everywhere  说明callee已经被联系上，但是处于忙状态中，在这个时间不能接受call。  
	//40) 603 Decline   说明callee已经被联系上，但是不能或不想加入call。  
	//41) 604 Does not exist anywhere   说明server有正式的信息说明callee不存在于网络中。  
	//42) 606 Not acceptable   说明callee已经被联系上，但是session描述的某些方面不被接受。

	//=====================通用类错误====================================
	//六大类SIP协议栈状态错误码
	TR_OK	= 10000,		//正常
	TR_ERROR = 10001,		//错误
	TR_ERROR_LOGIN = 10002,		//登录失败
	TR_ERROR_LICENCE =10003,	//授权错误或到期
	TR_ERROR_CAPBILITY_LIMITED = 1004,	//设备能力限制
	TR_ERROR_STREAM_IN = 10005,		//接入视频失败
	TR_ERROR_WAITE_FOR_TIME_OUT= 10006,	//等待超时
	TR_ERROR_FILE_NOT_EXIST = 10007,	//配置文件不存在
	TR_ERROR_STITCHING_PARAM = 10008,	//拼接参数错误
	//仅以上状态/错误码允许在进程间传递，由上村进程根据相应的错误处理机制执行异常处理
	//以下状态/错误码仅允许在进程内部使用

	//=====================系统类错误====================================
	TR_ERROR_LOGIN_SUCCEED	= 20000,	//系统登成功
	TR_ERROR_LOGIN_FAILED	= 20001,	//系统登类错误,登录失败


	TR_ERROR_LOGIN_ACC_ISNULL = 21001,	//登录用户名为空
	TR_ERROR_LOGIN_PSW_ISNULL = 21002,	//登录密码为空
	TR_ERROR_LOGIN_ACC = 21003,	//登录用户名错误
	TR_ERROR_LOGIN_PSW = 21004,	//登录密码错误
	TR_ERROR_LOGIN_ACCESS_DENY = 21005,	//拒绝访问
	TR_ERROR_LOGIN_VERIFY_FAILED = 21006,	//账户验证失败
	//...				//待补充

	TR_ERROR_MEM_SUCCEED = 22000,	//内存操作成功
	TR_ERROR_MEM_FAILED	= 22001,	//内存操作类错误,未知错误

	TR_ERROR_MEM_ALLOC = 22003,	//申请内存失败
	TR_ERROR_MEM_RELEASE = 22004,	//释放内存失败
	TR_ERROR_MEM_LENGTH = 22005,	//内存长度不足
	TR_ERROR_MEM_NULL = 22006,	//内存指针为空

	TR_LICENCE_SUCCEED = 23000,		//授权成功
	TR_LICENCE_ERROR = 23001,		//授权失败
	TR_LICENCE_LIMITED = 23002,		//授权限制：设备能力限制
	TR_LICENCE_TIMEOUT = 23003,		//授权过期

	//...				//待补充

	//=====================网络类错误======================================
	TR_ERROR_NETWORK_SUCCEED = 30000,	//网络操作成功
	TR_ERROR_NETWORK_FAILED	= 30001,	//网络操作失败

	//=====================拼接操作错误====================================
	TR_ERROR_STITCHING_SUCCEED	= 40000,	//拼接操作类成功
	TR_ERROR_STITCHING_FAILED	= 40001,	//拼接操作类错误,未知错误

	TR_ERROR_STITCHING_FORMAT	= 41001,	//图像格式不支持
	TR_ERROR_STITCHING_IMAGEDATA	= 41002,	//图像内容无效
	TR_ERROR_STITCHING_IMAGESIZE	= 41003,	//图像分辨率无效
	TR_ERROR_STITCHING_NULL	= 41004,		//拼接器无效
	TR_ERROR_STITCHING_NULLPOINT	= 41005,	//数据指针为空
	TR_ERROR_STITCHING_OVERFLOW	=	41006,		//拼接器对象溢出
	TR_ERROR_STITCHING_IMAGENUM = 41007,

	TR_ERROR_STITCHING_DEVICE_NULL = 42000,//设备无效
	TR_ERROR_STITCHING_DEVICE_DRIVE = 42001,//设备驱动无效
	TR_ERROR_STITCHING_DEVICE_NPP = 42002, //设备不支持npp
	TR_ERROR_STITCHING_DEVICE_TBB = 42003,//设备不支持tbb
	TR_ERROR_STITCHING_DEVICE_BIND = 42004,//设备绑定失败

	TR_ERROR_STITCHING_CUDA = 43000,//cuda操作失败
	TR_ERROR_STITCHING_CUDA_UPLOAD = 43001,//cuda上载数据失败
	TR_ERROR_STITCHING_CUDA_DOWNLOAD = 43002,//cuda下载数据失败
	TR_ERROR_STITCHING_CUDA_ALLOC = 43003,//cuda内存分配失败

	TR_ERROR_STITCHING_CFG = 44000,//拼接参数错误
	TR_ERROR_STITCHING_CFG_NULL = 44001,//拼接参数为空
	TR_ERROR_STITCHING_CFG_READ = 44002,//读取拼接参数失败
	TR_ERROR_STITCHING_CFG_SAVE = 44003,//保存拼接参数失败
	TR_ERROR_STITCHING_CFG_NUMBERS = 44004,//视频数量不匹配
	TR_ERROR_STITCHING_CFG_MASKS = 44005,//拼接掩码无效
	TR_ERROR_STITCHING_CFG_RECT = 44006,//拼接区域无效

	TR_ERROR_STITCHING_VERIFY_SUCCEED = 45000,//拼接模块授权成功
	TR_ERROR_STITCHING_VERIFY_FAILED = 45001,//拼接模块授权失败

	TR_ERROR_STITCHING_PREPARE= 46000,//预拼接失败
	TR_ERROR_STITCHING_RECTMODE= 46001,//裁剪区域错误
	TR_ERROR_STITCHING_BLENDMODE= 46002,//融合模式错误
	TR_ERROR_STITCHING_TBBMODE= 46003,//tbb加速模式错误
	TR_ERROR_STITCHING_WAITING_FOR_DATA= 46004,//等待数据

	TR_ERROR_STITCHING_PANO_NULL= 47000,//无拼接数据


	//...				//待补充


	//=====================图像类错误====================================
	TR_ERROR_IMAGE_SUCCEED	= 50000,	//图像操作类成功
	TR_ERROR_IMAGE_FAILED	= 50001,	//图像操作类错误,未知错误
	TR_ERROR_IMAGE_WIDTH = 51001,	//图像宽度无效
	TR_ERROR_IMAGE_HEIGHT = 51002, //图像高度无效
	TR_ERROR_IMAGE_SIZE = 51003, 	//图像分辨率无效
	TR_ERROR_IMAGE_NULL = 51004,	//图像为空
	//...				//待补充

	//=====================视频类错误====================================
	TR_ERROR_STREAM_SUCCEED	= 60000,	//视频操作类成功
	TR_ERROR_STREAM_FAILED	= 60001,	//视频操作类错误,未知错误

	TR_ERROR_STREAM_WIDTH = 61001,	//视频宽度无效
	TR_ERROR_STREAM_HEIGHT = 61002, //视频高度无效
	TR_ERROR_STREAM_SIZE = 61003, 	//视频分辨率无效
	TR_ERROR_STREAM_NULL = 61004,	//视频为空

	TR_ERROR_STREAM_REQEST = 62000,	//视频请求失败
	TR_ERROR_STREAM_PLAY = 62001,	//视频播放失败	
	TR_ERROR_STREAM_STOP = 62002,	//视频停止失败
	TR_ERROR_STREAM_PLAYFORWARD = 62003,	//视频快进失败
	TR_ERROR_STREAM_PLAYBACKWARD = 62004,	//视频快退失败
	TR_ERROR_STREAM_PLAYPAUSE = 62004,	//视频暂停失败
	//...				//待补充


	//=====================编解码类错误====================================
	TR_ERROR_CODEC_SUCCEED = 70000,			//编解码操作类成功
	TR_ERROR_CODEC_FAILED = 70001,			//编解码操作类错误,未知错误

	TR_ERROR_CODEC_ENCODING_INIT = 71001,	//编码器初始化失败
	TR_ERROR_CODEC_ENCODING_SIZE = 71002,	//编码器分辨率不匹配
	TR_ERROR_CODEC_ENCODING_FORMAT = 71003,	//编码器编码格式不支持

	TR_ERROR_CODEC_DECODING_INIT = 72001,	//解码器初始化失败
	TR_ERROR_CODEC_DECODING_SIZE = 72002, 	//解码器分辨率不匹配
	TR_ERROR_CODEC_DECODING_FORMAT = 72003, //解码器解码格式不支持
	//...				//待补充

	//=====================OSG类错误=======================================
	TR_ERROR_OSG_SUCCEED = 80000,			// OSG正常
	TR_ERROR_OSG_FAILED = 80001,			// OSG错误

	TR_ERROR_OSG_VIEWHWND = 81001,			// 窗口句柄为空
	TR_ERROR_OSG_ISINIT = 81002	,			// 再次初始化
	TR_ERROR_OSG_INITFILEPATH = 81003,		// 初始化文件路径为空
	TR_ERROR_OSG_LOADFILE = 81004,			// XML载入错误
	TR_ERROR_OSG_XMLERROR = 81005,			// XML读取错误
	TR_ERROR_OSG_XMLROOT = 81006,			// XML根节点为空
	TR_ERROR_OSG_XMLELEMENT = 81007,			// XML节点错误
	TR_ERROR_OSG_INITSCENE = 81008,			// 初始化OSG场景失败 
	TR_ERROR_OSG_CAMMODEL = 81009,			// 初始化虚拟相机模型失败
	TR_ERROR_OSG_CAMMARK = 81010,			// 创建相机标志失败

	TR_ERROR_OSG_PARAMETER_CAMID = 82001,	// 参数相机ID为空
	TR_ERROR_OSG_PARAMETER_VIEWPORT = 82002,	// 视口参数错误 全为0

	TR_ERROR_OSG_TOUR_ISADD = 83001,			// 该ID路径已填加
	TR_ERROR_OSG_TOUR_NEWERROR = 83002,		// 创建路径对象出错
	TR_ERROR_OSG_TOUR_LOADFILE = 83003,		// 路径文件载入错误
	TR_ERROR_OSG_TOUR_FILEERROR = 83004,		// 路径文件读取错误
	TR_ERROR_OSG_TOUR_FILEROOT = 83005,		// 路径根节点为空
	TR_ERROR_OSG_TOUR_OSGERROR = 83006,		// 开始漫游获取OSG失败
	TR_ERROR_OSG_TOUR_PATHMAN = 83007,		// 开始漫游manager出错
	TR_ERROR_OSG_TOUR_SETOSG = 83008,		// 开始漫游设置OSG出错
	TR_ERROR_OSG_TOUR_SETHWND = 83009,		// 开始漫游设置窗口句柄出错
	TR_ERROR_OSG_TOUR_ANIMATION = 83010,		// 根据路径ID开始漫游出错
	TR_ERROR_OSG_TOUR_MANERROR = 83011		// 开始漫游操作器出错



	//=====================XX类错误====================================	
	//...				//待补充

}TrstErrorCode;
