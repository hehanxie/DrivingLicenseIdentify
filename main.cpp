#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

void test();
Mat OstuBeresenThreshold(Mat image);
bool isEligible(const RotatedRect &candidate);
bool isTable(const RotatedRect &candidate);
vector<Point> getBorder(Mat image);
vector<vector<Point>> getInfor(Mat image);
Mat histeq(Mat in);

int main()
{
	// get source image
	Mat image = imread("/Users/whstarlit/Documents/Projects/Git/DriverLicenseIdentify/img/1.jpg");
	if (image.empty())
	{
		cout << "error";
		return -1;
	}
	// change to gray image
	Mat srcImage;
	cvtColor(image, srcImage, COLOR_BGR2GRAY);
//	imshow("src", srcImage);

	//vector<Point> border = getBorder(image);
	vector<vector<Point>> target = getInfor(image);





//
	waitKey(0);
	return 0;
}

Mat OstuBeresenThreshold(Mat image)
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

bool isEligible(const RotatedRect &candidate)
{
	// 宽高比 --- 大边框
	float ratio = 860 / 600;
	float error = 0.05;
	// get area
	int area = candidate.size.height * candidate.size.width;
	float areaRatio = candidate.size.width / candidate.size.height;
//	cout << "area: " << area << endl;
//	cout << "areaRatio: " << areaRatio << endl << endl;
	if (area > 130000)
	{
		cout << "width: " << candidate.size.width << endl;
		cout << "height: " << candidate.size.height << endl;
		cout << "area: " << area << endl;
		cout << "areaRatio: " << areaRatio << endl;
		return true;
	}
	else
	{
		return false;
	}

}

bool isTable(const RotatedRect &candidate)
{

	return false;
}

vector<Point> getBorder(Mat image)
{
	Mat srcImage;
	cvtColor(image, srcImage, COLOR_BGR2GRAY);
	if (srcImage.empty())
	{
		cout << "error";
	}

	// ostu 和 beresen算法，重新生成图像
	Mat ostuBeresenImage = OstuBeresenThreshold(srcImage);
//	imshow("ostu beresen image", ostuBeresenImage);

	// closed处理
	Mat element = getStructuringElement(MORPH_RECT ,Size(3 ,3));
	Mat closedImage;
	//进行形态学操作
	morphologyEx(ostuBeresenImage, closedImage, MORPH_CLOSE, element);
//	morphologyEx(binaryImage, openImage, MORPH_OPEN, element);
//	imshow("closed image", closedImage);

	// 膨胀处理
	Mat element1 = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat dilateImage;
	dilate(closedImage, dilateImage, element1);
//	imshow("dilate image", dilateImage);

	// 画边缘
	vector<vector<Point> > contours;
	findContours(dilateImage, contours, CV_RETR_EXTERNAL , CV_CHAIN_APPROX_NONE);
	vector< vector <Point> > ::iterator itc = contours.begin();

	Mat dst = image;
	int index = 0;
	while( itc != contours.end())
	{
		// 获取每个轮廓的最小有界矩形区域
		RotatedRect mr = minAreaRect(Mat(*itc));
		if (isEligible(mr))
		{
			cout << "index: " << index << endl;
			vector<Point> cnt = contours[index];
			float epsilon = 0.1 * arcLength(cnt, true);
			approxPolyDP(cnt, contours[index], epsilon, true);
			// 对画出的边重新矫正
			drawContours(dst, contours, index, Scalar(0, 0, 255), 2);
			cout << contours[index] << endl;

		}
		itc++;
		index++;
	}
	imshow("draw contours", dst);
	waitKey(0);
	return contours[index];
}

vector<vector<Point>> getInfor(Mat image)
{
	Mat srcImage;
	cvtColor(image, srcImage, COLOR_BGR2GRAY);
	if (srcImage.empty())
	{
		cout << "error";
	}
	Mat blurImage;
	blur(srcImage, srcImage, Size(5,5));

	Mat sobelImage;
	Sobel(srcImage, sobelImage, CV_8U, 1, 0, 3, 1, 0, BORDER_DEFAULT);//xorder=1,yorder=0,kernelsize=3

	Mat thresholdImage;
	threshold(sobelImage, thresholdImage, 0, 255, CV_THRESH_OTSU+CV_THRESH_BINARY);

	Mat element = getStructuringElement(MORPH_RECT, Size(15, 5) );
	morphologyEx(thresholdImage, thresholdImage, CV_MOP_CLOSE, element);
	imshow("threshold image2", thresholdImage);

	//Find 轮廓 of possibles plates
	vector< vector< Point> > contours;
	findContours(thresholdImage,
				 contours, // a vector of contours
				 CV_RETR_EXTERNAL, // 提取外部轮廓
				 CV_CHAIN_APPROX_NONE); // all pixels of each contours

	//Start to iterate to each contour founded
	vector<vector<Point> >::iterator itc= contours.begin();
	Mat dst = image;
	//drawContours(image, contours, -1, (0, 0, 255), 1);

	// 近似后的轮廓点集
	vector<vector<Point> > contours_poly( contours.size() );
	// 包围点集的最小矩形vector
	vector<Rect> boundRect(contours.size());
	int index = 0;
	while( itc != contours.end())
	{
		if (true)
		{
			vector<Point> cnt = contours[index];
			float epsilon = 0.1 * arcLength(cnt, true);
			approxPolyDP(cnt, contours[index], epsilon, true);
			// 对画出的边重新矫正
			drawContours(dst, contours, index, Scalar(0, 0, 255), 2);
		}
		itc++;
		index++;
	}
	//imshow("draw", dst);



//	Mat ostuBeresenImage = OstuBeresenThreshold(srcImage);
////	imshow("ostu beresen image", ostuBeresenImage);
//
//	// closed处理
//	Mat element = getStructuringElement(MORPH_RECT ,Size(2 ,2));
//	Mat closedImage;
//	// 进行形态学操作
//	morphologyEx(ostuBeresenImage, closedImage, MORPH_CLOSE, element);
//	imshow("closed image", closedImage);
//
//	Mat histeqImage = histeq(closedImage);
//	imshow("histeq image", histeqImage);
//	// 画边缘
//	vector<vector<Point> > contours;
//	findContours(closedImage, contours, RETR_LIST , CV_CHAIN_APPROX_NONE);
//	vector< vector <Point> > ::iterator itc = contours.begin();
//	Mat dst = image;
//	//drawContours(image, contours, -1, (0, 0, 255), 1);
//
//	// 近似后的轮廓点集
//	vector<vector<Point> > contours_poly( contours.size() );
//	// 包围点集的最小矩形vector
//	vector<Rect> boundRect(contours.size());
//	int index = 0;
//	while( itc != contours.end())
//	{
//		// 获取每个轮廓的最小有界矩形区域
//		RotatedRect mr = minAreaRect(Mat(*itc));
//		//if (isTable(mr))
//		{
//			//对多边形曲线做适当近似，contours_poly[i]是输出的近似点集
//			approxPolyDP( Mat(contours[index]), contours_poly[index], 3, true );
//			boundRect[index] = boundingRect(Mat(contours_poly[index]));
//			//画矩形，tl矩形左上角，br右上角
//			rectangle(dst, boundRect[index].tl(), boundRect[index].br(), Scalar(0, 0, 255), 2, 8, 0 );
//
////			vector<Point> cnt = contours[index];
////			float epsilon = 0.1 * arcLength(cnt, true);
////			approxPolyDP(cnt, contours[index], epsilon, true);
////			// 对画出的边重新矫正
////			drawContours(dst, contours, index, Scalar(0, 0, 255), 2);
//		}
//		itc++;
//		index++;
//	}
//	imshow("draw", dst);
	vector<vector<Point>> target;
	waitKey(0);
	return target;
}

void test()
{
	// 对图像二值化
//	Mat binaryImage;
	// ostu算法,获取最佳阈值,并二值化
//	int thresholdValue = Ostu(srcImage);
//	threshold(srcImage, binaryImage, thresholdValue, 255, CV_THRESH_BINARY);
//	imshow("ostu image", binaryImage);


	// 用3*3内核来降噪
//	Mat blurImage;
//	blur(closedImage, blurImage, Size(3,3) );
//	imshow("blur image", blurImage);


	// border, and auto reverse color
//	Mat cannyImage;
//	Canny(closedImage, cannyImage, 150, 100,5);
//	imshow("canny image", cannyImage);

	// reverse color
//	Mat reverseImage = binaryImage < 255;
//	imshow("reverse image", dst);
}

//直方图均衡化
Mat histeq(Mat in)
{
	Mat out(in.size(), in.type());
	if(in.channels()==3)
	{
		Mat hsv;
		vector<Mat> hsvSplit;
		cvtColor(in, hsv, CV_BGR2HSV);
		split(hsv, hsvSplit);
		equalizeHist(hsvSplit[2], hsvSplit[2]);
		merge(hsvSplit, hsv);
		cvtColor(hsv, out, CV_HSV2BGR);
	}else if(in.channels()==1){
		equalizeHist(in, out);
	}
	return out;
}



