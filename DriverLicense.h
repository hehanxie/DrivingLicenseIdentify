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

	// 图像处理参数
	static const int DEFAULT_MORPH_SIZE_WIDTH = 15;  // 11
	static const int DEFAULT_MORPH_SIZE_HEIGHT = 7;  // 7

public:
	DriverLicense(Mat srcImage);

	// 定位边界
	void borderDetect();

	// 判断是否为边界
	bool isBorder(RotatedRect candidate);
};


#endif //DRIVERLICENSEIDENTIFY_DRIVERLICENSE_H
