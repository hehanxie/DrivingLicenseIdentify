//
// Created by Nil on 2017/7/25.
//

#include "InforDetect.h"

void InforDetect::colorMatch(Mat src)
{
	int minRedH = 156;
	int maxRedH = 179;

	int minS = 43;
	int maxS = 255;
	int minV = 46;
	int maxV = 255;

//	Mat srcHSV;
//	// convert RGB to HSV
//	cvtColor(src, srcHSV, CV_BGR2HSV);
//	vector<Mat> hsvSplit;
//	split(srcHSV, hsvSplit);
//	equalizeHist(hsvSplit[2], hsvSplit[2]);
//	merge(hsvSplit, srcHSV);
//	Mat thresholdImage;
//	inRange(srcHSV, Scalar(0, minS, minV), Scalar(15, maxS, maxV), thresholdImage);
//	inRange(srcHSV, Scalar(minRedH, minS, minV), Scalar(maxRedH, maxS, maxV), thresholdImage);
//
//
//	//开操作 (去除一些噪点)
//	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
//	morphologyEx(thresholdImage, thresholdImage, MORPH_OPEN, element);
//
//	//闭操作 (连接一些连通域)
//	morphologyEx(thresholdImage, thresholdImage, MORPH_CLOSE, element);
//
//	imshow("threshold image", thresholdImage); //show the thresholded image

	waitKey(0);
}