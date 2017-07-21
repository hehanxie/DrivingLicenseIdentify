#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;


int Ostu(Mat image);
Mat OstuBeresenThreshold(Mat image);
bool isEligible(const RotatedRect &candidate);


int main()
{
	// get source image
	Mat srcImage = imread("/Users/whstarlit/Documents/Projects/Git/DriverLicenseIdentify/img/7.jpg", IMREAD_GRAYSCALE);

	if (srcImage.empty())
	{
		cout << "error";
		return -1;
	}
//	imshow("src", srcImage);

//	Mat binaryImage;
//	// 对图像二值化
//	// ostu算法,获取最佳阈值,并二值化
//	int thresholdValue = Ostu(srcImage);
//	threshold(srcImage, binaryImage, thresholdValue, 255, CV_THRESH_BINARY);
//	imshow("ostu image", binaryImage);



	// ostu 和 beresen算法，重新生成图像
	Mat ostuBeresenImage = OstuBeresenThreshold(srcImage);
	imshow("ostu beresen image", ostuBeresenImage);

//	Mat reverseImage = binaryImage < 255;
//	imshow("reverse image", dst);

//	// closed处理
//	Mat element = getStructuringElement(MORPH_RECT ,Size(3 ,3));
//	Mat closedImage;
//	//进行形态学操作
//	morphologyEx(binaryImage, closedImage, MORPH_CLOSE, element);
////	morphologyEx(binaryImage, openImage, MORPH_OPEN, element);
//	imshow("closed image", closedImage);
//
	// 用3*3内核来降噪
	Mat blurImage;
	blur(ostuBeresenImage, blurImage, Size(3,3) );
	imshow("blur image", blurImage);

	// 边缘提取(自动反色)
	Mat cannyImage;
	Canny(blurImage, cannyImage, 150, 100,5);
	imshow("canny image", cannyImage);

	// 画边缘
	vector<vector<Point> > contours;
	findContours(cannyImage, contours, RETR_LIST , CV_CHAIN_APPROX_NONE);
	vector< vector <Point> > ::iterator itc = contours.begin();
	Mat dst = Mat::zeros(cannyImage.size(), CV_8UC1);

	int index = 0;
	while( itc != contours.end())
	{
		// 获取每个轮廓的最小有界矩形区域
		RotatedRect mr = minAreaRect(Mat(*itc));
		if (isEligible(mr))
		{
			// cause problem
			drawContours(cannyImage, itc[index], 1, Scalar(0, 0, 255));
			break;
		}
		else
		{
			itc++;
			index++;
		}
	}


	imshow("draw contours", dst);


	waitKey(0);
	return 0;
}

int Ostu(Mat image)
{

	int width = image.cols;
	int height = image.rows;
	int x = 0, y = 0;
	int pixelCount[256] = {0};
	float pixelPro[256] = {0};
	int pixelSum = width * height, threshold = 0;

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
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
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
	}
	cout << "ostu" << endl;
	cout << "最佳阈值: " << threshold << endl << endl;
	return threshold;
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
			if(i < 2 ||  i > height - 3 || j < 2 || j > height - 3)
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
				rectAveThreshold = sum(image(Rect(i-2,j-2,5,5)))[0]/25;
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
	float ratio = 860/600;
	float error = 0.1;
	// get are
	int area = candidate.size.height * candidate.size.width;
	float areaRatio = candidate.size.width / candidate.size.height;

	if (abs(areaRatio - area) < error)
	{
		return true;
	}
	else
	{
		return false;
	}

}
