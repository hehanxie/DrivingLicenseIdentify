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
	Mat srcImage;
	int WIDTH;
	int HEIGHT;
	// 用于显示的图像
	Mat showImage;

	// 红色水印区域
	RedMarkArea *redMarkArea;
	Rect redArea;
	float ANGLE;

	Mat topSideArea;
	Mat upperSideArea;
	Rect upperSideRect;
	Mat upSideArea;
	Rect upSideRect;
	Mat rightSideArea;
	Mat downSideArea;

	vector<Mat> keyMat;
	map<string, Mat> keyInformation;

	// 图像处理参数
	static const int DEFAULT_MORPH_SIZE_WIDTH = 15;  // 11
	static const int DEFAULT_MORPH_SIZE_HEIGHT = 7;  // 7

	// 系数比例
	const float TOP_HEIGHT_RATIO = 0.80;
	const float UPPER_HEIGHT_RATIO = 0.55;

	const float UP_WIDH_RATIO = 4;
	const float UP_HEIGHT_RATIO = 0.50;  // 0.85

	const float RIGHT_WIDTH_RATIO = 2;

	const float DOWN_WIDTH_RATIO = 3;
	const float DOWN_HEIGHT_RATIO = 0.25;


public:
	DrivingLicense(Mat src);
	void informationProcessing(vector<Mat> v);

	// get each part of area as Mat
	Mat getRightSideArea(Rect redArea, float ratio);
	Mat getDownSideArea(Rect redArea, float widthRatio, float heightRatio);
	Mat getUpSideArea(Rect redArea, float widthRatio, float heightRatio);
	Mat getUpperSideArea(Rect upSideArea, float ratio);
	Mat getTopSideArea(Rect upperSideArea, float ratio);

	// rotete image
	void rotateImage(Mat src, Mat &img_rotate, float angle);
	void correctRect(Rect &rect, float angle);
	// 通过定位红色区域，确定信息位置。比例按照与红色区域的长宽比例进行偏移，裁剪
	Mat areaDivide(Mat roi, float widthOffsetRatio, float heightOffsetRatio, float widthRatio, float heightRatio);
	// save all image into vector before dividing
	void getKeyInformation(vector<Mat> &v);
};


#endif //DRIVERLICENSEIDENTIFY_DRIVERLICENSE_H
