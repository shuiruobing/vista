#pragma once
#include "TrstPanoStruct.h"
//#include "trpanosdk.h"

//线程数据结构

void Device2DToHost_YUV420(unsigned char* yuv_cuda, int width, int pitch, int height, unsigned char* yuv420_host);
void NV12DToYUV420(unsigned char* nv12, int width, int height, unsigned char* yuv420);
typedef struct _tagThreadData
{
	int nObject;
	void* pThis;

}ThreadData;

//帧数据格式
typedef enum _FMT_FRAME
{
	FMT_YUV420		= 0,
	FMT_YUV422		,
	FMT_RGB24		=4,
	FMT_RGB32		,
	FMT_H264		=8
}FMT_FRAME;



class __declspec(dllexport) TrstPano
{
public:
	TrstPano(void);
	~TrstPano(void);
private:	
	ThreadData* pThreadData;
	long    m_TimeCounterOut;
	long    m_TimeCounterIn;
	PanoParam m_stPanoParam;
	StitchParamter m_stPanoStitch;
	bool firstRun;
public:
	int m_DiagnosticMode;
	int Handle;
public:
	/*!@function
	***************************************************************************
	<PRE>
	函数名：SetDeviceUsed
	功能：设置拼接设备
	//@	输入参数：nHandle：拼接器句柄
	//@	输入参数：nGPUID：GPU编号，0，1，...
	返回值：
	正常，返回非负，
	作者：TRST
	</PRE>
	***************************************************************************/
	void SetDeviceUsed(int nHandle, int nGpuID);

	/*!@function
	***************************************************************************
	<PRE>
	函数名：SetConfigFileType
	功能：设置拼接参数文件类型
	//@	输入参数：nFileType：0,ini文件，1-xml文件
	//@	
	返回值：
	正常，返回非负，
	作者：TRST
	</PRE>
	***************************************************************************/
	void SetConfigFileType(int nFileType = 0);

	/*!@function
	***************************************************************************
	<PRE>
	函数名：InitPanoSDK
	功能：设置拼接参数
	//@	输入参数：strGroupID：
	//@			strIniPath：拼接参数文件
	//@			strMaskDir：mask目录
	//@			nFrameWidth：输入图像宽度
	//@			nFrameHeight：输入图像高度
	//@			nOutFrameWidth：输出帧宽度
	//@			nOutFrameHeight：输出帧高度
	//@			dScale：拼接处理缩放因子，不影响最后的输出尺寸。取值0-2之间，数值越小效率越高质量越差
	返回值：
	正常，返回非负，
	作者：TRST
	</PRE>
	***************************************************************************/
	int InitPanoSDK(char* strIniPath,
		int img_num,
		int nFrameWidth,
		int nFrameHeight,
		int nOutFrameWidth,
		int nOutFrameHeight
		);


int InitPanoSDK4K(char* strIniPath,
		int img_num,
		int nFrameWidth,
		int nFramePitch,
		int nFrameHeight,
		int nOutFrameWidth,
		int nOutFramePitch,
		int nOutFrameHeight
		);
	/*!@function
	***************************************************************************
	<PRE>
	函数名：InitPanoSDK_Ex
	功能：设置拼接参数
	//@	输入参数：strGroupID：
	//@			fHarray：单应矩阵
	//@			strPanoParam：拼接控制参数
	//@			nOutFrameWidth：输出帧宽度
	//@			nOutFrameHeight：输出帧高度
	//@			dScale：拼接处理缩放因子，不影响最后的输出尺寸。取值0.5-2之间，数值越小效率越高质量越差
	返回值：
		返回拼接器句柄
	作者：TRST
	</PRE>
	***************************************************************************/
	int InitPanoSDK_Ex(char* strGroupID,
		double fHarray[][9],
		PanoParam strPanoParam,
		int& nOutFrameWidth,
		int& nOutFrameHeight,
		double dScale
		);


	/*!@function
	***************************************************************************
	<PRE>
	函数名：UnInitPanoSDK
	功能：设置拼接参数
	//@	输入参数：nHandle：拼接器句柄
	返回值：
		成功，返回拼TRSTPANOOK
	作者：TRST
	</PRE>
	***************************************************************************/
	int UnInitPanoSDK(int nHandle);

	/*!@function
	***************************************************************************
	<PRE>
	函数名：DoStitchiing
	功能：执行拼接操作
	//@	输入参数：pFrameBufIn：输入帧数据
	//@			nCameraNum：摄像头数量
	//@输出参数：pBufBigStreamOut：输出大码流帧指针
	//@			pBufSmallStreamOut：输出小码流帧指针
	//@			nOutFrameWidth：输出大码流宽度，小马流=大码流/3
	//@			nOutFrameHeight：输出大码流高度，小马流=大码流/3
	//@			nUVModel:UV模式，nUVModel=0正常，nUVModel=1交换UV
	//@			nFrameType:输入图像格式FMT_YUV420= 0,FMT_YUV422	,FMT_RGB24	=4,	FMT_RGB32,	FMT_H264=8
	返回值：
	正常，返回非负，
	作者：TRST
	</PRE>
	***************************************************************************/
	int DoStitching(int nHandle,
		unsigned char **pFrameBufIn,
		int nCameraNum,
		unsigned char*& pBufBigStreamOut,
		unsigned char*& pBufSmallStreamOut,
		int nUVModel = 0,
		int nFrameType = 0
		);
	/*!@function
	***************************************************************************
	<PRE>
	函数名：DoStitchiing
	功能：执行拼接操作
	//@	输入参数：pFrameBufIn：输入帧数据
	//@			  nCameraNum：摄像头数量
	//@ 输出参数：pBufBigStreamOut：输出大码流帧指针数组
	//@			  pBufSmallStreamOut：输出小码流帧指针数组
	返回值：
	正常，返回零；错误，返回错误码。
	作者：TRST
	</PRE>
	***************************************************************************/
	int DoStitching(unsigned char** pFrameBufIn,
					int nCameraNum,
					unsigned char* pBufBigStreamOut,
					unsigned char* pBufSmallStreamOut);

	/*!@function
	***************************************************************************
	<PRE>
	函数名：SetFrameTypeIn
	功能：设置输入数据帧格式
	//@	输入参数：nFrameType：输入帧数据格式
	返回值：正常，返回非负，
	作者：TRST
	</PRE>
	***************************************************************************/
	int SetFrameTypeIn(int nHandle, int nFrameType);

	/*!@function
	***************************************************************************
	<PRE>
	函数名：SetFrameTypeOut
	功能：设置输出数据帧格式
	//@	输入参数：nFrameType：输入帧数据格式
	返回值：正常，返回非负，
	作者：TRST
	</PRE>
	***************************************************************************/
	int SetFrameTypeOut(int nHandle, int nFrameType);
	int SetFrameID(int nHandle, int nFrameID);
	int GetFrameID(int nHandle);
	/*!@function
	***************************************************************************
	<PRE>
	函数名：SetPanoVideoSize
	功能：设置输出 
	//@	输入参数：nHandle：拼接器句柄
	//@	nPanoWidth：输出图像宽度
	//@	nPanoHeight：输出图像高度
	返回值：正常，返回非负，
	作者：TRST
	</PRE>
	***************************************************************************/
	int SetPanoVideoSize(int nHandle, int nPanoWidth, int nPanoHeight);
	//更新单应矩阵
	int UpdateHomoArray(int nHandle,double fHarray[9]);
	//设置掩码使用模式， 0-读取本地掩码，1-在线计算掩码直边；2-在线计算掩码曲边.必须先于
	int SetMaskUseMode(int nHandle, int nMaskMode);

	////////////////////////////////亮度/////////////////////////////////////
	//更新亮度参数
	int UpdateBrightness(int nHandle, double fValue);
	//设置亮度调整模式
	int SetAutoBrightness(int nHandle, bool bAutoMode);
	//取拼接参数
	int GetStitchingParam(PanoParam& param);
	//从文件读拼接参数
	int ReadStitchingParam(char* strIniFileName, PanoParam& param);


	//////////////////////////////////////////////////////////////////////////
	//双线程拼接接口
public:
	/*!@function
	***************************************************************************
	<PRE>
	函数名：InitPanoSDK_Th
	功能：设置拼接参数
	//@	输入参数：strGroupID：
	//@			strIniPath：拼接参数文件
	//@			strMaskDir：mask目录
	//@			nFrameWidth：输入图像宽度
	//@			nFrameHeight：输入图像高度
	//@			nOutFrameWidth：输出帧宽度
	//@			nOutFrameHeight：输出帧高度
	//@			dScale：拼接处理缩放因子，不影响最后的输出尺寸。取值0-2之间，数值越小效率越高质量越差
	返回值：
	正常，返回非负，
	作者：TRST
	</PRE>
	***************************************************************************/
	int InitPanoSDK_Th(char* strIniPath,
		int img_num,
		int nFrameWidth,
		int nFrameHeight,
		int nOutFrameWidth,
		int nOutFrameHeight);
	int InitPanoSDK_Th4K(char* strIniPath,
		int img_num,
		int nFrameWidth,
		int nFramePitch,
		int nFrameHeight,
		int nOutFrameWidth,
		int nOutFramePitch,
		int nOutFrameHeight);

	/*!@function
	***************************************************************************
	<PRE>
	函数名：UpdateData_Th
	功能：设置拼接图像，取回拼接结果
	//@	输入参数：nHandle：句柄
	//@			pFrameBufIn：拼接图像指针
	//@			pBufBigStreamOut：拼接结果图像指针，大码流
	//@			pBufSmallStreamOut：拼接结果图像指针，小码流
	//@			nUVModel：拼接图像UV模式
	//@			nFrameType：拼接图像格式
	返回值：
	正常，返回非负，
	作者：TRST
	</PRE>
	***************************************************************************/
	int UpdateData_Th(int nHandle,
		int compensate,
		unsigned char **yuvs,
		unsigned char* res_yuv);

	int UpdateData_CustomizeOSD(int nHandle,
		int compensate,
		unsigned char **yuvs,
		unsigned char* res_yuv,
		STROSDPano errOSDInfo);

	void resize2D(int nHandle, unsigned char* src_img, int src_width, int src_height, 
		unsigned char* dst_img, int dst_width, int dst_height);

	char* ReadEdition();
	
	int UpdateData_4K(int nHandle, 
		unsigned char** yuvs,
		unsigned char** yuvs_host,
		unsigned char* res_yuv);

	int UpdateData_4KCustomizeOSD(int nHandle, 
		unsigned char** yuvs,	
		unsigned char** yuvs_host, 
		unsigned char* res_yuv,
		STROSDPano errOSDInfo);


	void resize2D_4K(int nHandle, unsigned char* src_img, int src_width, int src_pitch, int src_height, 
		unsigned char* dst_img, int dst_width, int dst_pitch, int dst_height);
			

private:
	//停止拼接
	int StopStitch(int nHandle, int nType);

};