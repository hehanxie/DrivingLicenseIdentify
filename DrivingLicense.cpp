//
// Created by Nil on 2017/7/27.
//

#include "DrivingLicense.h"


DrivingLicense::DrivingLicense(Mat src)
{
	this->srcImage = src.clone();
	HEIGHT = srcImage.rows;
	WIDTH = srcImage.cols;

	redMarkArea = new RedMarkArea(src);
//	if (!redMarkArea->isFindRedArea)
//	{
//		return ;
//	}
	// get rotated angle
	ANGLE = redMarkArea->getAngle();
	rotateImage(src, this->srcImage, ANGLE);
	showImage = this->srcImage.clone();

	// get red mark area
	redArea = redMarkArea->getRedRect();
	// correct angle
	correctRect(redArea, ANGLE);
	// draw red area to show
	rectangle(showImage, redArea, Scalar(0, 255, 0));
	imshow("red area", showImage);

	// put all area into keyMat vector
	getKeyInformation(resultWord);

	imshow("draw all area", showImage);
}



Mat DrivingLicense::getRightSideArea(Rect redArea, float ratio)
{
	int redWidth = redArea.width;
	int redHeight = redArea.height;
	Point p1;
	p1.x = redArea.x + redWidth;
	p1.y = redArea.y;

	Point p2;
	p2.x = p1.x + redWidth * ratio;
	p2.y = p1.y + redHeight;

	// rectangle need to be corrected
	Rect rect = Rect(p1, p2);
	// set roi image
	Mat roi;
	roi = srcImage(rect);
	// draw area
	rectangle(showImage, rect, Scalar(0, 255, 0));
//	imshow("right area", showImage);
	return roi;
}

Mat DrivingLicense::getDownSideArea(Rect redArea, float widthRatio, float heightRatio)
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
	Mat roi;
	roi = srcImage(rect);

	rectangle(showImage, rect, Scalar(255, 0, 255));
//	imshow("down area", showImage);
	return roi;
}

Mat DrivingLicense::getUpSideArea(Rect redArea, float widthRatio, float heightRatio)
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
	this->upSideRect = rect;
	Mat roi;
	roi = srcImage(rect);
	rectangle(showImage, rect, Scalar(255, 0, 0));
//	imshow("up area", showImage);
	return roi;
}

Mat DrivingLicense::getUpperSideArea(Rect upSideArea, float ratio)
{
	int width = upSideArea.width;
	int height = upSideArea.height;
	Point p1;
	p1.x = upSideArea.x;
	p1.y = upSideArea.y;

	Point p2;
	p2.x = p1.x + width;
	p2.y = p1.y - height * ratio;

	Rect rect = Rect(p1, p2);
	this->upperSideRect = rect;
	Mat roi;
	roi = srcImage(rect);
	rectangle(showImage, rect, Scalar(0, 255, 0));
//	imshow("upper area", showImage);
	return roi;
}

Mat DrivingLicense::getTopSideArea(Rect upperSideArea, float ratio)
{
	int width = upperSideArea.width;
	int height = upperSideArea.height;
	Point p1;
	p1.x = upperSideArea.x;
	p1.y = upperSideArea.y;

	Point p2;
	p2.x = p1.x + width;
	p2.y = p1.y - height * ratio;

	Rect rect = Rect(p1, p2);
	Mat roi;
	roi = srcImage(rect);
	rectangle(showImage, rect, Scalar(0, 0, 255));
//	imshow("top area", showImage);
	return roi;
}

void DrivingLicense::rotateImage(Mat src, Mat &img_rotate, float angle)
{
	if (angle == 0)
	{
		cout << "image is regular" << endl;
		return ;
	}
	//旋转中心为图像中心
	Point2f center;
	center.x = float(src.cols / 2.0);
	center.y = float(src.rows / 2.0);
//	int length = 0;
//	length = sqrt(src.cols * src.cols + src.rows * src.rows);
	//计算二维旋转的仿射变换矩阵
	Mat M = getRotationMatrix2D(center, angle, 1);
	// negative num means rotate clockwise
	warpAffine(src, img_rotate, M, Size(src.cols, src.rows), 1, 0, Scalar(255, 255, 255));//仿射变换，背景色填充为白色
//	imshow("rotate", img_rotate);
	// Move image
}

void DrivingLicense::correctRect(Rect &rect, float angle)
{
	float radian = angle * CV_PI / 180;
	cout << "y offset: " << sinf(radian) * 0.5 * HEIGHT << endl;
//	cout << "cos: " << cosf(radian) << endl;
	rect.y += 0.7 * sinf(radian) * HEIGHT;
}

Mat DrivingLicense::areaDivide(Mat roi, float widthOffsetRatio, float heightOffsetRatio, float widthRatio, float heightRatio)
{
//	imshow("valid", roi);

	Mat keywordArea;
	int width = roi.cols;
	int height = roi.rows;
	Point p1;
	p1.x = width * widthOffsetRatio;
	p1.y = height * heightOffsetRatio;
	Point p2;
	p2.x = p1.x + width * widthRatio;
	p2.y = p1.y + height * heightRatio;

	keywordArea = roi(Rect(p1, p2));
//	cout << Rect(p1, p2) << endl;
	return keywordArea;
}

void DrivingLicense::getKeyInformation(vector<vector<Mat>> &v)
{
	// get each part of key, and set as roi
	rightSideArea = getRightSideArea(redArea, RIGHT_WIDTH_RATIO);
	Mat birthday   = areaDivide(rightSideArea, 0.3,  0,    0.7, 0.3);
	Mat firstIssue = areaDivide(rightSideArea, 0.45, 0.33, 0.5, 0.3);
	Mat classType  = areaDivide(rightSideArea, 0.40, 0.67, 0.5, 0.25);

	v.push_back(wordDivide(birthday, "birthday"));
	v.push_back(wordDivide(firstIssue, "firstIssue"));
	v.push_back(wordDivide(classType, "classType"));


	downSideArea = getDownSideArea(redArea, DOWN_WIDTH_RATIO, DOWN_HEIGHT_RATIO);
	Mat validTime = areaDivide(downSideArea, 0.28, 0, 0.72, 1);
//	v.push_back(wordDivide(validTime));

	// width = 4, height = 0.50
	upSideArea = getUpSideArea(redArea, 4, 0.5);
	Mat address1 = areaDivide(upSideArea, 0.1, 0, 0.9, 0.5);
	Mat address2 = areaDivide(upSideArea, 0.05, 0.5, 0.95, 0.5);
//	v.push_back(wordDivide(address1));
//	v.push_back(wordDivide(address2));

	upperSideArea = getUpperSideArea(upSideRect, UPPER_HEIGHT_RATIO);
	Mat name = areaDivide(upperSideArea, 0.08, 0, 0.35, 1);
	Mat sex  = areaDivide(upperSideArea, 0.56, 0, 0.10, 1);
	Mat nationality = areaDivide(upperSideArea, 0.77, 0, 0.20, 1);
//	v.push_back(wordDivide(name));
//	v.push_back(wordDivide(sex));
//	v.push_back(wordDivide(nationality));

	topSideArea = getTopSideArea(upperSideRect, TOP_HEIGHT_RATIO);
	Mat driverID = areaDivide(topSideArea, 0.41, 0, 0.45, 0.9);
//	v.push_back(wordDivide(driverID));


//	for (int i = 0; i < v.size(); i++)
//	{
//		characterProcessing(v[i], PREFIX[i]);
//	}
}

vector<Mat> DrivingLicense::wordDivide(Mat image, string preStr)
{
	// 画轮廓矩形重叠切割——矩形顺序存在问题
	/*
	vector<Mat> result;
	int height = image.rows;
//	imshow("image", image);

	Mat dst;
	cvtColor(image, dst, CV_BGR2GRAY);
	threshold(dst, dst, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(dst, dst, MORPH_OPEN, element);


	Mat drawImage;
	cvtColor(dst, drawImage, CV_GRAY2BGR);

	vector<vector<Point>> contours;
	dst = dst < 100;
	findContours(dst, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	cout << "contours: " << contours.size() << endl;
//	Mat t = drawImage.clone();

	Rect rect1, rect2;
	double rate;
	int rSquare = height * height * 0.05;
	int size = contours.size();
	int square = 0;
	for (int i = 0; i < size; i++)
	{
		rect1 = boundingRect(contours[i]);
		if (i == 0)
		{
			rect2 = boundingRect(contours[i]);
		}
		else
		{
			rect2 = boundingRect(contours[i-1]);
		}

//		rectangle(t, rect1, Scalar(0, 0, 255));

		square = rect1.width * rect1.height;
		if (square >= rSquare)
		{
			rate = RectOverLapCoefficient(rect1, rect2);
//			cout << "rate: " << rate << endl;
			if (rate >= 0.2)
			{
				Rect mergeRect = RectMerge(rect1, rect2);
				rectangle(drawImage, mergeRect, Scalar(0, 0, 255));

				result.push_back(image(mergeRect));
				string str = to_string(i);
				imshow(str, drawImage);
			}
			else
			{
				rectangle(drawImage, rect1, Scalar(0, 0, 255));

				result.push_back(image(rect1));
				string str = to_string(i);
				imshow(str, drawImage);
			}
		}
	}

//	imshow("pre draw", t);
	imshow("divide characters", drawImage);

	return result;
	*/

	// 垂直投影切割
	vector<Mat> result;
	cvtColor(image, image, CV_BGR2GRAY);
	Mat src = image.clone();
	adaptiveThreshold(image, image, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 25, 10);
	// 水平膨胀，使黑点连成线
	erode(image,image,Mat(1,2,CV_8U,cvScalar(1)),Point(0,0),3);

	Mat dst;
	cvtColor(image, dst, CV_GRAY2BGR);

	// HOUGH line detect
//	imshow(preStr + " canny", image);
	vector<Vec2f> lines;
	int lineThreshold = image.cols * 0.7;
	HoughLines(image, lines, 1, CV_PI/180, lineThreshold, 0, 0 );
	while(lines.size() > 3)
	{
		lineThreshold++;
		HoughLines(image, lines, 1, CV_PI/90, lineThreshold, 0, 0 );
	}
	for( size_t i = 0; i < lines.size(); i++ )
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(dst, pt1, pt2, Scalar(0, 0, 255), 1, CV_AA);
	}

//	threshold(image, image, 100, 255, CV_THRESH_TRIANGLE + CV_THRESH_BINARY);
//	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
//	morphologyEx(image, image, MORPH_OPEN, element);
//	morphologyEx(image, image, MORPH_CLOSE, element);
//	morphologyEx(image, image, MORPH_OPEN, element);

//	imshow(preStr, dst);

	/*
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
//		cout << col << " x: " << tmp << endl;

	}
	//创建并绘制垂直投影图像
	Mat projImg(height, width, CV_8U, cv::Scalar(255));

	for (int col = 0; col < width; ++col)
	{
		line(projImg, Point(col, height - blackArray[col]), Point(col, height - 1), Scalar::all(0));
	}
//	imshow("birth vertical", projImg);

	int startIndex = 0;
	int endIndex = 0;
	// 是否进入字符区域
	bool inBlock = false;
	for (int col = 0; col < width; col++)
	{
		// 进入字符区
		if (!inBlock && blackArray[col] != 0)
		{
			startIndex = col;
			inBlock = true;
		}
		// 进入空白区
		else if (blackArray[col] == 0 && inBlock)
		{
			inBlock = false;
			endIndex = col;
			Mat roiImg = src(Range(0, height), Range(startIndex, endIndex));
			result.push_back(roiImg);

			string str = to_string(col);
//			imshow(str, roiImg);
		}
	}
	 */
	return result;
}

double DrivingLicense::RectOverLapCoefficient(Rect rect1, Rect rect2)
{
	CV_Assert(rect1.area() != 0 && rect2.area() != 0);
	double rate = 0.0;
	int flag = isRectOverLap(rect1, rect2);
	if (flag == false)
	{
		rate = 0.0;
		return 0;
	}
	Rect rectMerge;
	rectMerge = RectMerge(rect1, rect2);
	Mat dst(Size(rectMerge.width + rectMerge.x, rectMerge.height + rectMerge.y), CV_8UC1, Scalar(0));
	int maxArea = rect1.area() > rect2.area() ? rect1.area() : rect2.area();
	if (maxArea == rect1.area())
	{
		dst(rect2) = Scalar::all(0);
		dst(rect1) = Scalar::all(127);
		int count = countNonZero(dst(rect2));
		rate = (double)count / (double)rect2.area();
	}
	else
	{
		dst(rect1) = cv::Scalar::all(0);
		dst(rect2) = cv::Scalar::all(127);
		int count = countNonZero(dst(rect1));
		rate = (double)count / (double)rect1.area();
	}
	return rate;
}

bool DrivingLicense::isRectOverLap(Rect rect1, Rect rect2)
{
	//先算包含情况
	int x1 = rect1.x;
	int x2 = rect2.x;
	int y1 = rect1.y;
	int y2 = rect2.y;
	int width1 = rect1.width;
	int width2 = rect2.width;
	int height1 = rect1.height;
	int height2 = rect2.height;
	if (x2 > x1 + width1 || x2 + width2 < x1 || y2 + height2 < y1 || y2 > y1 + height1)
	{
		return false;
	}
	if (x1 > x2 + width2 || x1 + width1 < x2 || y1 + height1 < y2 || y1 > y2 + height2)
	{
		return false;
	}
	return true;
}

Rect DrivingLicense::RectMerge(Rect rect1, Rect rect2)
{
	Rect rectM;
	rectM.x = min(rect1.x, rect2.x);
	rectM.y = min(rect1.y, rect2.y);
	rectM.width = max(rect1.x + rect1.width, rect2.x + rect2.width) - rectM.x;
	rectM.height = max(rect1.y + rect1.height, rect2.y + rect2.height) - rectM.y;
	return rectM;
}

void DrivingLicense::characterProcessing(vector<Mat> &v, string prefix)
{
	cout << prefix << "size: " << v.size() << endl;
	for (int i = 0; i < v.size(); i++)
	{
		Mat c = v[i];
		threshold(c, c, 100, 255, CV_THRESH_BINARY);
		string str = to_string(i);
//		imwrite(PATH + prefix + str + ".png", c);
	}

	/*
	 * birthday: 9
	 * firstIssue: 9
	 * classType: 2
	 * validTime: 16
	 * address1: 18
	 * address2: 5
	 * name: 5
	 * sex: 1
	 * nationality: 4
	 * driverID: 14
	 */
}