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
	float ANGLE_;
	// 构成矩形的角度偏差
	static const int kAngleError = 40;
	// 面积比例，偏差比例
	const float kMinRedAreaRatio = 0.03;
	const float kMaxRedAreaRatio = 0.09;
	const float kMinScale = 0.8;
	const float kMaxScale = 1.25;
	const float kBlackRatio = 0.05;

public:
	RedMarkArea();

	RedMarkArea(Mat src);

	// 颜色匹配（查找红色区域）
	void ColorMatch();

	// 判断红色区域矩阵面积是否合理
	bool IsRedArea(Rect mr);

	// 水平投影(Y)
	Mat GetHorizontalProjection(Mat image);

	// 垂直投影(X)
	Mat GetVerticalProjection(Mat image);

	// 计算红色区域位置
	void SetRedSize();

	// 检测直线，计算偏转角度
	void LineDetect();
	// 弧度转换为角度
	float DegreeTrans(float theta);

	void SetRedRect(Rect rect);
	Rect GetRedRect();
	void SetAngle(float angle);
	float GetAngle();
	Mat GetCorrectSrcImage();

	void RotateImage(Mat src, Mat &img_rotate, float angle);
};


#endif //DRIVERLICENSEIDENTIFY_REDMARKAREA_H
