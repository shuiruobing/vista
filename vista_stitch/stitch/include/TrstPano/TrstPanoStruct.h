//
#pragma once

#define MAXCAMERAS		16	//����������
#define MAXCAMERAGROUP	9	//���8�鲢��ƴ��,��0������
#define MAXPANOWIDTH	9600	//ȫ����Ƶ�����
#define MAXPANOHEIGHT	4096	//ȫ����Ƶ���߶�
#define MAXNAMELENGTH	128		//��������
#define MAXPATHLENGTH	512		//·������
#define MAXIDLENGTH		64		//ID����
#define MAXURLLENGTH	128		//url����
#define MAXOSDCHANNELNAME	8	//ͨ������
//�ںϷ�ʽ
typedef enum _BlenderMode
{	
	nBlenderNo			= 0,
	nBlenderFeather		= 1,
	nBlenderMultiband	= 2
}BlenderMode;

//���ٷ�ʽ
typedef enum _TBBMode
{	
	NON_USE_TBBMode		= 0,
	USE_TBBMode			= 1
}TBBMode;

//�Ƿ�ʹ��ȫ����
typedef enum _RectMode
{	
	SHOW_FULLPANO		= 0,
	SHOW_FULLHEIGHT		= 1,
	SHOW_FULLWIDTH		= 2
}RectMode;

//�Ƿ����ʱ����Խ��
typedef enum _TimeMode
{	
	SHOW_TIMETEST		= 0,
	NON_TIMETEST		= 1
}TimeMode;

//��ά��ṹ
typedef struct _Trpano_Point
{
	double x;
	double y;
}Trpano_Point;
//��ά������
typedef struct _Trpano_CaliPoints
{
	int nPointGroup;
	int nPointNums[2];
	int nPanoWidth;
	int nPanoHeight;
	float nRoiScaleRatio;
	Trpano_Point TPoint[2][128];
}Trpano_CaliPoints;
//ɫ��У��
typedef struct _Trpano_ColorCorrect{
	int nEnable;		//ɫ��У������
	int nEdgeNums;		//�߽������
	Trpano_Point EdgePoint[2][MAXCAMERAS];
}Trpano_ColorCorrect;

//OSD-Position
typedef struct _OSDPos
{
	int nOffsetX;//ˮƽƫ��
	int nOffsetY;//��ֱƫ��
	int nLeftTopMode;//����ģʽ
	int nFontSize;	//�����С
	int nColorMode;	//��ɫģʽ
	float fDiaphaneity;//͸����
	int nInterval;		//��ʾ���
	int nEndX;//ˮƽƫ��
	int nEndY;//��ֱƫ��
}OSDPos;
//OSD-timestamp
typedef struct _STRTimeStamp
{
	int nEnable;	//����
	OSDPos _POS;	//��ʾλ��
	int nDayMode;	//����ģʽ
	int nHourMode;	//ʱ��ģʽ
	int nWeekMode;	//����ģʽ
}STRTimeStamp;
//OSD-channel name
typedef struct _STRChannelName
{
	int nEnable;	//����
	OSDPos _POS;	//��ʾλ��
	char strName[MAXNAMELENGTH];	//ͨ������
}STRChannelName;
//OSD
typedef struct _STROSDPano
{
	STRTimeStamp TimeStamp;
	STRChannelName ChannelName[MAXOSDCHANNELNAME];
	int OSD_Flashing;
	int Channel_Num;
}STROSDPano;

//��������ڵ�
typedef struct _tagEncodePara{
	int enable;
	int frame_rate;
	int code_rate;
	int width;
	int height;
}EncodePara;


//����ƴ�Ӳ����ṹ��
typedef struct _PanoParam
{
	int nStitchProcessID;			//ƴ����id�������ڱ�ʶ��ͬ��ƴ����
	int nFrameWidth;				//ͼ����
	int nFrameHeight;				//ͼ��߶�
	int nFramePitch;
	int nPanoWidth;					//ȫ��ͼ����
	int nPanoHeight;				//ȫ��ͼ��߶�
	int nPanoPitch;
	int nCameraNum;					//ͼ������
	int nInputFrameType;			//����ͼ�����ݸ�ʽ
	int nOutFrameType;				//���ͼ�����ݸ�ʽ
	float fInterScale;				//�ڲ����ű���
	float fOutScale;				//���ͼ�����ű���
	bool bPreview;					//Ԥ��
	int nMaskMode;					//����ģʽ
	char strMaskPre[MAXIDLENGTH];			//�����ļ���ǰ׺
	char strCameraID[MAXIDLENGTH];			//�����������
	char strCameraName[MAXNAMELENGTH];		//�����������
	char strMasks[MAXCAMERAS][MAXNAMELENGTH];	//mask�ļ�������Ŵӵ͵��߶�Ӧƴ�����д�����
	int nArrVectorIndex[MAXCAMERAS];//�������
	float fSharpness;				//�𻯲���
	int nBlenderMode;				//�ں�ģʽ
	int nUseTBBMode;				//TBB����ģʽ
	int nRectMode;					//��Ե�ü�ģʽ
	int nTimeMode;					//�����ʱ��ʾģʽ
	int nUVMode;					//UVģʽ
	double dHArray[MAXCAMERAS][9];	//ƴ�Ӿ���
	bool nEnableCalib;				//��ƵУ��
	Trpano_CaliPoints CaliPoint;	//У��������
	Trpano_ColorCorrect ColorCorrect;//ɫ��У��
	STROSDPano OSDInfo;				//OSD����
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



//��������ڵ�
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



//����˿ڽڵ�
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

//��Ƶƽ̨�ڵ�
typedef struct _tagVideoPlatform{
	int server_type;
	int server_port;
	char ip[16];
	char user_name[32];
	char user_pwd[32];
	char server_url[MAXURLLENGTH];
}VideoPlatform;

typedef struct _tagVrtCamera{
	char strCameraID[MAXIDLENGTH];			//�����������
	char strCameraName[MAXNAMELENGTH];		//�����������
	int bEnable;					//����
	int nPanoWidth;					//ȫ��ͼ����
	int nPanoHeight;				//ȫ��ͼ��߶�
	int nCameraNum;					//ͼ������
}VrtCamera;	//�����������

//ͼ������ͷ
typedef struct _PanoDataHeader
{
	int nStitchProcessID;
	int nPanoWidth;
	int nPanoHeight;
	int nCodeType;
}PanoDataHeader;


typedef enum _TrstErrorCode
{
	// sip�����룬����
	//01) 100 Trying   ˵��caller���ں��У�����û��ϵ��callee�� 
	//180 Ringing ˵��callee�Ѿ�����ϵ��,callee����������.�յ������Ϣ�󣬵ȴ�200 OK  
	//02) 181 Call is being forwarded  ˵��call������·�ɵ�����һ��Ŀ�ĵ�  
	//03) 182 Queued   ˵��callee��ǰ�ǲ��ɻ�õģ����ǶԷ�����ֱ�Ӿܾ����У�����ѡ����ں��ж�����  
	//04) 183 Session progress   ��������callerƵ��(inband)���󡣵���PSTN�յ�һ��ISDN��Ϣ��SIP gateway ����183 Session progress ��  

	//2xx successful Responses   
	//200 OK   

	//3xx Redirection Responses   
	//05) 300 Multiple choices  ˵�����еĵ�ַ�������ɶ����ַ�����еĵ�ַ�����ṩ�������û����û�������Դ�                           ��ѡ����ϵ�ĸ���   
	//06) 301 Moved permanently   ˵��ָ����ַ���û��Ѿ���Զ�����ã���ͷ���Ѿ�������һ����ַ�滻��.  
	//07) 302 Moved temporarily   ˵��ָ����ַ���û���ʱ�����ã���ͷ���Ѿ�������һ����ַ������.  
	//08) 305 Use proxy   ˵��caller������һ��proxy����ϵcallee. 
	//09) 380 Alternative service   ˵��call���ɹ������ǿ�ѡ�������ķ���   

	//4xx Request Failure Responses    
	//10) 400 Bad Request   ˵�����ڷǷ���ʽ�������ܱ���⡣  
	//11) 401 Unauthorized ˵��������Ҫ�û���֤��  
	//12) 402 Payment required  ˵����ɻỰ��Ҫ����.  
	//13) 403 Forbidden   ˵��server�Ѿ��յ�����������󵫲��ṩ����  
	//14) 404 Not Found   ˵��server����ȷ����Ϣ��ָ���������û�������.  
	//15) 405 Method Not Allowed   ˵��������ָ���ķ����ǲ�������ġ�������һ������ķ����б�  
	//16) 406 Not Acceptable ˵�����������Դֻ��ĳЩ���������������Ӧ����Ŀǰͷ(header)��ָ�������󲻽���.  
	//17) 407 Proxy authentication required   ��401 Unauthorized response����.���ǣ���˵��client����������proxy����֤�Լ���  
	//18) 408 Request timeout   ˵����timeoutʱ�����ǰ��server���ܲ�����Ӧ.  
	//19) 409 Conflict   ˵�����ں͵�ǰ��Դ״̬������ͻ�������ܱ�����  
	//20) 410 Gone   ˵��������Դ��server����Զ�����ã�Ҳû��ת���ĵ�ַ��  
	//21) 411 Length required  ˵���û��ܾ�����û�ж���content���ȵ�����  
	//22) 413 Request entity too large   ˵��server�ܾ�����������Ϊ��̫�󣬳�����server�ܴ���Ĵ�С��  
	//23) 414 Request-URI too long   ˵��server�ܾ�����������Ϊ�����URI̫����server���ܽ�������  
	//24) 415 Unsupported media   ˵��server�ܾ�����������Ϊbody��ʽ����Ŀ���ն�֧��  
	//25) 420 Bad extension   ˵��server���������header��ָ������չЭ�顣  
	//26) 480 Temporarily unavailable   ˵��callee�Ѿ�����ϵ�ϣ�������ʱ�����á�  
	//27) 481 Call leg/transaction does not exist  ˵��server���ں���������������һ��û��ƥ��legID��BYE��     ����һ��û��ƥ�������CANCEL��   
	//28) 482 Loop detected   ˵��server�յ���һ���������Լ�·��������.  
	//29) 483 Too many hops   ˵��server�յ���һ����������Ҫ��hop����������header����������hop��.  
	//30) 484 Address incomplete   ˵��server�յ�һ����ַ������������. 
	//31) 485 Ambiguous   ˵��server�յ�һ����������callee�ĵ�ַ�ǲ���ȷ�ģ�Ҳû�п��ܱ��õĵ�ַ��ѡ��  
	//32) 486 Busy here   ˵��callee�Ѿ�����ϵ�ϣ��������ǵ�ϵͳ���ܳ��ܶ����call.   
	//488  ����ʱ���ܽ��У���  

	//5xx Server Failure Responses   
	//33) 500 Server internal error   ˵��server��gateway�����������Ӷ����ܴ�������.  
	//34) 501 Not implemented   ˵��server��gateway��֧�������������Ĺ���.  
	//35) 502 Bad gateway   ˵��server��gateway������server�յ�һ���Ƿ���Ӧ.
	//36) 503 Service unavailable   ˵�����ڳ����ػ�ά�����⣬server��gateway���ܴ�������.  
	//37) 504 Gateway timeout  ˵��server��gatewayû�д�����һ��server(��location server)�յ�һ����ʱ����Ӧ.     
	//38) 505 Version not supported   ˵��server��gateway��֧�����������õ���SIP�汾��   

	//6xx Global Responses   
	//39) 600 Busy everywhere  ˵��callee�Ѿ�����ϵ�ϣ����Ǵ���æ״̬�У������ʱ�䲻�ܽ���call��  
	//40) 603 Decline   ˵��callee�Ѿ�����ϵ�ϣ����ǲ��ܻ������call��  
	//41) 604 Does not exist anywhere   ˵��server����ʽ����Ϣ˵��callee�������������С�  
	//42) 606 Not acceptable   ˵��callee�Ѿ�����ϵ�ϣ�����session������ĳЩ���治�����ܡ�

	//=====================ͨ�������====================================
	//������SIPЭ��ջ״̬������
	TR_OK	= 10000,		//����
	TR_ERROR = 10001,		//����
	TR_ERROR_LOGIN = 10002,		//��¼ʧ��
	TR_ERROR_LICENCE =10003,	//��Ȩ�������
	TR_ERROR_CAPBILITY_LIMITED = 1004,	//�豸��������
	TR_ERROR_STREAM_IN = 10005,		//������Ƶʧ��
	TR_ERROR_WAITE_FOR_TIME_OUT= 10006,	//�ȴ���ʱ
	TR_ERROR_FILE_NOT_EXIST = 10007,	//�����ļ�������
	TR_ERROR_STITCHING_PARAM = 10008,	//ƴ�Ӳ�������
	//������״̬/�����������ڽ��̼䴫�ݣ����ϴ���̸�����Ӧ�Ĵ��������ִ���쳣����
	//����״̬/������������ڽ����ڲ�ʹ��

	//=====================ϵͳ�����====================================
	TR_ERROR_LOGIN_SUCCEED	= 20000,	//ϵͳ�ǳɹ�
	TR_ERROR_LOGIN_FAILED	= 20001,	//ϵͳ�������,��¼ʧ��


	TR_ERROR_LOGIN_ACC_ISNULL = 21001,	//��¼�û���Ϊ��
	TR_ERROR_LOGIN_PSW_ISNULL = 21002,	//��¼����Ϊ��
	TR_ERROR_LOGIN_ACC = 21003,	//��¼�û�������
	TR_ERROR_LOGIN_PSW = 21004,	//��¼�������
	TR_ERROR_LOGIN_ACCESS_DENY = 21005,	//�ܾ�����
	TR_ERROR_LOGIN_VERIFY_FAILED = 21006,	//�˻���֤ʧ��
	//...				//������

	TR_ERROR_MEM_SUCCEED = 22000,	//�ڴ�����ɹ�
	TR_ERROR_MEM_FAILED	= 22001,	//�ڴ���������,δ֪����

	TR_ERROR_MEM_ALLOC = 22003,	//�����ڴ�ʧ��
	TR_ERROR_MEM_RELEASE = 22004,	//�ͷ��ڴ�ʧ��
	TR_ERROR_MEM_LENGTH = 22005,	//�ڴ泤�Ȳ���
	TR_ERROR_MEM_NULL = 22006,	//�ڴ�ָ��Ϊ��

	TR_LICENCE_SUCCEED = 23000,		//��Ȩ�ɹ�
	TR_LICENCE_ERROR = 23001,		//��Ȩʧ��
	TR_LICENCE_LIMITED = 23002,		//��Ȩ���ƣ��豸��������
	TR_LICENCE_TIMEOUT = 23003,		//��Ȩ����

	//...				//������

	//=====================���������======================================
	TR_ERROR_NETWORK_SUCCEED = 30000,	//��������ɹ�
	TR_ERROR_NETWORK_FAILED	= 30001,	//�������ʧ��

	//=====================ƴ�Ӳ�������====================================
	TR_ERROR_STITCHING_SUCCEED	= 40000,	//ƴ�Ӳ�����ɹ�
	TR_ERROR_STITCHING_FAILED	= 40001,	//ƴ�Ӳ��������,δ֪����

	TR_ERROR_STITCHING_FORMAT	= 41001,	//ͼ���ʽ��֧��
	TR_ERROR_STITCHING_IMAGEDATA	= 41002,	//ͼ��������Ч
	TR_ERROR_STITCHING_IMAGESIZE	= 41003,	//ͼ��ֱ�����Ч
	TR_ERROR_STITCHING_NULL	= 41004,		//ƴ������Ч
	TR_ERROR_STITCHING_NULLPOINT	= 41005,	//����ָ��Ϊ��
	TR_ERROR_STITCHING_OVERFLOW	=	41006,		//ƴ�����������
	TR_ERROR_STITCHING_IMAGENUM = 41007,

	TR_ERROR_STITCHING_DEVICE_NULL = 42000,//�豸��Ч
	TR_ERROR_STITCHING_DEVICE_DRIVE = 42001,//�豸������Ч
	TR_ERROR_STITCHING_DEVICE_NPP = 42002, //�豸��֧��npp
	TR_ERROR_STITCHING_DEVICE_TBB = 42003,//�豸��֧��tbb
	TR_ERROR_STITCHING_DEVICE_BIND = 42004,//�豸��ʧ��

	TR_ERROR_STITCHING_CUDA = 43000,//cuda����ʧ��
	TR_ERROR_STITCHING_CUDA_UPLOAD = 43001,//cuda��������ʧ��
	TR_ERROR_STITCHING_CUDA_DOWNLOAD = 43002,//cuda��������ʧ��
	TR_ERROR_STITCHING_CUDA_ALLOC = 43003,//cuda�ڴ����ʧ��

	TR_ERROR_STITCHING_CFG = 44000,//ƴ�Ӳ�������
	TR_ERROR_STITCHING_CFG_NULL = 44001,//ƴ�Ӳ���Ϊ��
	TR_ERROR_STITCHING_CFG_READ = 44002,//��ȡƴ�Ӳ���ʧ��
	TR_ERROR_STITCHING_CFG_SAVE = 44003,//����ƴ�Ӳ���ʧ��
	TR_ERROR_STITCHING_CFG_NUMBERS = 44004,//��Ƶ������ƥ��
	TR_ERROR_STITCHING_CFG_MASKS = 44005,//ƴ��������Ч
	TR_ERROR_STITCHING_CFG_RECT = 44006,//ƴ��������Ч

	TR_ERROR_STITCHING_VERIFY_SUCCEED = 45000,//ƴ��ģ����Ȩ�ɹ�
	TR_ERROR_STITCHING_VERIFY_FAILED = 45001,//ƴ��ģ����Ȩʧ��

	TR_ERROR_STITCHING_PREPARE= 46000,//Ԥƴ��ʧ��
	TR_ERROR_STITCHING_RECTMODE= 46001,//�ü��������
	TR_ERROR_STITCHING_BLENDMODE= 46002,//�ں�ģʽ����
	TR_ERROR_STITCHING_TBBMODE= 46003,//tbb����ģʽ����
	TR_ERROR_STITCHING_WAITING_FOR_DATA= 46004,//�ȴ�����

	TR_ERROR_STITCHING_PANO_NULL= 47000,//��ƴ������


	//...				//������


	//=====================ͼ�������====================================
	TR_ERROR_IMAGE_SUCCEED	= 50000,	//ͼ�������ɹ�
	TR_ERROR_IMAGE_FAILED	= 50001,	//ͼ����������,δ֪����
	TR_ERROR_IMAGE_WIDTH = 51001,	//ͼ������Ч
	TR_ERROR_IMAGE_HEIGHT = 51002, //ͼ��߶���Ч
	TR_ERROR_IMAGE_SIZE = 51003, 	//ͼ��ֱ�����Ч
	TR_ERROR_IMAGE_NULL = 51004,	//ͼ��Ϊ��
	//...				//������

	//=====================��Ƶ�����====================================
	TR_ERROR_STREAM_SUCCEED	= 60000,	//��Ƶ������ɹ�
	TR_ERROR_STREAM_FAILED	= 60001,	//��Ƶ���������,δ֪����

	TR_ERROR_STREAM_WIDTH = 61001,	//��Ƶ�����Ч
	TR_ERROR_STREAM_HEIGHT = 61002, //��Ƶ�߶���Ч
	TR_ERROR_STREAM_SIZE = 61003, 	//��Ƶ�ֱ�����Ч
	TR_ERROR_STREAM_NULL = 61004,	//��ƵΪ��

	TR_ERROR_STREAM_REQEST = 62000,	//��Ƶ����ʧ��
	TR_ERROR_STREAM_PLAY = 62001,	//��Ƶ����ʧ��	
	TR_ERROR_STREAM_STOP = 62002,	//��Ƶֹͣʧ��
	TR_ERROR_STREAM_PLAYFORWARD = 62003,	//��Ƶ���ʧ��
	TR_ERROR_STREAM_PLAYBACKWARD = 62004,	//��Ƶ����ʧ��
	TR_ERROR_STREAM_PLAYPAUSE = 62004,	//��Ƶ��ͣʧ��
	//...				//������


	//=====================����������====================================
	TR_ERROR_CODEC_SUCCEED = 70000,			//����������ɹ�
	TR_ERROR_CODEC_FAILED = 70001,			//�������������,δ֪����

	TR_ERROR_CODEC_ENCODING_INIT = 71001,	//��������ʼ��ʧ��
	TR_ERROR_CODEC_ENCODING_SIZE = 71002,	//�������ֱ��ʲ�ƥ��
	TR_ERROR_CODEC_ENCODING_FORMAT = 71003,	//�����������ʽ��֧��

	TR_ERROR_CODEC_DECODING_INIT = 72001,	//��������ʼ��ʧ��
	TR_ERROR_CODEC_DECODING_SIZE = 72002, 	//�������ֱ��ʲ�ƥ��
	TR_ERROR_CODEC_DECODING_FORMAT = 72003, //�����������ʽ��֧��
	//...				//������

	//=====================OSG�����=======================================
	TR_ERROR_OSG_SUCCEED = 80000,			// OSG����
	TR_ERROR_OSG_FAILED = 80001,			// OSG����

	TR_ERROR_OSG_VIEWHWND = 81001,			// ���ھ��Ϊ��
	TR_ERROR_OSG_ISINIT = 81002	,			// �ٴγ�ʼ��
	TR_ERROR_OSG_INITFILEPATH = 81003,		// ��ʼ���ļ�·��Ϊ��
	TR_ERROR_OSG_LOADFILE = 81004,			// XML�������
	TR_ERROR_OSG_XMLERROR = 81005,			// XML��ȡ����
	TR_ERROR_OSG_XMLROOT = 81006,			// XML���ڵ�Ϊ��
	TR_ERROR_OSG_XMLELEMENT = 81007,			// XML�ڵ����
	TR_ERROR_OSG_INITSCENE = 81008,			// ��ʼ��OSG����ʧ�� 
	TR_ERROR_OSG_CAMMODEL = 81009,			// ��ʼ���������ģ��ʧ��
	TR_ERROR_OSG_CAMMARK = 81010,			// ���������־ʧ��

	TR_ERROR_OSG_PARAMETER_CAMID = 82001,	// �������IDΪ��
	TR_ERROR_OSG_PARAMETER_VIEWPORT = 82002,	// �ӿڲ������� ȫΪ0

	TR_ERROR_OSG_TOUR_ISADD = 83001,			// ��ID·�������
	TR_ERROR_OSG_TOUR_NEWERROR = 83002,		// ����·���������
	TR_ERROR_OSG_TOUR_LOADFILE = 83003,		// ·���ļ��������
	TR_ERROR_OSG_TOUR_FILEERROR = 83004,		// ·���ļ���ȡ����
	TR_ERROR_OSG_TOUR_FILEROOT = 83005,		// ·�����ڵ�Ϊ��
	TR_ERROR_OSG_TOUR_OSGERROR = 83006,		// ��ʼ���λ�ȡOSGʧ��
	TR_ERROR_OSG_TOUR_PATHMAN = 83007,		// ��ʼ����manager����
	TR_ERROR_OSG_TOUR_SETOSG = 83008,		// ��ʼ��������OSG����
	TR_ERROR_OSG_TOUR_SETHWND = 83009,		// ��ʼ�������ô��ھ������
	TR_ERROR_OSG_TOUR_ANIMATION = 83010,		// ����·��ID��ʼ���γ���
	TR_ERROR_OSG_TOUR_MANERROR = 83011		// ��ʼ���β���������



	//=====================XX�����====================================	
	//...				//������

}TrstErrorCode;
