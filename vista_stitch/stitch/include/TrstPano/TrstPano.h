#pragma once
#include "TrstPanoStruct.h"
//#include "trpanosdk.h"

//�߳����ݽṹ

void Device2DToHost_YUV420(unsigned char* yuv_cuda, int width, int pitch, int height, unsigned char* yuv420_host);
void NV12DToYUV420(unsigned char* nv12, int width, int height, unsigned char* yuv420);
typedef struct _tagThreadData
{
	int nObject;
	void* pThis;

}ThreadData;

//֡���ݸ�ʽ
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
	��������SetDeviceUsed
	���ܣ�����ƴ���豸
	//@	���������nHandle��ƴ�������
	//@	���������nGPUID��GPU��ţ�0��1��...
	����ֵ��
	���������طǸ���
	���ߣ�TRST
	</PRE>
	***************************************************************************/
	void SetDeviceUsed(int nHandle, int nGpuID);

	/*!@function
	***************************************************************************
	<PRE>
	��������SetConfigFileType
	���ܣ�����ƴ�Ӳ����ļ�����
	//@	���������nFileType��0,ini�ļ���1-xml�ļ�
	//@	
	����ֵ��
	���������طǸ���
	���ߣ�TRST
	</PRE>
	***************************************************************************/
	void SetConfigFileType(int nFileType = 0);

	/*!@function
	***************************************************************************
	<PRE>
	��������InitPanoSDK
	���ܣ�����ƴ�Ӳ���
	//@	���������strGroupID��
	//@			strIniPath��ƴ�Ӳ����ļ�
	//@			strMaskDir��maskĿ¼
	//@			nFrameWidth������ͼ����
	//@			nFrameHeight������ͼ��߶�
	//@			nOutFrameWidth�����֡���
	//@			nOutFrameHeight�����֡�߶�
	//@			dScale��ƴ�Ӵ����������ӣ���Ӱ����������ߴ硣ȡֵ0-2֮�䣬��ֵԽСЧ��Խ������Խ��
	����ֵ��
	���������طǸ���
	���ߣ�TRST
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
	��������InitPanoSDK_Ex
	���ܣ�����ƴ�Ӳ���
	//@	���������strGroupID��
	//@			fHarray����Ӧ����
	//@			strPanoParam��ƴ�ӿ��Ʋ���
	//@			nOutFrameWidth�����֡���
	//@			nOutFrameHeight�����֡�߶�
	//@			dScale��ƴ�Ӵ����������ӣ���Ӱ����������ߴ硣ȡֵ0.5-2֮�䣬��ֵԽСЧ��Խ������Խ��
	����ֵ��
		����ƴ�������
	���ߣ�TRST
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
	��������UnInitPanoSDK
	���ܣ�����ƴ�Ӳ���
	//@	���������nHandle��ƴ�������
	����ֵ��
		�ɹ�������ƴTRSTPANOOK
	���ߣ�TRST
	</PRE>
	***************************************************************************/
	int UnInitPanoSDK(int nHandle);

	/*!@function
	***************************************************************************
	<PRE>
	��������DoStitchiing
	���ܣ�ִ��ƴ�Ӳ���
	//@	���������pFrameBufIn������֡����
	//@			nCameraNum������ͷ����
	//@���������pBufBigStreamOut�����������ָ֡��
	//@			pBufSmallStreamOut�����С����ָ֡��
	//@			nOutFrameWidth�������������ȣ�С����=������/3
	//@			nOutFrameHeight������������߶ȣ�С����=������/3
	//@			nUVModel:UVģʽ��nUVModel=0������nUVModel=1����UV
	//@			nFrameType:����ͼ���ʽFMT_YUV420= 0,FMT_YUV422	,FMT_RGB24	=4,	FMT_RGB32,	FMT_H264=8
	����ֵ��
	���������طǸ���
	���ߣ�TRST
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
	��������DoStitchiing
	���ܣ�ִ��ƴ�Ӳ���
	//@	���������pFrameBufIn������֡����
	//@			  nCameraNum������ͷ����
	//@ ���������pBufBigStreamOut�����������ָ֡������
	//@			  pBufSmallStreamOut�����С����ָ֡������
	����ֵ��
	�����������㣻���󣬷��ش����롣
	���ߣ�TRST
	</PRE>
	***************************************************************************/
	int DoStitching(unsigned char** pFrameBufIn,
					int nCameraNum,
					unsigned char* pBufBigStreamOut,
					unsigned char* pBufSmallStreamOut);

	/*!@function
	***************************************************************************
	<PRE>
	��������SetFrameTypeIn
	���ܣ�������������֡��ʽ
	//@	���������nFrameType������֡���ݸ�ʽ
	����ֵ�����������طǸ���
	���ߣ�TRST
	</PRE>
	***************************************************************************/
	int SetFrameTypeIn(int nHandle, int nFrameType);

	/*!@function
	***************************************************************************
	<PRE>
	��������SetFrameTypeOut
	���ܣ������������֡��ʽ
	//@	���������nFrameType������֡���ݸ�ʽ
	����ֵ�����������طǸ���
	���ߣ�TRST
	</PRE>
	***************************************************************************/
	int SetFrameTypeOut(int nHandle, int nFrameType);
	int SetFrameID(int nHandle, int nFrameID);
	int GetFrameID(int nHandle);
	/*!@function
	***************************************************************************
	<PRE>
	��������SetPanoVideoSize
	���ܣ�������� 
	//@	���������nHandle��ƴ�������
	//@	nPanoWidth�����ͼ����
	//@	nPanoHeight�����ͼ��߶�
	����ֵ�����������طǸ���
	���ߣ�TRST
	</PRE>
	***************************************************************************/
	int SetPanoVideoSize(int nHandle, int nPanoWidth, int nPanoHeight);
	//���µ�Ӧ����
	int UpdateHomoArray(int nHandle,double fHarray[9]);
	//��������ʹ��ģʽ�� 0-��ȡ�������룬1-���߼�������ֱ�ߣ�2-���߼�����������.��������
	int SetMaskUseMode(int nHandle, int nMaskMode);

	////////////////////////////////����/////////////////////////////////////
	//�������Ȳ���
	int UpdateBrightness(int nHandle, double fValue);
	//�������ȵ���ģʽ
	int SetAutoBrightness(int nHandle, bool bAutoMode);
	//ȡƴ�Ӳ���
	int GetStitchingParam(PanoParam& param);
	//���ļ���ƴ�Ӳ���
	int ReadStitchingParam(char* strIniFileName, PanoParam& param);


	//////////////////////////////////////////////////////////////////////////
	//˫�߳�ƴ�ӽӿ�
public:
	/*!@function
	***************************************************************************
	<PRE>
	��������InitPanoSDK_Th
	���ܣ�����ƴ�Ӳ���
	//@	���������strGroupID��
	//@			strIniPath��ƴ�Ӳ����ļ�
	//@			strMaskDir��maskĿ¼
	//@			nFrameWidth������ͼ����
	//@			nFrameHeight������ͼ��߶�
	//@			nOutFrameWidth�����֡���
	//@			nOutFrameHeight�����֡�߶�
	//@			dScale��ƴ�Ӵ����������ӣ���Ӱ����������ߴ硣ȡֵ0-2֮�䣬��ֵԽСЧ��Խ������Խ��
	����ֵ��
	���������طǸ���
	���ߣ�TRST
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
	��������UpdateData_Th
	���ܣ�����ƴ��ͼ��ȡ��ƴ�ӽ��
	//@	���������nHandle�����
	//@			pFrameBufIn��ƴ��ͼ��ָ��
	//@			pBufBigStreamOut��ƴ�ӽ��ͼ��ָ�룬������
	//@			pBufSmallStreamOut��ƴ�ӽ��ͼ��ָ�룬С����
	//@			nUVModel��ƴ��ͼ��UVģʽ
	//@			nFrameType��ƴ��ͼ���ʽ
	����ֵ��
	���������طǸ���
	���ߣ�TRST
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
	//ֹͣƴ��
	int StopStitch(int nHandle, int nType);

};