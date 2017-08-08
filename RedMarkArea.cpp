//
// Created by Nil on 2017/7/27.
//


#include "RedMarkArea.h"
#include <iostream>
using namespace std;

RedMarkArea::RedMarkArea()
{

}

RedMarkArea::RedMarkArea(Mat src)
{
	this->srcImage = src.clone();
	showImage = src.clone();

	startRow = 0;
	endRow = 0;
	startCol = 0;
	endCol = 0;
	ANGLE = 0;
	HEIGHT = srcImage.rows;
	WIDTH = srcImage.cols;

	horizontalArray = new int[HEIGHT];
	verticalArray = new int[WIDTH];

	colorMatch();
	setRedSize();
	lineDetect();

	imshow("red image", showImage);
}

void RedMarkArea::colorMatch()
{
	// 设置 红色的 HSV范围
	int minRedH = 140; // 140
	int maxRedH = 180;

	int minS = 43; // 75
	int maxS = 255;
	int minV = 46; // 75
	int maxV = 220;

	// 将BGR 转换为 HSV
	Mat srcHSV;
	cvtColor(showImage, srcHSV, CV_BGR2HSV);
	// 直方图均衡化
	vector<Mat> hsvSplit;
	split(srcHSV, hsvSplit);
	equalizeHist(hsvSplit[2], hsvSplit[2]);
	merge(hsvSplit, srcHSV);

	// 通过hsv空间，定位红色区域
	Mat thresholdImage;
	inRange(srcHSV, Scalar(minRedH, minS, minV), Scalar(maxRedH, maxS, maxV), thresholdImage);
//	imshow("thre", thresholdImage);

	Mat dst;
	thresholdImage.convertTo(dst, CV_8UC1);
	adaptiveThreshold(dst, thresholdImage, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 7, 3);
//	imshow("adaptive", thresholdImage);

	// 开操作 (去除一些噪点)
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(thresholdImage, thresholdImage, MORPH_OPEN, element);

	// 闭操作 (连接一些连通域)
	morphologyEx(thresholdImage, thresholdImage, MORPH_CLOSE, element);

	// get red image
	this->redImage = thresholdImage.clone();
//	imshow("red", redImage);

	// 显示经过处理后的图像
	getHorizontalProjection(thresholdImage);
//	imshow("horizontal", getHorizontalProjection(thresholdImage));

	getVerticalProjection(thresholdImage);
//	imshow("vertical", getVerticalProjection(thresholdImage));
}

bool RedMarkArea::isRedArea(RotatedRect mr)
{
//	cout << "width: " << width;
//	cout << " height: " << height << endl;
	int area = mr.size.width * mr.size.height;

	int borderArea = HEIGHT * WIDTH;
	int rMinArea = borderArea * (RED_AREA_RATIO - RED_AREA_ERROR);
	int rMaxArea = borderArea * (RED_AREA_RATIO + RED_AREA_ERROR);

	if (area > rMinArea && area < rMaxArea)
	{
		return true;
	}
	return false;
}

Mat RedMarkArea::getVerticalProjection(Mat image)
{
	// 黑点最大数目和最大行
	int maxCol = 0;
	int maxNum = 0;
	// 黑点最小数目和最小行
	int minCol = 0;
	int minNum = HEIGHT;

	//图像的高和宽
	int height = HEIGHT;
	int width = WIDTH;
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
	int minNum = WIDTH;
	// 保存当前行的黑点数目
	int tmp = 0;
	// 保存每一行黑点数目的数组
	int *blackArray = new int[HEIGHT];


	//循环访问图像数据，查找每一行的白点的数目
	for (int i = 0; i < HEIGHT; i++)
	{
		tmp = 0;
		for (int j = 0; j < WIDTH/2; j++)
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
	Mat projImg(HEIGHT, WIDTH, CV_8U, Scalar(255));

	for (int i = 0; i < HEIGHT; i++)
	{
		//  黑点个数
		line(projImg, Point(blackArray[i], i), Point(WIDTH - 1, i), Scalar::all(0));
	}

	horizontalArray = blackArray;
	return  projImg;
}

void RedMarkArea::setRedSize()
{
	int N = srcImage.rows > srcImage.cols ? srcImage.cols : srcImage.rows * 0.075;
	// x position
	for (int i = 0; i < WIDTH/2; i++)
	{
		if (abs(verticalArray[i + 1] - verticalArray[i]) >= 10 && verticalArray[i] > N)
		{
			startCol = i + 3;
			break;
		}
	}
	for (int i = WIDTH/2; i > 1; i--)
	{
		if (abs(verticalArray[i] - verticalArray[i - 1]) >= 10 && verticalArray[i] > N)
		{
			endCol = i - 3;
			break;
		}
	}

	// y position
	for (int i = HEIGHT/2; i < HEIGHT - 1; i++)
	{
		if (abs(horizontalArray[i + 1] - horizontalArray[i]) >= 10 && horizontalArray[i] > N)
		{
			startRow = i + 3;
			break;
		}
	}
	for (int i = HEIGHT - 1; i > 1; i--)
	{
		if (abs(horizontalArray[i] - horizontalArray[i - 1]) >= 10 && horizontalArray[i] > N)
		{
			endRow = i - 3;
			break;
		}
	}

	Point p1 = Point(startCol, startRow);
	Point p2 = Point(endCol, endRow);

	Rect rect = Rect(p1, p2);
	if (rect.width > rect.height)
	{
		rect.height = rect.width;
	}
	else
	{
		rect.width = rect.height;
	}
	setRedRect(rect);
	cout << "red rect: " << rect << endl;

	rectangle(showImage, rect, Scalar(255, 0, 0));
}

void RedMarkArea::setRedRect(Rect rect)
{
	this->redRect = rect;
}

Rect RedMarkArea::getRedRect()
{
	return redRect;
}

void RedMarkArea::lineDetect()
{
//	imshow("red image", redImage);
	Mat lineImage;
	cvtColor(redImage, lineImage, CV_GRAY2BGR);

	Mat blurImage;
	blur(redImage, blurImage, Size(3, 3));
//	imshow("blur", blurImage);

	Mat dst, cannyImage;
	float minThreshold = 70;
	Canny(blurImage, dst, minThreshold, 3 * minThreshold);
	threshold(dst, dst, 0, 255, CV_THRESH_BINARY);
//	imshow("canny", dst);


	// draw line and corner and calculate angle
	vector<Vec2f> lines;
	int lineThreshold = 100;
	HoughLines(dst, lines, 1, CV_PI / 180, lineThreshold, 0, 0);
	float sum = 0;
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		sum += theta;
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(lineImage, pt1, pt2, Scalar(0, 0, 255), 1, CV_AA);
	}
	float average = sum/lines.size();
	if (lines.size() == 0)
	{
		average = 0;
	}
//	imshow("line", lineImage);
	float angle = degreeTrans(average) - 90;
	setAngle(angle);
//	Mat rotate;
//	rotateImage(srcImage, rotate, angle);
//	imshow("rotate", rotate);
}

//度数转换
float RedMarkArea::degreeTrans(float theta)
{
	double res = theta / CV_PI * 180;
	return res;
}

void RedMarkArea::setAngle(float angle)
{
	if (abs(angle) > 10)
	{
		angle = 0;
	}
	this->ANGLE = angle;
	cout << "angle = " << angle << endl;
}

float RedMarkArea::getAngle()
{
	return this->ANGLE;
}