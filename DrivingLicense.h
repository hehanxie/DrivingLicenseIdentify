//
// Created by Nil on 2017/7/27.
//

#ifndef DRIVERLICENSEIDENTIFY_DRIVERLICENSE_H
#define DRIVERLICENSEIDENTIFY_DRIVERLICENSE_H

#include <iostream>
#include "RedMarkArea.h"

using namespace std;
using namespace cv;


class DrivingLicense
{
protected:
	// 源图像，宽高
	Mat src_image_;
	int WIDTH_;
	int HEIGHT_;
	// 用于显示的图像
	Mat show_image_;

	// 红色水印区域
	RedMarkArea *red_mark_area_;
	Rect red_area_;
	float ANGLE_;

	Mat top_area_;
	Mat upper_area_;
	Rect upper_area_rect_;
	Mat up_area_;
	Rect up_area_rect_;
	Mat right_area_;
	Mat down_area_;

	vector<vector<Mat>> result_word_;

	// 图像处理参数
	static const int kDefaultMorphSizeWidth = 15;  // 11
	static const int kDefaultMorphSizeHeight = 7;  // 7

	// 系数比例
	const float kTopHeightRatio = 1.0;
	const float kUpperHeightRatio = 0.55;

	const float kUpWidthRatio = 4;
	const float kUpHeightRatio = 0.50;  // 0.85

	const float kRightWidthRatio = 2;

	const float kDownWidthRatio = 3;
	const float kDownHeightRatio = 0.25;

	const string PATH = "/Users/whstarlit/Documents/Projects/Git/DrivingLicense/testImg/data/";
	const string PREFIX[10] = {"birthday_", "firstIssue_", "classType_", "validTime_", "address1_",
								"address2_", "name_", "sex_", "nationality_", "driverID_"};

public:
	DrivingLicense(Mat src);

	// get each part of area as Mat
	Mat GetRightArea(Rect redArea, float ratio);
	Mat GetDownArea(Rect redArea, float widthRatio, float heightRatio);
	Mat GetUpArea(Rect redArea, float widthRatio, float heightRatio);
	Mat GetUpperArea(Rect upSideArea, float ratio);
	Mat GetTopArea(Rect upperSideArea, float ratio);

	// 旋转图像
	void RotateImage(Mat src, Mat &img_rotate, float angle);
	void CorrectRect(Rect &rect, float angle);
	// 通过定位红色区域，确定信息位置。比例按照与红色区域的长宽比例进行偏移，裁剪
	Mat AreaDivide(Mat roi, float widthOffsetRatio, float heightOffsetRatio, float widthRatio, float heightRatio);
	// 获取所有关键区域信息
	void GetKeyInformation(vector<vector<Mat>> &v);
	// 字符切割，并存入容器中
	vector<Mat> WordDivide(Mat image, string preStr);
	// 灰度化后的文字处理，使文字更加清晰
	void CharacterProcessing(vector<Mat> &v, string prefix);

	//  矩形重叠系数
	double RectOverLapCoefficient(Rect rect1, Rect rect2);
	// 判断两个矩形是否重叠
	bool IsRectOverLap(Rect rect1, Rect rect2);
	// 矩形合并
	Rect RectMerge(Rect rect1, Rect rect2);
};


#endif //DRIVERLICENSEIDENTIFY_DRIVERLICENSE_H
