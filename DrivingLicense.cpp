//
// Created by Nil on 2017/7/27.
//

#include "DrivingLicense.h"


DrivingLicense::DrivingLicense(Mat srcImage)
{
	this->srcImage = srcImage.clone();
	showImage = srcImage.clone();

	redMarkArea = new RedMarkArea(srcImage);

	// get rotated angle
	ANGLE = redMarkArea->getAngle();
	rotateImage(srcImage, this->srcImage, ANGLE);
	rotateImage(srcImage, showImage, ANGLE);
	redArea = redMarkArea->getRedRect();

	rightSideArea = getRightSideArea(redArea, RIGHT_WIDTH_RATIO);
	downSideArea = getDownSideArea(redArea, DOWN_WIDTH_RATIO, DOWN_HEIGHT_RATIO);
//	upSideArea = getUpSideArea(redArea, UP_WIDH_RATIO, UP_HEIGHT_RATIO);
//	upperSideArea = getUpperSideArea(upSideRect, UPPER_RATIO);
//	topSideArea = getTopSideArea(upperSideRect, TOP_RATIO);
//
//	imshow("right area", rightSideArea);
//	imshow("down area", downSideArea);
//	imshow("up area", upSideArea);
//	imshow("upper area", upperSideArea);
//	imshow("top area", topSideArea);
//	imshow("area", locateKeyword(downSideArea, 0, 0, 0.63, 1));
	imshow("all area", showImage);
}

Mat DrivingLicense::getRightSideArea(Rect redArea, float ratio)
{
	int redWidth = redArea.width;
	int redHeight = redArea.height;
	Point p1;
	p1.x = redArea.x + redWidth;
	p1.y = redArea.y;

	Point p2;
	p2.x = p1.x + redWidth * ratio;
	p2.y = p1.y + redHeight;

	// rectangle need to be corrected
	Rect rect = Rect(p1, p2);
	// set roi image
	Mat roi;
	roi = srcImage(rect);
	// draw area
	rectangle(showImage, rect, Scalar(0, 255, 0));
//	imshow("right area", showImage);
	return roi;
}

Mat DrivingLicense::getDownSideArea(Rect redArea, float widthRatio, float heightRatio)
{
	int redWidth = redArea.width;
	int redHeight = redArea.height;
	Point p1;
	p1.x = redArea.x;
	p1.y = redArea.y + redHeight;

	Point p2;
	p2.x = p1.x + redWidth * widthRatio;
	p2.y = p1.y + redHeight * heightRatio;

	Rect rect = Rect(p1, p2);
	Mat roi;
	roi = srcImage(rect);

	rectangle(showImage, rect, Scalar(255, 0, 255));
//	imshow("down area", showImage);
	return roi;
}

Mat DrivingLicense::getUpSideArea(Rect redArea, float widthRatio, float heightRatio)
{
	int redWidth = redArea.width;
	int redHeight = redArea.height;
	Point p1;
	p1.x = redArea.x;
	p1.y = redArea.y;

	Point p2;
	p2.x = p1.x + redWidth * widthRatio;
	p2.y = p1.y - redHeight * heightRatio;

	Rect rect = Rect(p1, p2);
	this->upSideRect = rect;
	Mat roi;
	roi = srcImage(rect);
	rectangle(showImage, rect, Scalar(255, 0, 0));
//	imshow("up area", showImage);
	return roi;
}

Mat DrivingLicense::getUpperSideArea(Rect upSideArea, float ratio)
{
	int width = upSideArea.width;
	int height = upSideArea.height;
	Point p1;
	p1.x = upSideArea.x;
	p1.y = upSideArea.y;

	Point p2;
	p2.x = p1.x + width;
	p2.y = p1.y - height * ratio;

	Rect rect = Rect(p1, p2);
	this->upperSideRect = rect;
	Mat roi;
	roi = srcImage(rect);
	rectangle(showImage, rect, Scalar(0, 255, 0));
//	imshow("upper area", showImage);
	return roi;
}

Mat DrivingLicense::getTopSideArea(Rect upperSideArea, float ratio)
{
	int width = upperSideArea.width;
	int height = upperSideArea.height;
	Point p1;
	p1.x = upperSideArea.x;
	p1.y = upperSideArea.y;

	Point p2;
	p2.x = p1.x + width;
	p2.y = p1.y - height * ratio;

	Rect rect = Rect(p1, p2);
	Mat roi;
	roi = srcImage(rect);
	rectangle(showImage, rect, Scalar(0, 0, 255));
//	imshow("top area", showImage);
	return roi;
}

void DrivingLicense::rotateImage(Mat src, Mat &img_rotate, float degree)
{
	//旋转中心为图像中心
	Point2f center;
	center.x = float(src.cols / 2.0);
	center.y = float(src.rows / 2.0);
//	int length = 0;
//	length = sqrt(src.cols * src.cols + src.rows * src.rows);
	//计算二维旋转的仿射变换矩阵
	Mat M = getRotationMatrix2D(center, degree, 1);
	warpAffine(src, img_rotate, M, Size(src.cols, src.rows), 1, 0, Scalar(255, 255, 255));//仿射变换，背景色填充为白色
//	imshow("rotate", img_rotate);
}

// 通过定位红色区域，确定信息位置。比例按照与红色区域的长宽比例进行偏移，裁剪
Mat DrivingLicense::locateKeyword(Mat roi, float widthOffsetRatio, float heightOffsetRatio, float widthRatio, float heightRatio)
{
	Mat keywordArea;
	int width = roi.cols;
	int height = roi.rows;
	Point p1;
	p1.x = width + width * widthOffsetRatio;
	p1.y = height + height * heightOffsetRatio;
	Point p2;
	p2.x = p1.x + width * widthRatio;
	p2.y = p1.y + height * heightRatio;
	return keywordArea;
}

void DrivingLicense::test()
{
	static const int DEFAULT_GAUSSIANBLUR_SIZE = 7;
	// SOBEL 算子 内核大小
	static const int SOBEL_SCALE = 1;
	static const int SOBEL_DELTA = 0;
	static const int SOBEL_DDEPTH = CV_16S;
	static const int SOBEL_X_WEIGHT = 0;
	static const int SOBEL_Y_WEIGHT = 1;


	Mat grayImage;
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);

//	// 高斯
//	Mat blurImage;
//	GaussianBlur(srcImage, blurImage, Size(DEFAULT_GAUSSIANBLUR_SIZE, DEFAULT_GAUSSIANBLUR_SIZE), 0, 0, BORDER_DEFAULT);


	// Sobel Gradient X
	Mat sobelImage, absSobelImage;
	Sobel(grayImage, sobelImage, SOBEL_DDEPTH, 1, 0, 3, SOBEL_SCALE, SOBEL_DELTA, BORDER_DEFAULT);
	convertScaleAbs(sobelImage, absSobelImage);
//	imshow("sobel x image", absSobelImage);

	// Sobel Gradient Y
	Mat sobelYImage, absSobelYImage;
	Sobel(grayImage, sobelYImage, SOBEL_DDEPTH, 0, 1, 3, SOBEL_SCALE, SOBEL_DELTA, BORDER_DEFAULT);
	convertScaleAbs(sobelYImage, absSobelYImage);
//	imshow("sobelY image", absSobelYImage);

	// Total Gradient
	Mat gradImage;
	addWeighted(absSobelImage, SOBEL_X_WEIGHT, absSobelYImage, SOBEL_Y_WEIGHT, 0, gradImage);
	imshow("grad image", gradImage);
}
