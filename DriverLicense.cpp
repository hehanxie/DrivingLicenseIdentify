//
// Created by Nil on 2017/7/27.
//

#include "DriverLicense.h"


DriverLicense::DriverLicense(Mat srcImage)
{
	this->srcImage = srcImage.clone();
	borderDetect();
	redMarkArea = new RedMarkArea(srcImage, borderImage);

	redArea = redMarkArea->getRedArea();
	rightSideArea = getRightSideArea(redArea, RIGHT_WIDTH_RATIO);
	downSideArea = getDownSideArea(redArea, DOWN_WIDTH_RATIO, DOWN_HEIGHT_RATIO);
	upSideArea = getUpSideArea(redArea, UP_WIDH_RATIO, UP_HEIGHT_RATIO);
	upperSideArea = getUpperSideArea(upSideArea, UPPER_RATIO);

	imshow("draw area", borderImage);
};

Rect DriverLicense::getRightSideArea(Rect redArea, float ratio)
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


Rect DriverLicense::getUpperSideArea(Rect upSideArea, float ratio)
{
	int redWidth = upSideArea.width;
	int redHeight = upSideArea.height;
	Point p1;
	p1.x = upSideArea.x;
	p1.y = upSideArea.y;

	Point p2;
	p2.x = p1.x + redWidth;
	p2.y = p1.y - redHeight * 0.5;

	Rect rect = Rect(p1, p2);
	rectangle(borderImage, rect, Scalar(0, 255, 0));
//	imshow("upper area", borderImage);
	return rect;
}

Rect DriverLicense::getUpSideArea(Rect redArea, float widthRatio, float heightRatio)
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
	rectangle(borderImage, rect, Scalar(0, 255, 0));
//	imshow("up area", borderImage);
	return rect;
}

Rect DriverLicense::getDownSideArea(Rect redArea, float widthRatio, float heightRatio)
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

void DriverLicense::borderDetect()
{
	Mat image = srcImage.clone();

	cvtColor(image, image, COLOR_BGR2GRAY);
	if (srcImage.empty())
	{
		cout << "error";
		return ;
	}

	Mat dst;
	adaptiveThreshold(image, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 7, 7);
//	imshow("ostu beresen image", ostuBeresenImage);

	// closed 处理
	Mat element = getStructuringElement(MORPH_RECT ,Size(DEFAULT_MORPH_SIZE_WIDTH ,DEFAULT_MORPH_SIZE_WIDTH));
	Mat closedImage;
	//进行形态学操作
	morphologyEx(dst, closedImage, MORPH_CLOSE, element);
//	morphologyEx(binaryImage, openImage, MORPH_OPEN, element);
//	imshow("closed image", closedImage);

	// 膨胀处理
	Mat dilateImage;
	dilate(closedImage, dilateImage, element);
//	imshow("dilate image", dilateImage);

	// 画边缘
	vector<vector<Point> > contours;
	findContours(dilateImage, contours, CV_RETR_EXTERNAL , CV_CHAIN_APPROX_NONE);
	vector< vector <Point> > ::iterator itc = contours.begin();

	int index = 0;
	while( itc != contours.end())
	{
		// 获取每个轮廓的最小有界矩形区域
		RotatedRect mr = minAreaRect(Mat(*itc));
		if (isBorder(mr))
		{
			Point2f P[4];
			mr.points(P);
			for(int j = 0; j <= 3; j++)
			{
				line(srcImage, P[j], P[(j + 1) % 4], Scalar(0, 0, 255), 2);
			}

			borderImage = srcImage(Rect(P[2], P[0]));
			cout << "border width: " << mr.size.width << endl;
			cout << "border height: " << mr.size.height << endl;
			break;
		}
		itc++;
		index++;
	}
//	imshow("border image", borderImage);
}

bool DriverLicense::isBorder(RotatedRect candidate)
{
	// get area
	int area = candidate.size.height * candidate.size.width;
	int rArea = srcImage.rows * srcImage.cols * 0.5;

//	if (area > 130000)
	if (area > rArea)
	{
		return true;
	}
	else
	{
		return false;
	}
}