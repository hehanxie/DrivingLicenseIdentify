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
	Rect redRect;
	// gray image with 8UC1
	Mat redImage;

	Mat srcImage;
	int HEIGHT;
	int WIDTH;
	float ANGLE;

	Mat showImage;


	int *horizontalArray;
	int startRow;
	int endRow;

	int *verticalArray;
	int startCol;
	int endCol;

	// 构成矩形的角度偏差
	static const int ANGLE_ERROR = 40;
	// 面积比例，偏差比例
	const float RED_AREA_RATIO = 0.05;
	const float RED_AREA_ERROR = 0.02;

public:
	RedMarkArea();

	RedMarkArea(Mat src);

	// 颜色匹配（查找红色区域）
	void colorMatch();

	// 判断红色区域矩阵面积是否合理
	bool isRedArea(RotatedRect mr);

	// 水平投影
	Mat getHorizontalProjection (Mat image);

	// 垂直投影
	Mat getVerticalProjection(Mat image);

	// 计算红色区域位置
	void setRedSize();

	void setRedRect(Rect rect);
	Rect getRedRect();
	void lineDetect();
	void setAngle(float angle);
	float getAngle();
	float degreeTrans(float theta);
};


#endif //DRIVERLICENSEIDENTIFY_REDMARKAREA_H
