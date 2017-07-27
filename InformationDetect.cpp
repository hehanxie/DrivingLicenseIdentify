//
// Created by Nil on 2017/7/24.
//

#include "InformationDetect.h"

InformationDetect::InformationDetect()
{
	borderWidth = 0;
	borderHeight = 0;
}

void InformationDetect::locateInfor(Mat srcImage)
{

	/*
	 * 图像前期处理：高斯 -> 灰度 -> Sobel -> Binary -> Closed -> findContours
	 * 选择所有可能的矩形 -> 筛选判断
	*/

	/*
	// 高斯
	Mat blurImage;
	GaussianBlur(srcImage, blurImage, Size(DEFAULT_GAUSSIANBLUR_SIZE, DEFAULT_GAUSSIANBLUR_SIZE), 0, 0, BORDER_DEFAULT);

	// 灰度
	Mat grayImage;
	cvtColor(blurImage, grayImage, CV_RGB2GRAY);
//	imshow("gray image", grayImage);

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
//	imshow("grad image", gradImage);

	// OSTU 二值化
	Mat thresholdImage;
	threshold(gradImage, thresholdImage, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	imshow("threshold image", thresholdImage);

	// 闭操作
	Mat closedImage;
	Mat element = getStructuringElement(MORPH_RECT, Size(DEFAULT_MORPH_SIZE_WIDTH, DEFAULT_MORPH_SIZE_HEIGHT));
	morphologyEx(thresholdImage, closedImage, MORPH_CLOSE, element);
	imshow("closed image", closedImage);

	// 找轮廓
	vector<vector<Point>> contours;
	findContours(closedImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	vector<vector<Point> >::iterator itc = contours.begin();
	vector<RotatedRect> rects;

	// 将所有可能的轮廓存储
	while (itc != contours.end())
	{
		// 获取每个轮廓的最小有界矩形区域
		RotatedRect mr = minAreaRect(Mat(*itc));
		if (!isValidInfor(mr))
		{
			itc = contours.erase(itc);
		}
		else
		{
			rects.push_back(mr);
			itc++;
		}
	}
//	cout << "total rects: " << contours.size() << endl;
//	drawContours(image, contours, -1, Scalar(0, 0, 255), 2);

	vector<Mat> resultVec;
	int k = 0;
	//  再次筛选符合初步
	for (int i = 0; i < rects.size(); i++)
	{
		// 如果矩形的旋转角度不在范围内，则抛弃
		if (isRedAreaAngle(rects[i]))
		{
			Point2f P[4];
			rects[i].points(P);
			for(int j = 0; j <= 3; j++)
			{
				line(srcImage, P[j], P[(j + 1) % 4], Scalar(0, 0, 255), 2);
			}
			//drawContours(image, contours, i, Scalar(0, 0, 255), 2);

			// 创建并旋转图像
			Mat rotmat = getRotationMatrix2D(rects[i].center, rects[i].angle, 1);
			Mat rotatedImage;
			warpAffine(srcImage, rotatedImage, rotmat, srcImage.size(), CV_INTER_CUBIC);
			// 将所有关键部分矩阵保存
			resultVec.push_back(rotatedImage);
		}
	}
//	imshow("1", resultVec[1]);

	imshow("draw contours", srcImage);
	//waitKey(0);

	*/
}

bool InformationDetect::isValidInfor(RotatedRect mr)
{
	/*

	int width = mr.size.width;
	int height = mr.size.height;
	int area = width * height;

	int rWidth = getBorderWidth() * (WIDTH_RATIO - SIZE_ERROR);
	int rHeight = getBorderHeight() * (HEIGHT_RATIO - SIZE_ERROR);
	int rMinArea = rWidth * rHeight;
	int rMaxArea = getBorderWidth() * getBorderHeight() * (AREA_RATIO + AREA_RATIO_ERROR);

	if (area > rMinArea && area < rMaxArea && width > rWidth && height > rHeight)
	{
		return true;
	}
//	if (area > 1000 && height > 25)
//	{
//		return true;
//	}

	*/
	return false;
}

