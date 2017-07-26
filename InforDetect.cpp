//
// Created by Nil on 2017/7/25.
//

#include "InforDetect.h"

void InforDetect::colorMatch(Mat src)
{
	int minRedH = 140;
	int maxRedH = 180;

	int minS = 40;
	int maxS = 255;
	int minV = 40;
	int maxV = 255;

	Mat srcHSV;
//	 convert RGB to HSV
	cvtColor(src, srcHSV, CV_BGR2HSV);
	vector<Mat> hsvSplit;
	split(srcHSV, hsvSplit);
	equalizeHist(hsvSplit[2], hsvSplit[2]);
	merge(hsvSplit, srcHSV);
	Mat thresholdImage;
//	inRange(srcHSV, Scalar(0, minS, minV), Scalar(15, maxS, maxV), thresholdImage);
	inRange(srcHSV, Scalar(minRedH, minS, minV), Scalar(maxRedH, maxS, maxV), thresholdImage);


	//开操作 (去除一些噪点)
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(thresholdImage, thresholdImage, MORPH_OPEN, element);

	//闭操作 (连接一些连通域)
	morphologyEx(thresholdImage, thresholdImage, MORPH_CLOSE, element);

	imshow("threshold image", thresholdImage); //show the thresholded image

	vector<vector<Point>> contours;
	findContours(thresholdImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	vector<vector<Point> >::iterator itc = contours.begin();
	while (itc != contours.end())
	{
		RotatedRect mr = minAreaRect(Mat(*itc));
		if (1)
		{
			Point2f P[4];
			mr.points(P);
			for(int j = 0; j <= 3; j++)
			{
				line(src, P[j], P[(j + 1) % 4], Scalar(0, 0, 255), 2);
			}
		}
		itc++;
	}

	cout << "total: " << contours.size() << endl;
	imshow("draw red", src);

	// src image must be CV_8UC1
//	Mat dst;
//	adaptiveThreshold(src, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 7, 7);
//	imshow("adaptive threshold", dst);


	waitKey(0);
}
void InforDetect::setROI(Mat src)
{
	// set roi area, change roi will affect src image
	Mat srcImage = src;
	// set roi
	Mat srcROI = srcImage(Rect(0, 0, srcImage.cols / 2, srcImage.rows / 2));
	// draw rectangle on roi
	rectangle(srcImage, Rect(5, 5, 50, 50), Scalar(0, 0, 255), 2);
}
