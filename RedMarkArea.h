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
	Mat src_image_;
	int HEIGHT_;
	int WIDTH_;

	// 用于显示的图像
	Mat show_image_;
	// 红色区域矩阵
	Rect red_rect_;
	// 红色区域图像
	Mat red_image_;

	// 红色区域标识
	bool is_find_red_area_;

	// 水平投影数组
	int *horizontal_array_;
	int start_row_;
	int end_row_;

	// 垂直投影数组
	int *vertical_array_;
	int start_col_;
	int end_col_;

	//图像偏转角度
	float ANGLE_;
	// 构成矩形的角度偏差
	static const int kAngleError = 40;
	// 面积比例，偏差比例
	const float kMinRedAreaRatio = 0.03;
	const float kMaxRedAreaRatio = 0.09;
	const float kMinScale = 0.8;
	const float kMaxScale = 1.25;

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

};


#endif //DRIVERLICENSEIDENTIFY_REDMARKAREA_H
