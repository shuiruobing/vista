#ifndef MERGE_IMG_HEADER
#define MERGE_IMG_HEADER

/*
 * 功能：直接拼接图像，不缩放，一字、田字可根据输出宽、高决定。
src_img: 输入图像指针数组，2d显存（带pitch）地址
src_width: 输入图像宽
src_pitch: 输入图像pitch
sri_height: 输入图像高
img_num: 输入图像个数
dst_img: 输出图像指针，2d显存（带pitch）地址
dst_width: 输出图像宽, 必须是src_width的整数倍
dst_pitch: 输出图像pitch
dst_height: 输出图像高，必须是src_height的整数倍，且img_num*src_width*src_height<=dst_width*dst_height
            -img_num=2时，令dst_width=2*src_width, dst_height=src_height, 则为“一”字拼接
			-img_num=4时，令dst_width=2*src_width, dst_height=2*src_height, 则为“田”字拼接
			-若img_num*src_width*src_height<dst_width*dst_height，则目标图像多出区域用黑底填充
device_id: 显卡id（默认值为0）
返回： 0：成功，
      -1：失败
*/
_declspec(dllexport) int merge_nv12(unsigned char **src_img, int src_width, int src_pitch, int src_height, int img_num, 
                        	         unsigned char *dst_img,  int dst_width, int dst_pitch, int dst_height, int device_id = 0);

/*
 * 功能：缩放图像
src_img: 输入图像指针，2d显存（带pitch）地址
src_width: 输入图像宽
src_pitch: 输入图像pitch
sri_height: 输入图像高 
dst_img: 输出图像指针，2d显存（带pitch）地址
dst_width: 输出图像宽
dst_pitch: 输出图像pitch
dst_height: 输出图像高 
device_id: 显卡id （默认取值为0）
返回：0：成功，
     -1：失败
*/
_declspec(dllexport) int resize_nv12(unsigned char *src_img, int src_width, int src_pitch, int src_height,  
	                                 unsigned char *dst_img, int dst_width, int dst_pitch, int dst_height, int device_id = 0);

#endif
