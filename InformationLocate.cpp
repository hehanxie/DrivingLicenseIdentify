//
// Created by Nil on 2017/7/24.
//

#include "InformationLocate.h"

InformationLocate::InformationLocate()
{
	borderWidth = 0;
	borderHeight = 0;
}


bool InformationLocate::isBorder(RotatedRect candidate)
{
	// get area
	int area = candidate.size.height * candidate.size.width;
	int rArea = getImageWidth() * getImageHeight() * 0.5;

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

void InformationLocate::locateBorder(Mat image)
{
	setImageWidth(image.cols);
	setImageHeight(image.rows);

	Mat srcImage;
	cvtColor(image, srcImage, COLOR_BGR2GRAY);
	if (srcImage.empty())
	{
		cout << "error";
		return ;
	}

	Mat dst;
	adaptiveThreshold(srcImage, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 7, 7);
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
				line(image, P[j], P[(j + 1) % 4], Scalar(0, 0, 255), 2);
			}
			// set border value
			setBorderWidth(mr.size.width);
			setBorderHeight(mr.size.height);
			cout << "border width: " << mr.size.width << endl;
			cout << "border height: " << mr.size.height << endl;
			break;
		}
		itc++;
		index++;
	}
//	imshow("draw border", image);
}

void InformationLocate::locateInfor(Mat srcImage)
{
	/*
	 * 图像前期处理：高斯 -> 灰度 -> Sobel -> Binary -> Closed -> findContours
	 * 选择所有可能的矩形 -> 筛选判断
	*/

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
}

bool InformationLocate::isValidInfor(RotatedRect mr)
{
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
	return false;
}

int InformationLocate::getBorderWidth()
{
	return borderWidth;
}

void InformationLocate::setBorderWidth(int borderWidth)
{
	InformationLocate::borderWidth = borderWidth;
}

int InformationLocate::getBorderHeight()
{
	return borderHeight;
}

void InformationLocate::setBorderHeight(int borderHeight)
{
	InformationLocate::borderHeight = borderHeight;
}

int InformationLocate::getImageWidth()
{
	return imageWidth;
}

int InformationLocate::getImageHeight()
{
	return imageHeight;
}

void InformationLocate::setImageWidth(int imageWidth)
{
	InformationLocate::imageWidth = imageWidth;
}

void InformationLocate::setImageHeight(int imageHeight)
{
	InformationLocate::imageHeight = imageHeight;
}


// 7.26
void InformationLocate::colorMatch(Mat src)
{
	// 设置 红色的 HSV范围
	int minRedH = 100;
	int maxRedH = 180;

	int minS = 40;
	int maxS = 255;
	int minV = 40;
	int maxV = 255;

	// 将BGR 转换为 HSV
	Mat srcHSV;
	cvtColor(src, srcHSV, CV_BGR2HSV);
	vector<Mat> hsvSplit;
	split(srcHSV, hsvSplit);
	equalizeHist(hsvSplit[2], hsvSplit[2]);
	merge(hsvSplit, srcHSV);
	Mat thresholdImage;
//	inRange(srcHSV, Scalar(0, minS, minV), Scalar(15, maxS, maxV), thresholdImage);
	inRange(srcHSV, Scalar(minRedH, minS, minV), Scalar(maxRedH, maxS, maxV), thresholdImage);


	Mat dst;
	thresholdImage.convertTo(dst, CV_8UC1);
	adaptiveThreshold(dst, thresholdImage, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 7, 5);

	// 开操作 (去除一些噪点)
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(thresholdImage, thresholdImage, MORPH_OPEN, element);

	// 闭操作 (连接一些连通域)
	element = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(thresholdImage, thresholdImage, MORPH_CLOSE, element);

	// 显示经过处理吼的图像
	imshow("threshold image", thresholdImage);

	vector<vector<Point>> contours;
	findContours(thresholdImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	vector<vector<Point> >::iterator itc = contours.begin();
	vector<Mat> redAreaVec;
	while (itc != contours.end())
	{
		RotatedRect mr = minAreaRect(Mat(*itc));
		// 如果区域大小符合，且角度正常
		if (isRedArea(mr) && isRedAreaAngle(mr))
		{
			Point2f P[4];
			mr.points(P);
			for(int j = 0; j <= 3; j++)
			{
				// 画出红色区域（矩形）
				line(src, P[j], P[(j + 1) % 4], Scalar(0, 0, 255), 2);
			}

			// 将红色水印区域保存
			Mat rotmat = getRotationMatrix2D(mr.center, mr.angle, 1);
			Mat rotatedImage;
			warpAffine(src, rotatedImage, rotmat, src.size(), CV_INTER_CUBIC);

			redAreaVec.push_back(rotatedImage);
		}
		itc++;
	}

	imshow("red area", src);

	waitKey(0);
}

bool InformationLocate::isRedArea(RotatedRect mr)
{
	int width = mr.size.width;
	int height = mr.size.height;

//	cout << "width: " << width;
//	cout << " height: " << height << endl;
	int area = width * height;

	int borderArea = getBorderWidth() * getBorderHeight();
	int rMinArea = borderArea * (RED_AREA_RATIO - RED_AREA_ERROR);
	int rMaxArea = borderArea * (RED_AREA_RATIO + RED_AREA_ERROR);

	if (area > rMinArea && area < rMaxArea)
	{
		return true;
	}
	return false;
}

bool InformationLocate::isRedAreaAngle(RotatedRect mr)
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