//
// Created by Nil on 2017/7/27.
//

#include "DrivingLicense.h"


DrivingLicense::DrivingLicense(Mat src)
{
	this->srcImage = src.clone();
	HEIGHT = srcImage.rows;
	WIDTH = srcImage.cols;

	redMarkArea = new RedMarkArea(src);
	if (!redMarkArea->isFindRedArea)
	{
		return ;
	}
	// get rotated angle
	ANGLE = redMarkArea->getAngle();
	rotateImage(src, this->srcImage, ANGLE);
	showImage = this->srcImage.clone();

	// get red mark area
	redArea = redMarkArea->getRedRect();
	// correct angle
	correctRect(redArea, ANGLE);
	// draw red area to show
	rectangle(showImage, redArea, Scalar(0, 255, 0));

	// put all area into keyMat vector
	getKeyInformation(keyMat);
//	// divide each part
//	informationProcessing(keyMat);

	imshow("draw all area", showImage);
}

void DrivingLicense::informationProcessing(vector<Mat> v)
{
	// get each part key into map

	// right area
	Mat birthdayArea   = areaDivide(v[0], 0.3,  0,    0.7, 0.3);
	Mat firstIssueArea = areaDivide(v[0], 0.45, 0.33, 0.5, 0.3);
	Mat classArea      = areaDivide(v[0], 0.30, 0.67, 0.7, 0.3);
	imshow("birthday", birthdayArea);
	imshow("first issue", firstIssueArea);
	imshow("class", classArea);

	// down area
	Mat validTimeArea = areaDivide(v[1], 0.28, 0, 0.72, 1);
	imshow("valid time", validTimeArea);

	// up area
	Mat addressArea = areaDivide(v[2], 0.1, 0, 0.9, 1);
	imshow("address", addressArea);

	// upper area
	Mat nameArea   		= areaDivide(v[3], 0.08, 0, 0.35, 1);
	Mat sexArea 		= areaDivide(v[3], 0.56, 0, 0.10, 1);
	Mat nationalityArea = areaDivide(v[3], 0.80, 0, 0.20, 1);
	imshow("name", nameArea);
	imshow("sex", sexArea);
	imshow("nationality", nationalityArea);

//	// top area
	Mat driverID = areaDivide(v[4], 0.41, 0, 0.45, 1);
	imshow("driver id", driverID);
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

void DrivingLicense::rotateImage(Mat src, Mat &img_rotate, float angle)
{
	//旋转中心为图像中心
	Point2f center;
	center.x = float(src.cols / 2.0);
	center.y = float(src.rows / 2.0);
//	int length = 0;
//	length = sqrt(src.cols * src.cols + src.rows * src.rows);
	//计算二维旋转的仿射变换矩阵
	Mat M = getRotationMatrix2D(center, angle, 1);
	// negative num means rotate clockwise
	warpAffine(src, img_rotate, M, Size(src.cols, src.rows), 1, 0, Scalar(255, 255, 255));//仿射变换，背景色填充为白色
//	imshow("rotate", img_rotate);
	// Move image
}

void DrivingLicense::correctRect(Rect &rect, float angle)
{
	float radian = angle * CV_PI / 180;
	cout << "y offset: " << sinf(radian) * 0.7 * HEIGHT << endl;
//	cout << "cos: " << cosf(radian) << endl;
	rect.y += 0.7 * sinf(radian) * HEIGHT;
}

Mat DrivingLicense::areaDivide(Mat roi, float widthOffsetRatio, float heightOffsetRatio, float widthRatio, float heightRatio)
{
	// image deal and
	Mat dst;
	cvtColor(roi, dst, CV_BGR2GRAY);
	threshold(dst, dst, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	Mat element = getStructuringElement(MORPH_RECT, Size(2, 2));
	morphologyEx(dst, roi, MORPH_OPEN, element);

//	imshow("valid", roi);

	Mat keywordArea;
	int width = roi.cols;
	int height = roi.rows;
	Point p1;
	p1.x = width * widthOffsetRatio;
	p1.y = height * heightOffsetRatio;
	Point p2;
	p2.x = p1.x + width * widthRatio;
	p2.y = p1.y + height * heightRatio;

	keywordArea = roi(Rect(p1, p2));
//	imshow("keyword area", keywordArea);
	return keywordArea;
}

void DrivingLicense::getKeyInformation(vector<Mat> &v)
{
	// get each part of key, and set as roi
	rightSideArea = getRightSideArea(redArea, RIGHT_WIDTH_RATIO);
	downSideArea = getDownSideArea(redArea, DOWN_WIDTH_RATIO, DOWN_HEIGHT_RATIO);
	upSideArea = getUpSideArea(redArea, UP_WIDH_RATIO, UP_HEIGHT_RATIO);
	upperSideArea = getUpperSideArea(upSideRect, UPPER_HEIGHT_RATIO);
	topSideArea = getTopSideArea(upperSideRect, TOP_HEIGHT_RATIO);

	v.push_back(rightSideArea);
	v.push_back(downSideArea);
	v.push_back(upSideArea);
	v.push_back(upperSideArea);
	v.push_back(topSideArea);
}

