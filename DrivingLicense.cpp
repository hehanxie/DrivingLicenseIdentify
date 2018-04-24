//
// Created by Nil on 2017/7/27.
//

#include "DrivingLicense.h"


DrivingLicense::DrivingLicense(Mat src)
{
	red_mark_area_ = new RedMarkArea(src);
	cout << "red area correct" << endl;
	this->src_image_ = red_mark_area_->GetCorrectSrcImage();
//	this->src_image_ = src.clone();
	HEIGHT_ = src_image_.rows;
	WIDTH_ = src_image_.cols;

//	red_mark_area_ = new RedMarkArea(src);
	show_image_ = this->src_image_.clone();

	// get red mark area
	red_area_rect = red_mark_area_->GetRedRect();

	// draw red area to show
	rectangle(show_image_, red_area_rect, Scalar(0, 255, 0), 2);
//	imshow("red area", show_image_);

	// put all area into keyMat vector
	GetKeyInformationArea(idDividedResult);
//	cvtColor(show_image_, show_image_, CV_BGR2GRAY);
	imshow("draw all area", show_image_);
//	OutputFile(idDividedResult, "");
}

void DrivingLicense::GetKeyInformationArea(vector<Mat> &v)
{

	// get each part of key, and set as roi
//	right_area_ = GetRightArea(red_area_rect, kRightWidthRatio);
//	Mat birthday   = AreaDivide(right_area_, 0.3, 0, 0.7, 0.3);
//	Mat firstIssue = AreaDivide(right_area_, 0.45, 0.33, 0.5, 0.3);
//	Mat classType  = AreaDivide(right_area_, 0.40, 0.67, 0.5, 0.25);

//	v.push_back(WordDivide(birthday, "birthday"));
//	v.push_back(WordDivide(firstIssue, "firstIssue"));
//	v.push_back(WordDivide(classType, "classType"));


//	down_area_ = GetDownArea(red_area_rect, kDownWidthRatio, kDownHeightRatio);
//	Mat validTime = AreaDivide(down_area_, 0.28, 0, 0.72, 1);
//	v.push_back(WordDivide(validTime));

	// width = 4, height = 0.50
	up_area_ = GetUpArea(red_area_rect, 4, 0.5);
	cout << "up area correct" << endl;
//	Mat address1 = AreaDivide(up_area_, 0.1, 0, 0.9, 0.5);
//	Mat address2 = AreaDivide(up_area_, 0.05, 0.5, 0.95, 0.5);
//	v.push_back(WordDivide(address1));
//	v.push_back(WordDivide(address2));

	upper_area_ = GetUpperArea(up_area_rect_, kUpperHeightRatio);
	cout << "upper area correct" << endl;
//	Mat name = AreaDivide(upper_area_, 0.08, 0, 0.35, 1);
//	Mat sex  = AreaDivide(upper_area_, 0.56, 0, 0.10, 1);
//	Mat nationality = AreaDivide(upper_area_, 0.77, 0, 0.20, 1);
//	v.push_back(WordDivide(name));
//	v.push_back(WordDivide(sex));
//	v.push_back(WordDivide(nationality));

	top_area_ = GetTopArea(upper_area_rect_, kTopHeightRatio);
	cout << "top area correct" << endl;
//	Mat driverID = AreaDivide(top_area_, 0.41, 0, 0.45, 0.9);
	Mat driverID = AreaDivide(top_area_, 0, 0, 0.9, 0.9);
	Mat idArea = IdAreaDivide(driverID, "driver_id");
	v = WordDivide(idArea, "driver_id");



	/*
	// 得到分割后的数字区域
	float height_offset_ratio_of_red = 0.83;
	float width_ratio_of_red = 4;
	float height_ratio_of_red = 0.275;
	Mat driverIdArea = IdAreaDivide(GetIdArea(red_area_rect, height_offset_ratio_of_red, width_ratio_of_red, height_ratio_of_red), "driver_id");
	v.push_back(WordDivide(driverIdArea, "driver_id"));
	*/

//	for (int i = 0; i < v.size(); i++)
//	{
//		OutputFile(v[i], PREFIX[i]);
//	}
}

Mat DrivingLicense::GetRightArea(Rect redArea, float ratio)
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
	roi = src_image_(rect);
	// draw area
//	rectangle(show_image_, rect, Scalar(0, 255, 0));
//	imshow("right area", show_image_);
	return roi;
}

Mat DrivingLicense::GetDownArea(Rect redArea, float widthRatio, float heightRatio)
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
	roi = src_image_(rect);

//	rectangle(show_image_, rect, Scalar(255, 0, 255));
//	imshow("down area", show_image_);
	return roi;
}

Mat DrivingLicense::GetUpArea(Rect redArea, float widthRatio, float heightRatio)
{
	int redWidth = redArea.width;
	int redHeight = redArea.height;
	Point p1;
	p1.x = redArea.x;
	p1.y = redArea.y;

	Point p2;
	p2.x = (int)(p1.x + redWidth * widthRatio) % src_image_.cols;
	p2.y = p1.y - redHeight * heightRatio;

	Rect rect = Rect(p1, p2);
	this->up_area_rect_ = rect;
	Mat roi;
	roi = src_image_(rect);

//	rectangle(show_image_, rect, Scalar(255, 0, 0));
//	imshow("up area", show_image_);
	return roi;
}

Mat DrivingLicense::GetUpperArea(Rect upSideArea, float ratio)
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
	this->upper_area_rect_ = rect;
	Mat roi;
	roi = src_image_(rect);
//	rectangle(show_image_, rect, Scalar(0, 255, 0));
//	imshow("upper area", show_image_);
	return roi;
}

Mat DrivingLicense::GetTopArea(Rect upperSideArea, float ratio)
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
	roi = src_image_(rect);
	rectangle(show_image_, rect, Scalar(0, 0, 0), 2);
//	imshow("top area", show_image_);
	return roi;
}

Mat DrivingLicense::GetIdArea(Rect redArea, float heightOffsetRatio, float widthRatio, float heightRatio)
{

	int redWidth = redArea.width;
	int redHeight = redArea.height;
	Point p1;
	p1.x = redArea.x;
	p1.y = redArea.y - redHeight * heightOffsetRatio;

	Point p2;
	p2.x = p1.x + redWidth * widthRatio;
	p2.y = p1.y - redHeight * heightRatio;

	Rect rect = Rect(p1, p2);
	this->up_area_rect_ = rect;
	Mat roi;
	roi = src_image_(rect);

	rectangle(show_image_, rect, Scalar(255, 0, 0), 2);
//	imshow("up area", show_image_);
	return roi;
}

Mat DrivingLicense::AreaDivide(Mat roi, float widthOffsetRatio, float heightOffsetRatio, float widthRatio,
							   float heightRatio)
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

Mat DrivingLicense:: IdAreaDivide(Mat image, string preStr)
{
	Mat src_image = image.clone();
	// 将得到的字符区域二值化
	vector<Mat> result;
	cvtColor(image, image, CV_BGR2GRAY, CV_8UC1);
	adaptiveThreshold(image, image, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 25, 10);
	Mat adaptive_image = image.clone();

	// 通过形态学闭操作，将数字区域连通
	Mat element = getStructuringElement(MORPH_RECT, Size(15, 3));
	morphologyEx(image, image, MORPH_CLOSE, element);
//	imshow(preStr, image);

	// 寻找轮廓，划分区域
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(image, contours, hierarchy, CV_CHAIN_APPROX_NONE, CHAIN_APPROX_SIMPLE);

	Mat contours_image = Mat::zeros(image.size(), CV_8UC1);
	Mat Contours = Mat::zeros(image.size(), CV_8UC1);  //绘制

	Rect word_area_rect;
	for (int i = 0; i < contours.size(); i++)
	{
		//绘制轮廓
		drawContours(contours_image, contours, i, Scalar(255), 1, 8, hierarchy);
		//绘制轮廓的最小外结矩形
		RotatedRect rect = minAreaRect(contours[i]);
		Point2f P[4];
		rect.points(P);
//		cout << rect.size.area() << endl;
		if (rect.size.area() > image.rows * image.cols * 0.2)
		{
			cout << "id area size: " << rect.size << endl;
			word_area_rect = rect.boundingRect();
			rectangle(adaptive_image, word_area_rect, Scalar(255), 1);
			break;
		}

		// 画出所有轮廓
//		for (int j = 0; j <= 3; j++)
//		{
//			line(contours_image, P[j], P[(j + 1) % 4], Scalar(255), 2);
//		}

	}
//	imshow("contours", contours_image); //轮廓

	imshow("src_id_image", adaptive_image);
	red_mark_area_->RotateImage(src_image, src_image, red_mark_area_->GetAngle()*-1);
	if (word_area_rect.y < 0)
	{
		word_area_rect.y = 0;
	}
	Mat idAreaRoi = src_image(word_area_rect);
//	imshow("id area roi", idAreaRoi);
	return idAreaRoi;
}

vector<Mat> DrivingLicense::WordDivide(Mat image, string preStr)
{
	vector<Mat> result_word;
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

//	Mat src_id_image = image.clone();
//	cout << image.rows * image.cols << endl;
//	imshow("find countours", image)


	// 对数字区域进行二值化
	cvtColor(image, image, CV_BGR2GRAY, CV_8UC1);
	// 数字区域 = 255，其他 = 0
	adaptiveThreshold(image, image, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 25, 10);
//	imshow("word adaptive threshold", image);

	// output into image
	imwrite(PATH + "id.jpg", image);

	//图像的高和宽
	int height = image.rows;
	int width = image.cols;
	// 保存当前行的黑点数目
	int tmp = 0;
	// 保存每一行黑点数目的数组
	int *blackArray = new int[width];

	//循环访问图像数据，查找每一列的黑点的数目
	for (int col = 0; col < width; col++)
	{
		tmp = 0;
		for (int row = 0; row < height; row++)
		{
			if (image.at<uchar>(row, col) == 255)
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
	imshow("vertical", projImg);

	int startIndex = 0;
	int endIndex = 0;
	// 是否进入字符区域
	bool inWordArea = false;
	int count = 1;
	for (int col = 0; col < width; col++)
	{
		// 进入字符区
		if (!inWordArea && blackArray[col] != 0)
		{
			startIndex = col;
			inWordArea = true;
		}
		// 进入空白区
		else if (blackArray[col] == 0 && inWordArea)
		{
			int whiteStartCol = col;
			int whiteEndCol = col + 1;
			for (int j = whiteStartCol; j < width; j++)
			{
				if (blackArray[j] != 0)
				{
					whiteEndCol = j;
					break;
				}
			}
			inWordArea = false;
			if (whiteEndCol - whiteStartCol > width / 18)
			{
				endIndex = col;
			}
			else
			{
				endIndex = (whiteStartCol + whiteEndCol) / 2;
			}
			Point p1, p2;
			p1.x = endIndex;
			p1.y = 0;
			p2.x = endIndex;
			p2.y = height;
			line(image, p1, p2, Scalar(255), 1);
			Mat roiImg = image(Range(0, height), Range(startIndex, endIndex));
			result_word.push_back(roiImg);

//			imshow(to_string(count), roiImg);
			count++;
		}
		// last character
		else if (col == width - 1 && inWordArea)
		{
			Mat roiImg = image(Range(0, height), Range(endIndex, width - 1));
			result_word.push_back(roiImg);

//			imshow(to_string(count), roiImg);
		}
	}
	imshow("show divided id area", image);
	return result_word;
}

void DrivingLicense::OutputFile(vector<Mat> &v, string prefix)
{
	cout << prefix << "size: " << v.size() << endl;
	for (int i = 0; i < v.size(); i++)
	{
		Mat c = v[i];
		string str = to_string(i+1);
		imwrite(PATH + prefix + str + ".jpg", c);
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





bool DrivingLicense::IsRectOverLap(Rect rect1, Rect rect2)
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

double DrivingLicense::RectOverLapCoefficient(Rect rect1, Rect rect2)
{
	CV_Assert(rect1.area() != 0 && rect2.area() != 0);
	double rate = 0.0;
	int flag = IsRectOverLap(rect1, rect2);
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

