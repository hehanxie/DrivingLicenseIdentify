//
// Created by Nil on 2017/7/24.
//

#include "InformationLocate.h"

InformationLocate::InformationLocate()
{
	borderWidth = 0;
	borderHeight = 0;

	errorAngle = 60;
}

Mat InformationLocate::OstuBeresenThreshold(Mat image)
{
	if (image.empty())
	{
		cout << "error" << endl;
	}

	int width = image.cols;
	int height = image.rows;
	int x = 0, y = 0;
	int pixelCount[256] = {0};
	float pixelPro[256] = {0};
	int pixelSum = width * height, worsethreshold = 0, threshold = 0;

	uchar* data = (uchar*)image.data;

	//统计灰度级中每个像素在整幅图像中的个数
	for (int i = y; i < height; i++)
	{
		for (int j = x; j <width; j++)
		{
			pixelCount[data[i * image.cols + j]]++;
		}
	}

	//计算每个像素在整幅图像中的比例
	for (int i = 0; i < 256; i++)
	{
		pixelPro[i] = (float)(pixelCount[i]) / (float)(pixelSum);
	}

	//经典ostu算法,得到前景和背景的分割
	//遍历灰度级[0,255],计算出方差最大的灰度值,为最佳阈值
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMin = -10, deltaMax = 0;
	for (int i = 0; i < 256; i++)
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;

		for (int j = 0; j < 256; j++)
		{
			if (j <= i) //背景部分
			{
				//以i为阈值分类，第一类总的概率
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}
			else       //前景部分
			{
				//以i为阈值分类，第二类总的概率
				w1 += pixelPro[j];
				u1tmp += j * pixelPro[j];
			}
		}

		u0 = u0tmp / w0;        //第一类的平均灰度
		u1 = u1tmp / w1;        //第二类的平均灰度
		u = u0tmp + u1tmp;        //整幅图像的平均灰度
		//计算类间方差
		deltaTmp = w0 * (u0 - u)*(u0 - u) + w1 * (u1 - u)*(u1 - u);
		//找出最大类间方差以及对应的阈值
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = i;
		}
		if (deltaTmp < deltaMin)
		{
			deltaMin = deltaTmp;
			worsethreshold = i;
		}
	}

//	cout << "ostu, beresen" << endl;
//	cout << "最佳阈值: " << threshold << endl;
//	cout << "最差阈值: " << worsethreshold << endl;
	const float C = 0.12;
	float beta = C * (threshold - worsethreshold + 1) / 128;
//	cout << "矫正因子: " << beta << endl;

	float rectAveThreshold;
	//IplImage to Mat
	double lowT = (1 - beta) * threshold;
	double highT = (1 + beta) * threshold;
	Mat targetImage = image.clone();
	for (int i = 0; i < height; i++)
	{
		uchar * p = image.ptr<uchar>(i);
		uchar *outPtr = targetImage.ptr<uchar>(i);
		for (int j = 0; j < width; j++)
		{
			if(i < 2 ||  i > height - 2 || j < 2 || j > height - 2)
			{
				if( p[j] <= lowT )
				{
					outPtr[j] = 0;
				}
				else
				{
					outPtr[j] = 255;
				}
			}
			else
			{
				rectAveThreshold = sum(image(Rect(i-2,j-2,2,2)))[0]/4;
				if( p[j] < lowT || (p[j] < rectAveThreshold &&  (lowT <= p[j] && p[j] >= highT)))
				{
					outPtr[j] = 0;
				}
				if( p[j] > highT || (p[j] >= rectAveThreshold &&  (lowT <= p[j] && p[j] >= highT)))
				{
					outPtr[j] = 255;
				}
			}
		}
	}
	return targetImage;
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

int InformationLocate::locateBorder(Mat image)
{
	setImageWidth(image.cols);
	setImageHeight(image.rows);

	Mat srcImage;
	cvtColor(image, srcImage, COLOR_BGR2GRAY);
	if (srcImage.empty())
	{
		cout << "error";
		return 0;
	}

	// ostu 和 beresen算法，重新生成图像
	Mat ostuBeresenImage = OstuBeresenThreshold(srcImage);
//	imshow("ostu beresen image", ostuBeresenImage);

	// closed处理
	Mat element = getStructuringElement(MORPH_RECT ,Size(DEFAULT_MORPH_SIZE_WIDTH ,DEFAULT_MORPH_SIZE_WIDTH));
	Mat closedImage;
	//进行形态学操作
	morphologyEx(ostuBeresenImage, closedImage, MORPH_CLOSE, element);
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

//			vector<Point> cnt = contours[index];
//			float epsilon = 0.1 * arcLength(cnt, true);
//			approxPolyDP(cnt, contours[index], epsilon, true);
//			// 对画出的边重新矫正
//			drawContours(image, contours, index, Scalar(0, 0, 255), 2);

			setBorderWidth(mr.size.width);
			setBorderHeight(mr.size.height);
			break;
		}
		itc++;
		index++;
	}
	imshow("draw contours", image);
	//waitKey(0);
	return 1;
}

int InformationLocate::locateInfor(Mat srcImage)
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

	Mat thresholdImage;
	threshold(gradImage, thresholdImage, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	imshow("threshold image", thresholdImage);

	// closed operation
	Mat closedImage;
	Mat element = getStructuringElement(MORPH_RECT, Size(DEFAULT_MORPH_SIZE_WIDTH, DEFAULT_MORPH_SIZE_HEIGHT));
	morphologyEx(thresholdImage, closedImage, MORPH_CLOSE, element);
	imshow("closed image", closedImage);

	// get contours
	vector<vector<Point>> contours;
	findContours(closedImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//	drawContours(image, contours, -1, Scalar(0, 0, 255), 2);

	vector<vector<Point> >::iterator itc = contours.begin();
	vector<RotatedRect> rects;

	// push possible rects to vector
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
	cout << "total rects: " << contours.size() << endl;
//	drawContours(image, contours, -1, Scalar(0, 0, 255), 2);

	vector<Mat> resultVec;
	int k = 0;
	//  再次筛选符合初步
	for (int i = 0; i < rects.size(); i++)
	{
		RotatedRect minRect = rects[i];
		Size2f rectSize = minRect.size;
		int width = rectSize.width;
		int height = rectSize.height;
		float r = width / height;
		float angle = minRect.angle;
		if (r < 1)
		{
			angle = 90 + angle;
			swap(width, height);
		}

		// 如果矩形的旋转角度不在范围内，则抛弃
		if (angle - errorAngle < 0 && angle + errorAngle > 0)
		{
			Point2f P[4];
			rects[i].points(P);
			for(int j = 0; j <= 3; j++)
			{
				line(srcImage, P[j], P[(j + 1) % 4], Scalar(0, 0, 255), 2);
			}
			//drawContours(image, contours, i, Scalar(0, 0, 255), 2);

			//Create and rotate image
			Mat rotmat = getRotationMatrix2D(minRect.center, angle, 1);
			Mat rotatedImage;
			warpAffine(srcImage, rotatedImage, rotmat, srcImage.size(), CV_INTER_CUBIC);

//			Mat resultMat;
//			resultMat = showResultMat(rotatedImage, rectSize, minRect.center, k++);
			resultVec.push_back(rotatedImage);
		}
	}
//	imshow("1", resultVec[1]);

	imshow("draw contours", srcImage);
	//waitKey(0);
	return 1;
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
