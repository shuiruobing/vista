#ifndef MERGE_IMG_HEADER
#define MERGE_IMG_HEADER

/*
 * ���ܣ�ֱ��ƴ��ͼ�񣬲����ţ�һ�֡����ֿɸ���������߾�����
src_img: ����ͼ��ָ�����飬2d�Դ棨��pitch����ַ
src_width: ����ͼ���
src_pitch: ����ͼ��pitch
sri_height: ����ͼ���
img_num: ����ͼ�����
dst_img: ���ͼ��ָ�룬2d�Դ棨��pitch����ַ
dst_width: ���ͼ���, ������src_width��������
dst_pitch: ���ͼ��pitch
dst_height: ���ͼ��ߣ�������src_height������������img_num*src_width*src_height<=dst_width*dst_height
            -img_num=2ʱ����dst_width=2*src_width, dst_height=src_height, ��Ϊ��һ����ƴ��
			-img_num=4ʱ����dst_width=2*src_width, dst_height=2*src_height, ��Ϊ�����ƴ��
			-��img_num*src_width*src_height<dst_width*dst_height����Ŀ��ͼ���������úڵ����
device_id: �Կ�id��Ĭ��ֵΪ0��
���أ� 0���ɹ���
      -1��ʧ��
*/
_declspec(dllexport) int merge_nv12(unsigned char **src_img, int src_width, int src_pitch, int src_height, int img_num, 
                        	         unsigned char *dst_img,  int dst_width, int dst_pitch, int dst_height, int device_id = 0);

/*
 * ���ܣ�����ͼ��
src_img: ����ͼ��ָ�룬2d�Դ棨��pitch����ַ
src_width: ����ͼ���
src_pitch: ����ͼ��pitch
sri_height: ����ͼ��� 
dst_img: ���ͼ��ָ�룬2d�Դ棨��pitch����ַ
dst_width: ���ͼ���
dst_pitch: ���ͼ��pitch
dst_height: ���ͼ��� 
device_id: �Կ�id ��Ĭ��ȡֵΪ0��
���أ�0���ɹ���
     -1��ʧ��
*/
_declspec(dllexport) int resize_nv12(unsigned char *src_img, int src_width, int src_pitch, int src_height,  
	                                 unsigned char *dst_img, int dst_width, int dst_pitch, int dst_height, int device_id = 0);

#endif
