//
// Created by Nil on 2017/7/24.
//

#ifndef DRIVERLICENSEIDENTIFY_INFORMATIONLOCATE_H
#define DRIVERLICENSEIDENTIFY_INFORMATIONLOCATE_H

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class InformationLocate
{
protected:
	int imageWidth;
	int imageHeight;
	int borderWidth;
	int borderHeight;

	int errorAngle;
public:
	void setBorderWidth(int borderWidth);
	int getBorderWidth();
	void setBorderHeight(int borderHeight);
	int getBorderHeight();
	int getImageWidth();
	void setImageWidth(int imageWidth);
	int getImageHeight();
	void setImageHeight(int imageHeight);

	static const int DEFAULT_GAUSSIANBLUR_SIZE = 7;
	static const int SOBEL_SCALE = 1;
	static const int SOBEL_DELTA = 0;
	static const int SOBEL_DDEPTH = CV_16S;
	static const int SOBEL_X_WEIGHT = 0;
	static const int SOBEL_Y_WEIGHT = 1;
	static const int DEFAULT_MORPH_SIZE_WIDTH = 15;  // 11
	static const int DEFAULT_MORPH_SIZE_HEIGHT = 7;  // 7

	const float WIDTH_RATIO = 0.05;
	const float HEIGHT_RATIO = 0.045;
	const float SIZE_ERROR = 0.01;
	const float AREA_RATIO = 0.1;
	const float AREA_RATIO_ERROR = 0.05;


	InformationLocate();
	Mat OstuBeresenThreshold(Mat image);
	bool isBorder(RotatedRect candidate);
	int locateBorder(Mat image);
	int locateInfor(Mat image);
	bool isValidInfor(RotatedRect mr);
};


#endif //DRIVERLICENSEIDENTIFY_INFORMATIONLOCATE_H
