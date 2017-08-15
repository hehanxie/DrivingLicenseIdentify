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
//	if (!redMarkArea->isFindRedArea)
//	{
//		return ;
//	}
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
//	imshow("red area", showImage);

	// put all area into keyMat vector
	getKeyInformation(keyMat);
//	// divide each part
	informationProcessing(keyMat);

//	imshow("draw all area", showImage);
}

void DrivingLicense::informationProcessing(vector<Mat> v)
{
	string PATH = "/Users/whstarlit/Documents/Projects/Git/DrivingLicense/testImg/data/";
	// get each part key into map

	// right area
//	Mat birthday   = areaDivide(v[0], 0.3,  0,    0.7, 0.3);
//	Mat firstIssue = areaDivide(v[0], 0.45, 0.33, 0.5, 0.3);
//	Mat class      = areaDivide(v[0], 0.30, 0.67, 0.7, 0.3);
//	imshow("birthday", birthday);
//	imshow("first issue", firstIssue);
//	imshow("class", class);
//
//	// down area
//	Mat validTime = areaDivide(v[1], 0.28, 0, 0.72, 1);
//	imshow("valid time", validTime);
//
//	// up area
	Mat address1 = areaDivide(v[2], 0.1, 0, 0.9, 0.5);
//	Mat address2 = areaDivide(v[2], 0.05, 0.5, 0.95, 0.5);

//	imshow("address", address);
//
//	// upper area
//	Mat name = areaDivide(v[3], 0.08, 0, 0.35, 1);
//	imwrite(PATH + "name.png", name);
//	imshow("name", name);

//	Mat sex  = areaDivide(v[3], 0.56, 0, 0.10, 1);
//	imwrite(PATH + "sex.png", sex);
//	imshow("sex", sex);

// 	Mat nationality = areaDivide(v[3], 0.77, 0, 0.20, 1);
//	imwrite(PATH + "nationality.png", nationality);
//	imshow("nationality", nationality);

//	// top area
//	Mat driverID = areaDivide(v[4], 0.41, 0, 0.45, 1);
//	imwrite(PATH + "driverID.png", driverID);
//	imshow("driver id", driverID);

	wordDivide(address1);
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
	cout << "y offset: " << sinf(radian) * 0.5 * HEIGHT << endl;
//	cout << "cos: " << cosf(radian) << endl;
	rect.y += 0.7 * sinf(radian) * HEIGHT;
}

Mat DrivingLicense::areaDivide(Mat roi, float widthOffsetRatio, float heightOffsetRatio, float widthRatio, float heightRatio)
{
	// image deal and
	Mat dst;
	cvtColor(roi, dst, CV_BGR2GRAY);
	threshold(dst, roi, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	Mat element = getStructuringElement(MORPH_RECT, Size(2, 2));
	morphologyEx(roi, roi, MORPH_OPEN, element);

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
	cout << Rect(p1, p2) << endl;
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

void DrivingLicense::wordDivide(Mat image)
{
	int height = image.rows;
	imshow("image", image);
	Mat drawImage;
	cvtColor(image, drawImage, CV_GRAY2BGR);
	image = image < 100;
	vector<vector<Point>> contours;
	findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	cout << "contours: " << contours.size() << endl;
	Mat t = drawImage.clone();
	drawContours(t, contours, -1, Scalar(0, 0, 255), 1);
	imshow("pre draw", t);
//	RotatedRect rect;
//	for (int i = 0; i < contours.size(); i++)
//	{
//		rect = minAreaRect(contours[i]);
//		if (rect.size.width * rect.size.height >= height * height * 0.1)
//		{
//			Point2f P[4];
//			rect.points(P);
//			for (int j = 0; j <= 3; j++)
//			{
//				line(drawImage, P[j], P[(j + 1) % 4], Scalar(0, 0, 255), 2);
//			}
//		}
//	}

	Rect rect1, rect2;
	double rate;
	for (int i = 1; i < contours.size(); i++)
	{
		rect1 = boundingRect(contours[i-1]);
		rect2 = boundingRect(contours[i]);
		if (rect1.width * rect1.height >= height * height * 0.1 &&
			rect2.width * rect2.height >= height * height * 0.1)
		{
			rate = RectOverLapCoefficient(rect1, rect2);
			cout << "rate: " << rate << endl;
			if (rate >= 0.2)
			{
//				contours.push_back(RectMerge(rect1, rect2));
//				cout << "merge: " << i << endl;
				rectangle(drawImage, RectMerge(rect1, rect2), Scalar(0, 0, 255));
			}
			else
			{
				rectangle(drawImage, rect1, Scalar(0, 0, 255));
				rectangle(drawImage, rect2, Scalar(0, 0, 255));
			}
		}
	}

	imshow("draw contours", drawImage);
}

double DrivingLicense::RectOverLapCoefficient(Rect rect1, Rect rect2)
{
	CV_Assert(rect1.area() != 0 && rect2.area() != 0);
	double rate = 0.0;
	int flag = isRectOverLap(rect1, rect2);
	if (flag == false)
	{
		rate = 0.0;
		return 0;
	}
	Rect rectMerge;
	rectMerge = RectMerge(rect1, rect2);
	Mat dst(Size(rectMerge.width + rectMerge.x, rectMerge.height + rectMerge.y), CV_8UC1, Scalar(0));
	int maxArea = rect1.area() > rect2.area() ? rect1.area() : rect2.area();
	if (maxArea == rect1.area())
	{
		dst(rect2) = Scalar::all(0);
		dst(rect1) = Scalar::all(127);
		int count = countNonZero(dst(rect2));
		rate = (double)count / (double)rect2.area();
	}
	else
	{
		dst(rect1) = cv::Scalar::all(0);
		dst(rect2) = cv::Scalar::all(127);
		int count = countNonZero(dst(rect1));
		rate = (double)count / (double)rect1.area();
	}
	return rate;
}

bool DrivingLicense::isRectOverLap(Rect rect1, Rect rect2)
{
	//先算包含情况
	int x1 = rect1.x;
	int x2 = rect2.x;
	int y1 = rect1.y;
	int y2 = rect2.y;
	int width1 = rect1.width;
	int width2 = rect2.width;
	int height1 = rect1.height;
	int height2 = rect2.height;
	if (x2 > x1 + width1 || x2 + width2 < x1 || y2 + height2 < y1 || y2 > y1 + height1)
	{
		return false;
	}
	if (x1 > x2 + width2 || x1 + width1 < x2 || y1 + height1 < y2 || y1 > y2 + height2)
	{
		return false;
	}
	return true;
}

Rect DrivingLicense::RectMerge(Rect rect1, Rect rect2)
{
	Rect rectM;
	rectM.x = min(rect1.x, rect2.x);
	rectM.y = min(rect1.y, rect2.y);
	rectM.width = max(rect1.x + rect1.width, rect2.x + rect2.width) - rectM.x;
	rectM.height = max(rect1.y + rect1.height, rect2.y + rect2.height) - rectM.y;
	return rectM;
}