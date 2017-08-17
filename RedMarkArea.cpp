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
	isFindRedArea = false;
	HEIGHT = srcImage.rows;
	WIDTH = srcImage.cols;

	horizontalArray = new int[HEIGHT];
	verticalArray = new int[WIDTH];

	colorMatch();
	setRedSize();
	isFindRedArea = isRedArea(getRedRect());
	CV_Assert(isFindRedArea);
	lineDetect();
}

void RedMarkArea::colorMatch()
{
	// 设置 红色的 HSV范围
	int minRedH = 140; // 140
	int maxRedH = 180;

	int minS = 75; // 75
	int maxS = 255;

	int minV = 75; // 75
	int maxV = 255;

	// 将BGR 转换为 HSV
	Mat srcHSV;
	cvtColor(showImage, srcHSV, CV_BGR2HSV);
	// 直方图均衡化
	vector<Mat> hsvSplit;
	split(srcHSV, hsvSplit);
	equalizeHist(hsvSplit[2], hsvSplit[2]);
	merge(hsvSplit, srcHSV);

	// 通过hsv空间，定位红色区域
	inRange(srcHSV, Scalar(minRedH, minS, minV), Scalar(maxRedH, maxS, maxV), srcHSV);
//	imshow("HSV", srcHSV);

	// 通过RGB定位红色区域
	Mat bgrImage = srcImage.clone();
	for(int i=0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			int b = (uchar) bgrImage.at<Vec3b>(i, j)[0];
			int g = (uchar) bgrImage.at<Vec3b>(i, j)[1];
			int r = (uchar) bgrImage.at<Vec3b>(i, j)[2];
			if (j < WIDTH/2 && r - g >= 40 && r - b >= 40)
			{
				bgrImage.at<Vec3b>(i, j) = 255;
			}
			else
			{
				bgrImage.at<Vec3b>(i, j) = 0;
			}
		}
	}
//	imshow("RGB", bgrImage);
	cvtColor(bgrImage, bgrImage, CV_BGR2GRAY);
	// 合并定位结果
	Mat redLocationImage;
	addWeighted(bgrImage, 0.5, srcHSV, 0.5, 0.0, redLocationImage);
//	imshow("combine", redLocationImage);

	Mat dst;
	redLocationImage.convertTo(dst, CV_8UC1);
	adaptiveThreshold(dst, redLocationImage, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 7, 3);
//	imshow("adaptive", redLocationImage);

	// 开操作 (去除一些噪点)
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(redLocationImage, redLocationImage, MORPH_OPEN, element);

	// 闭操作 (连接一些连通域)
//	morphologyEx(redLocationImage, redLocationImage, MORPH_CLOSE, element);

	// get red image
	this->redImage = redLocationImage.clone();
//	imshow("red", redImage);

	// 显示经过处理后的图像
	getHorizontalProjection(redLocationImage);
//	imshow("y", getHorizontalProjection(redLocationImage));
	getVerticalProjection(redLocationImage);
//	imshow("x", getVerticalProjection(redLocationImage));
}

bool RedMarkArea::isRedArea(Rect mr)
{
//	cout << "width: " << width;
//	cout << " height: " << height << endl;
	int height = mr.height;
	int width = mr.width;
	float scale = 1;
	if (height == 0 || width == 0)
	{
		return false;
	}
	if (width > height)
	{
		scale = width / height;
	}
	else
	{
		scale = height / width;
	}

	int area = width * height;
	int imageArea = HEIGHT * WIDTH;
	int rMinArea = imageArea * MIN_RED_AREA_RATIO;
	int rMaxArea = imageArea * MAX_RED_AREA_RATIO;

	if (area > rMinArea && area < rMaxArea && scale > MIN_SCALE && scale < MAX_SCALE)
	{
		return true;
	}
	return false;
}

Mat RedMarkArea::getVerticalProjection(Mat image)
{
	//图像的高和宽
	int height = image.rows;
	int width = image.cols;
	// 保存当前行的黑点数目
	int tmp = 0;
	// 保存每一行黑点数目的数组
	int *blackArray = new int[width];

	//循环访问图像数据，查找每一行的黑点的数目
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
//		if (tmp != 0)
//		{
//			cout << col << " x: " << tmp << endl;
//		}
	}
	//创建并绘制垂直投影图像
	Mat projImg(height, width, CV_8U, cv::Scalar(255));

	for (int col = 0; col < width; ++col)
	{
		line(projImg, Point(col, height - blackArray[col]), Point(col, height - 1), Scalar::all(0));
	}

	verticalArray = blackArray;
	return projImg;
}

Mat RedMarkArea::getHorizontalProjection (Mat image)
{
	//图像的高和宽
	int height = image.rows;
	int width = image.cols;
	// 保存当前行的黑点数目
	int tmp = 0;
	// 保存每一行黑点数目的数组
	int *blackArray = new int[height];


	//循环访问图像数据，查找每一行的黑点的数目
	for (int i = 0; i < height; i++)
	{
		tmp = 0;
		// 红色区域位于图像左半边
		for (int j = 0; j < width/2; j++)
		{
			// 黑点
			if (image.at<uchar>(i, j) == 0)
			{
				tmp++;
			}
		}
		blackArray[i] = tmp;
//		if (tmp != 0)
//		{
//			cout << i << " y: " << tmp << endl;
//		}
	}

	//创建并绘制水平投影图像
	Mat projImg(height, width, CV_8U, Scalar(0));

	for (int i = 0; i < height; i++)
	{
		//  黑点个数
		line(projImg, Point(blackArray[i], i), Point(width - 1, i), Scalar::all(255));
	}

	horizontalArray = blackArray;
	return  projImg;
}

void RedMarkArea::setRedSize()
{
	int N = srcImage.rows > srcImage.cols ? srcImage.cols : srcImage.rows * 0.075;
	cout << "threshold dot: " << N << endl;
	// x position
	for (int i = 0; i < WIDTH/2; i++)
	{
		if (abs(verticalArray[i + 1] - verticalArray[i]) >= 10 && verticalArray[i] > N)
		{
			startCol = i + 1;
			break;
		}
	}
	for (int i = WIDTH/2; i > 1; i--)
	{
		if (abs(verticalArray[i] - verticalArray[i - 1]) >= 10 && verticalArray[i] > N)
		{
			endCol = i - 1;
			break;
		}
	}

	// y position
	for (int i = HEIGHT/2; i < HEIGHT - 1; i++)
	{
		if (abs(horizontalArray[i + 1] - horizontalArray[i]) >= 10 && horizontalArray[i] > N)
		{
			startRow = i + 1;
			break;
		}
	}
	for (int i = HEIGHT - 1; i > 1; i--)
	{
		if (abs(horizontalArray[i] - horizontalArray[i - 1]) >= 10 && horizontalArray[i] > N)
		{
			endRow = i - 1;
			break;
		}
	}

	Point p1 = Point(startCol, startRow);
	Point p2 = Point(endCol, endRow);

	Rect rect = Rect(p1, p2);
//	if (rect.width > rect.height)
//	{
//		rect.height = rect.width;
//	}
//	else
//	{
//		rect.width = rect.height;
//	}
	setRedRect(rect);
	cout << "red rect: " << rect << endl;

	rectangle(showImage, rect, Scalar(255, 0, 0));

//	imshow("red mark", showImage);
}

void RedMarkArea::lineDetect()
{
//	imshow("red image", redImage);
	Mat lineImage;
	cvtColor(redImage, lineImage, CV_GRAY2BGR);

	Mat blurImage;
	blur(redImage, blurImage, Size(3, 3));
//	imshow("blur", blurImage);
	float minThreshold = 70;
	Mat dst, cannyImage;

	Canny(blurImage, dst, minThreshold, 3 * minThreshold);
	threshold(dst, dst, 0, 255, CV_THRESH_BINARY);
//	imshow("canny", dst);


	// draw line and corner and calculate angle
	vector<Vec2f> lines;
	int lineThreshold = 100;
	if (WIDTH > HEIGHT)
	{
		lineThreshold = HEIGHT * 0.13;
	}
	else
	{
		lineThreshold = WIDTH * 0.13;
	}
	HoughLines(dst, lines, 1, CV_PI / 180, lineThreshold, 0, 0);
	float sum = 0;
	int count = 0;
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		double a = cos(theta), b = sin(theta);
		if (a < 0.5 && a > -0.5)
		{
			sum += theta;
			count++;
			Point pt1, pt2;

			double x0 = a * rho, y0 = b * rho;
			// P1 -> P2
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			line(lineImage, pt1, pt2, Scalar(0, 0, 255), 1, CV_AA);

//			cout << "cos: " << a << "\tsin: " << b;
//			cout << "\tangle: " << degreeTrans(theta) - 90 << endl;
		}
	}
	float average;
	if (count == 0)
	{
		average = 1;
	}
	else
	{
		average= sum/count;
	}
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

void RedMarkArea::setRedRect(Rect rect)
{
	this->redRect = rect;
}

Rect RedMarkArea::getRedRect()
{
	return redRect;
}
