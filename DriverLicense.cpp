//
// Created by Nil on 2017/7/27.
//

#include "DriverLicense.h"


DriverLicense::DriverLicense(Mat srcImage)
{
	this->srcImage = srcImage.clone();
	borderDetect();
	redMarkArea = new RedMarkArea(srcImage, borderImage);

};


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