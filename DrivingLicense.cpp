//
// Created by Nil on 2017/7/27.
//

#include "DrivingLicense.h"


DrivingLicense::DrivingLicense(Mat srcImage)
{
	this->srcImage = srcImage.clone();
	borderDetect();

	// redMarkArea = new RedMarkArea(srcImage, borderImage);

	// redArea = redMarkArea->getRedArea();
	// rightSideArea = getRightSideArea(redArea, RIGHT_WIDTH_RATIO);
	// downSideArea = getDownSideArea(redArea, DOWN_WIDTH_RATIO, DOWN_HEIGHT_RATIO);
	// upSideArea = getUpSideArea(redArea, UP_WIDH_RATIO, UP_HEIGHT_RATIO);
	// upperSideArea = getUpperSideArea(upSideArea, UPPER_RATIO);
	// topSideArea = getTopSideArea(upperSideArea, TOP_RATIO);

//	imshow("draw area", borderImage);
}

Rect DrivingLicense::getRightSideArea(Rect redArea, float ratio)
{
	int redWidth = redArea.width;
	int redHeight = redArea.height;
	Point p1;
	p1.x = redArea.x + redWidth;
	p1.y = redArea.y;

	Point p2;
	p2.x = p1.x + redWidth * ratio;
	p2.y = p1.y + redHeight;

	Rect rect = Rect(p1, p2);
	rectangle(borderImage, rect, Scalar(255, 0, 0));
//	imshow("right area", borderImage);
	return rect;
}

Rect DrivingLicense::getTopSideArea(Rect upperSideArea, float ratio)
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
	rectangle(borderImage, rect, Scalar(0, 0, 255));
//	imshow("top area", borderImage);
	return rect;
}

Rect DrivingLicense::getUpperSideArea(Rect upSideArea, float ratio)
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
	rectangle(borderImage, rect, Scalar(0, 255, 0));
//	imshow("upper area", borderImage);
	return rect;
}

Rect DrivingLicense::getUpSideArea(Rect redArea, float widthRatio, float heightRatio)
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
	rectangle(borderImage, rect, Scalar(255, 0, 0));
//	imshow("up area", borderImage);
	return rect;
}

Rect DrivingLicense::getDownSideArea(Rect redArea, float widthRatio, float heightRatio)
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
	rectangle(borderImage, rect, Scalar(255, 0, 255));
//	imshow("down area", borderImage);
	return rect;
}

// 通过定位红色区域，确定信息位置。比例按照与红色区域的长宽比例进行偏移，裁剪
Mat DrivingLicense::locateKeyword(Rect rect, float widthOffsetRatio, float heightOffsetRatio, float widthRatio, float heightRatio)
{
	int width = rect.x;
	int height = rect.y;
	Point p1;
	p1.x = width + width * widthOffsetRatio;
	p1.y = height + height * heightOffsetRatio;
	Point p2;
	p2.x = p1.x + width * widthRatio;
	p2.y = p1.y + height * heightRatio;

}

void DrivingLicense::borderDetect()
{
	/*
	 * canny, find border 
	*/
	Mat image = srcImage.clone();
	if (image.empty())
	{
		cout << "error";
		return;
	}
	Mat dst;
	pyrMeanShiftFiltering(image, dst, 10, 10);
	// convert to gray
	cvtColor(dst, image, COLOR_BGR2GRAY);
	blur(image, image, Size(3, 3));
	threshold(image, image, 0, 255, CV_THRESH_OTSU+CV_THRESH_BINARY);
	imshow("blur", image);

	Mat cannyImage;
	float minThreshold = 40;
	Canny(image, cannyImage, minThreshold, 3 * minThreshold);
	threshold(cannyImage, dst, 0, 255, CV_THRESH_BINARY);

//	pyrMeanShiftFiltering(image, dst, 10, 10);
	imshow("filter", dst);


//	// 画边缘
//	vector<vector<Point> > contours;
//	findContours(dst, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//	vector<vector<Point> >::iterator itc = contours.begin();
//	drawContours(srcImage, contours, -1, Scalar(0, 0, 255));
//	imshow("draw", srcImage);



//	int index = 0;
//	while (itc != contours.end())
//	{
//		// 获取每个轮廓的最小有界矩形区域
//		RotatedRect mr = minAreaRect(Mat(*itc));
//		if (isBorder(mr))
//		{
//			Point2f P[4];
//			mr.points(P);
//			for (int j = 0; j <= 3; j++)
//			{
//				line(srcImage, P[j], P[(j + 1) % 4], Scalar(0, 0, 255), 2);
//			}
//
//			// set border roi region
//			this->borderImage = srcImage(Rect(P[2], P[0]));
//			cout << "border width: " << mr.size.width << endl;
//			cout << "border height: " << mr.size.height << endl;
//			imshow("border image", borderImage);
//			break;
//		}
//		itc++;
//		index++;
//	}

}

bool DrivingLicense::isBorder(RotatedRect candidate)
{
	// get area
	int area = candidate.size.height * candidate.size.width;
	int rArea = srcImage.rows * srcImage.cols * 0.5;

	if (area > rArea)
	{
		return true;
	}
	else
	{
		return false;
	}
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