//
// Created by Nil on 2017/7/27.
//


#include "RedMarkArea.h"
#include <iostream>
using namespace std;

RedMarkArea::RedMarkArea()
{

}

RedMarkArea::RedMarkArea(Mat srcImage, Mat borderImage)
{
	this->srcImage = srcImage.clone();

	startRow = 0;
	endRow = 0;
	startCol = 0;
	endCol = 0;
	srcHeight = srcImage.rows;
	srcWidth = srcImage.cols;
	borderHeight = borderImage.rows;
	borderWidth = borderImage.cols;

	horizontalArray = new int[borderHeight];
	verticalArray = new int[borderWidth];

	this->srcImage = srcImage;
	this->borderImage = borderImage;

	colorMatch();
	setRedSize();
//	imshow("src image", srcImage);
}

void RedMarkArea::colorMatch()
{
	// 设置 红色的 HSV范围
	int minRedH = 140; // 140
	int maxRedH = 180;

	int minS = 75; // 80
	int maxS = 255;
	int minV = 75; // 80
	int maxV = 220;

	// 将BGR 转换为 HSV
	Mat srcHSV;
	cvtColor(borderImage, srcHSV, CV_BGR2HSV);
	// 直方图均衡化
	vector<Mat> hsvSplit;
	split(srcHSV, hsvSplit);
	equalizeHist(hsvSplit[2], hsvSplit[2]);
	merge(hsvSplit, srcHSV);

	Mat thresholdImage;
//	inRange(srcHSV, Scalar(0, minS, minV), Scalar(15, maxS, maxV), thresholdImage);
//	inRange(srcHSV, Scalar(0, 60, 60), Scalar(10, maxS, maxV), thresholdImage);
	inRange(srcHSV, Scalar(minRedH, minS, minV), Scalar(maxRedH, maxS, maxV), thresholdImage);

	Mat dst;
	thresholdImage.convertTo(dst, CV_8UC1);
	adaptiveThreshold(dst, thresholdImage, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 7, 3);
//	imshow("adaptive", thresholdImage);

	// 开操作 (去除一些噪点)
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(thresholdImage, thresholdImage, MORPH_OPEN, element);

	// 闭操作 (连接一些连通域)
	element = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(thresholdImage, thresholdImage, MORPH_CLOSE, element);

	// 显示经过处理后的图像
	getHorizontalProjection(thresholdImage);
//	imshow("horizontal", getHorizontalProjection(thresholdImage));

	getVerticalProjection(thresholdImage);
//	imshow("vertical", getVerticalProjection(thresholdImage));

//	imshow("morphplogy image", thresholdImage);
//	imshow("src image", srcImage);
	// 描绘轮廓
//	vector<vector<Point>> contours;
//	findContours(thresholdImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//	drawContours(src, contours, -1, Scalar(0, 0, 255), 2);
//	cout << "contours: " << contours.size() << endl;
//	imshow("all contours", src);

//	vector<vector<Point> >::iterator itc = contours.begin();
//	vector<Mat> redAreaVec;
//	while (itc != contours.end())
//	{
//		RotatedRect mr = minAreaRect(Mat(*itc));
//		// 如果区域大小符合，且角度正常
//		if (isRedArea(mr) && isRedAreaAngle(mr))
//		{
//			Point2f P[4];
//			mr.points(P);
//			for(int j = 0; j <= 3; j++)
//			{
//				// 画出红色区域（矩形）
//				line(src, P[j], P[(j + 1) % 4], Scalar(0, 0, 255), 2);
//			}
//
//			// 将红色水印区域保存
//			Mat rotmat = getRotationMatrix2D(mr.center, mr.angle, 1);
//			Mat rotatedImage;
//			warpAffine(src, rotatedImage, rotmat, src.size(), CV_INTER_CUBIC);
//
//			redAreaVec.push_back(rotatedImage);
//		}
//		itc++;
//	}

//	imshow("red area", src);
//	waitKey(0);
}

bool RedMarkArea::isRedArea(RotatedRect mr)
{
//	cout << "width: " << width;
//	cout << " height: " << height << endl;
	int area = mr.size.width * mr.size.height;

	int borderArea = borderImage.cols * borderImage.rows;
	int rMinArea = borderArea * (RED_AREA_RATIO - RED_AREA_ERROR);
	int rMaxArea = borderArea * (RED_AREA_RATIO + RED_AREA_ERROR);

	if (area > rMinArea && area < rMaxArea)
	{
		return true;
	}
	return false;
}

bool RedMarkArea::isRedAreaAngle(RotatedRect mr)
{
	Size2f rectSize = mr.size;
	int width = rectSize.width;
	int height = rectSize.height;
	float angle = mr.angle;
	// 如果矩形的旋转角度不在范围内，则抛弃
	if (angle > - ANGLE_ERROR)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Mat RedMarkArea::getVerticalProjection(Mat image)
{
	// 黑点最大数目和最大行
	int maxCol = 0;
	int maxNum = 0;
	// 黑点最小数目和最小行
	int minCol = 0;
	int minNum = borderHeight;

	//图像的高和宽
	int height = borderHeight;
	int width = borderWidth;
	// 保存当前行的黑点数目
	int tmp = 0;
	// 保存每一行黑点数目的数组
	int *blackArray = new int[width];

	//循环访问图像数据，查找每一行的255点的数目
	for (int col = 0; col < width; col++)
	{
		tmp = 0;
		for (int row = 0; row < height; row++)
		{
			if (image.at<uchar>(row, col) == 0)
			{
				tmp++;
			}
		}
		blackArray[col] = tmp;
		if (tmp > maxNum)
		{
			maxNum = tmp;
			maxCol = col;
		}
		if (tmp < minNum)
		{
			minNum = tmp;
			minCol = col;
		}
//		if (tmp != 0)
//		{
//			cout << col << " x: " << tmp << endl;
//		}
	}
	//创建并绘制垂直投影图像
	Mat projImg(height, width, CV_8U, cv::Scalar(255));

	for (int col = 0; col < width; ++col)
	{
		line(projImg, Point(col, blackArray[col]), Point(col, height - 1), Scalar::all(0));
	}

	verticalArray = blackArray;
	return projImg;
}

Mat RedMarkArea::getHorizontalProjection (Mat image)
{
	// 黑点最大数目和最大行
	int maxLine = 0;
	int maxNum = 0;
	// 黑点最小数目和最小行
	int minLine = 0;
	int minNum = borderWidth;
	//图像的高和宽
	int height = borderHeight;
	int width = borderWidth;
	// 保存当前行的黑点数目
	int tmp = 0;
	// 保存每一行黑点数目的数组
	int *blackArray = new int[height];


	//循环访问图像数据，查找每一行的白点的数目
	for (int i = 0; i < height; i++)
	{
		tmp = 0;
		for (int j = 0; j < width/2; j++)
		{
			// 白点
			if (image.at<uchar>(i, j) == 0)
			{
				tmp++;
			}
		}
		blackArray[i] = tmp;
		if (tmp > maxNum)
		{
			maxNum = tmp;
			maxLine = i;
		}
		if (tmp < minNum)
		{
			minNum = tmp;
			minLine = i;
		}
//		if (tmp != 0)
//		{
//			cout << i << " y: " << tmp << endl;
//		}
	}

	//创建并绘制水平投影图像
	Mat projImg(height, width, CV_8U, Scalar(255));

	for (int i = 0; i < height; i++)
	{
		//  黑点个数
		line(projImg, Point(blackArray[i], i), Point(width - 1, i), Scalar::all(0));
	}

	horizontalArray = blackArray;
	return  projImg;
}

void RedMarkArea::setRedSize()
{
	// x position
	for (int i = 0; i < borderWidth/2; i++)
	{
		if (abs(verticalArray[i + 1] - verticalArray[i]) >= 10)
		{
			startCol = i + 5;
			break;
		}
	}
	for (int i = borderWidth/2; i > 1; i--)
	{
		if (abs(verticalArray[i] - verticalArray[i - 1]) >= 10)
		{
			endCol = i - 5;
			break;
		}
	}

	// y position
	for (int i = borderHeight/2; i < borderHeight - 1; i++)
	{
		if (abs(horizontalArray[i + 1] - horizontalArray[i]) >= 10)
		{
			startRow = i + 5;
			break;
		}
	}
	for (int i = borderHeight - 1; i > 1; i--)
	{
		if (abs(horizontalArray[i] - horizontalArray[i - 1]) >= 10)
		{
			endRow = i - 5;
			break;
		}
	}

	Point p1 = Point(startCol, startRow);
	Point p2 = Point(endCol, endRow);
	cout << "start point: " << p1 << endl;
	cout << "end point: " << p2 << endl;

	Rect rect = Rect(p1, p2);
	setRedArea(rect);
	rectangle(borderImage, rect, Scalar(0, 0, 255));
//	imshow("red area", borderImage);
}

void RedMarkArea::setRedArea(Rect redArea)
{
	this->redArea = redArea;
}

Rect RedMarkArea::getRedArea()
{
	return redArea;
}