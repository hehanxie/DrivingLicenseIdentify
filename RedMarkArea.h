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
public:
	// 源图像，宽高
	Mat srcImage;
	int HEIGHT;
	int WIDTH;

	// 用于显示的图像
	Mat showImage;
	// 红色区域矩阵
	Rect redRect;
	// 红色区域图像
	Mat redImage;

	// 红色区域标识
	bool isFindRedArea;

	// 水平投影数组
	int *horizontalArray;
	int startRow;
	int endRow;

	// 垂直投影数组
	int *verticalArray;
	int startCol;
	int endCol;

	//图像偏转角度
	float ANGLE;
	// 构成矩形的角度偏差
	static const int ANGLE_ERROR = 40;
	// 面积比例，偏差比例
	const float MIN_RED_AREA_RATIO = 0.03;
	const float MAX_RED_AREA_RATIO = 0.09;
	const float MIN_SCALE = 0.8;
	const float MAX_SCALE = 1.25;

public:
	RedMarkArea();

	RedMarkArea(Mat src);

	// 颜色匹配（查找红色区域）
	void colorMatch();

	// 判断红色区域矩阵面积是否合理
	bool isRedArea(Rect mr);

	// 水平投影(Y)
	Mat getHorizontalProjection (Mat image);

	// 垂直投影(X)
	Mat getVerticalProjection(Mat image);

	// 计算红色区域位置
	void setRedSize();

	// 检测直线，计算偏转角度
	void lineDetect();
	// 弧度转换为角度
	float degreeTrans(float theta);

	void setRedRect(Rect rect);
	Rect getRedRect();
	void setAngle(float angle);
	float getAngle();

};


#endif //DRIVERLICENSEIDENTIFY_REDMARKAREA_H
