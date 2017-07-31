//
// Created by Nil on 2017/7/27.
//

#ifndef DRIVERLICENSEIDENTIFY_DRIVERLICENSE_H
#define DRIVERLICENSEIDENTIFY_DRIVERLICENSE_H

#include <iostream>
#include "RedMarkArea.h"

using namespace std;
using namespace cv;


class DriverLicense
{
protected:
	Mat srcImage;
	Mat borderImage;

	RedMarkArea *redMarkArea;
	Rect redArea;

	Rect topSideArea;
	Rect upperSideArea;
	Rect upSideArea;
	Rect rightSideArea;
	Rect downSideArea;

	// 图像处理参数
	static const int DEFAULT_MORPH_SIZE_WIDTH = 15;  // 11
	static const int DEFAULT_MORPH_SIZE_HEIGHT = 7;  // 7

	// 系数比例
	const float TOP_RATIO = 1;
	const float UPPER_RATIO = 0.5;

	const float UP_WIDH_RATIO = 4;
	const float UP_HEIGHT_RATIO = 0.57;  // 0.85

	const float RIGHT_WIDTH_RATIO = 2.05;

	const float DOWN_WIDTH_RATIO = 3.05;
	const float DOWN_HEIGHT_RATIO = 0.27;


public:
	DriverLicense(Mat srcImage);

	Rect getTopSideArea(Rect upperSideArea, float ratio);
	Rect getUpperSideArea(Rect upSideArea, float ratio);
	Rect getUpSideArea(Rect redArea, float widthRatio, float heightRatio);
	Rect getRightSideArea(Rect redArea, float ratio);
	Rect getDownSideArea(Rect redArea, float widthRatio, float heightRatio);

	Mat locateKeyword(Rect rect, float widthOffsetRatio, float heightOffsetRatio, float widthRatio, float heightRatio);

	// 定位边界
	void borderDetect();

	// 判断是否为边界
	bool isBorder(RotatedRect candidate);
};


#endif //DRIVERLICENSEIDENTIFY_DRIVERLICENSE_H
