//
// Created by Nil on 2017/7/27.
//

#include "DrivingLicense.h"


DrivingLicense::DrivingLicense(Mat srcImage)
{
	this->srcImage = srcImage.clone();
//	borderDetect();

	this->borderImage = srcImage.clone();
	redMarkArea = new RedMarkArea(srcImage, borderImage);

	redArea = redMarkArea->getRedRect();

//	rightSideArea = getRightSideArea(redArea, RIGHT_WIDTH_RATIO);
//	downSideArea = getDownSideArea(redArea, DOWN_WIDTH_RATIO, DOWN_HEIGHT_RATIO);
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
//	imshow("all area", borderImage);
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
	rectangle(borderImage, rect, Scalar(0, 255, 0));
//	imshow("right area", borderImage);
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
	rectangle(borderImage, rect, Scalar(255, 0, 255));
//	imshow("down area", borderImage);
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
	rectangle(borderImage, rect, Scalar(255, 0, 0));
//	imshow("up area", borderImage);
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
	rectangle(borderImage, rect, Scalar(0, 255, 0));
//	imshow("upper area", borderImage);
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
	rectangle(borderImage, rect, Scalar(0, 0, 255));
//	imshow("top area", borderImage);
	return roi;
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
	Mat dst = image.clone();
	// meanshift to improve contrast ratio, but inefficient
//	pyrMeanShiftFiltering(image, dst, 10, 10);

	blur(image, image, Size(3, 3));
//	imshow("blur", image);

	Mat cannyImage;
	float minThreshold = 70;
	Canny(image, cannyImage, minThreshold, 3 * minThreshold);
	threshold(cannyImage, dst, 0, 255, CV_THRESH_BINARY);
//	imshow("canny", dst);

	// 闭操作 (连接一些连通域)
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	morphologyEx(dst, dst, MORPH_CLOSE, element);
//	imshow("close", dst);

	// draw line and corner and calculate angle
	vector<Vec4i> lines;
	double theta = CV_PI / 180;
	int lineThreshold = 80;
	double minLineLength = srcImage.rows > srcImage.cols ? srcImage.cols : srcImage.rows * 0.5;
	double maxLineGap = 10;
	HoughLinesP(dst, lines, 1, theta, lineThreshold, minLineLength * 0.5, maxLineGap);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(srcImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 0, 0), 2, CV_AA);
	}
	imshow("src", srcImage);


//	// dras contours
//	vector<vector<Point> > contours;
//	findContours(dst, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//	vector<vector<Point> >::iterator itc = contours.begin();
//	drawContours(srcImage, contours, -1, Scalar(0, 0, 255));
//	imshow("draw", srcImage);



//	int index = 0;
//	bool isFindBorder = false;
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
//			isFindBorder = true;
//			break;
//		}
//		itc++;
//		index++;
//	}
//	if (!isFindBorder)
//	{
//		this->borderImage = srcImage.clone();
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
