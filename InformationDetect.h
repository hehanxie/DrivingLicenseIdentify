//
// Created by Nil on 2017/7/24.
//

#ifndef DRIVERLICENSEIDENTIFY_INFORMATIONLOCATE_H
#define DRIVERLICENSEIDENTIFY_INFORMATIONLOCATE_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include "RedMarkArea.h"

using namespace std;
using namespace cv;

class InformationDetect
{
protected:
	// 原始图像宽高
	int imageWidth;
	int imageHeight;
	// 证件边界宽高
	int borderWidth;
	int borderHeight;

public:
	InformationDetect();

	// 高斯模糊内核大小
	static const int DEFAULT_GAUSSIANBLUR_SIZE = 7;
	// SOBEL 算子 内核大小
	static const int SOBEL_SCALE = 1;
	static const int SOBEL_DELTA = 0;
	static const int SOBEL_DDEPTH = CV_16S;
	static const int SOBEL_X_WEIGHT = 0;
	static const int SOBEL_Y_WEIGHT = 1;

	static const int DEFAULT_MORPH_SIZE_WIDTH = 15;  // 11
	static const int DEFAULT_MORPH_SIZE_HEIGHT = 7;  // 7

	// 最大允许角度偏差
	const int ANGLE_ERROR = 45;
	// 单个字体宽高比例系数
	const float WIDTH_RATIO = 0.05;
	const float HEIGHT_RATIO = 0.045;
	// 面积比例系数
	const float SIZE_ERROR = 0.01;
	// 面积比例，误差
	const float AREA_RATIO = 0.1;
	const float AREA_RATIO_ERROR = 0.05;

	// 红色水印区域面积比例，偏差比例
	const float RED_AREA_RATIO = 0.05;
	const float RED_AREA_ERROR = 0.02;


	// 定位关键信息位置
	void locateInfor(Mat image);
	// 位置是否有效
	bool isValidInfor(RotatedRect mr);


};


#endif //DRIVERLICENSEIDENTIFY_INFORMATIONLOCATE_H
