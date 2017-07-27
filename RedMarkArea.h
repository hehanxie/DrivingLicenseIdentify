//
// Created by Nil on 2017/7/27.
//

#ifndef DRIVERLICENSEIDENTIFY_REDMARKAREA_H
#define DRIVERLICENSEIDENTIFY_REDMARKAREA_H

#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;


class RedMarkArea
{
protected:
	Mat srcImage;
	int srcHeight;
	int srcWidth;
	Mat borderImage;
	int borderHeight;
	int borderWidth;

	int *horizontalArray;
	int startRow;
	int endRow;
	int height;

	int *verticalArray;
	int startCol;
	int endCol;
	int width;

	// 构成矩形的角度偏差
	static const int ANGLE_ERROR = 40;
	// 面积比例，偏差比例
	const float RED_AREA_RATIO = 0.05;
	const float RED_AREA_ERROR = 0.02;

public:
	RedMarkArea();

	RedMarkArea(Mat srcImage, Mat borderImage);

	// 颜色匹配（查找红色区域）
	void colorMatch();

	// 判断红色区域矩阵面积是否合理
	bool isRedArea(RotatedRect mr);

	// 判断红色区域矩阵角度是否合理
	bool isRedAreaAngle(RotatedRect mr);

	// 水平投影
	Mat getHorizontalProjection (Mat image);

	// 垂直投影
	Mat getVerticalProjection(Mat image);

};


#endif //DRIVERLICENSEIDENTIFY_REDMARKAREA_H
