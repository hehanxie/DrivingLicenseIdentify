#include <iostream>
#include <opencv2/opencv.hpp>
#include "DrivingLicense.h"
using namespace std;
using namespace cv;

int main()
{
	// red area problem: 1.1
	// 1.3 --- 703
	Mat srcImage = imread("/Users/whstarlit/Documents/Projects/Git/DrivingLicense/testImg/1.4.jpg");// 1.1 ~ 1.4
	if (srcImage.empty())
	{
		cout << "error";
		return -1;
	}

	resize(srcImage, srcImage, Size(srcImage.cols * 0.2, srcImage.rows * 0.2));
//	imshow("src", srcImage);

	cv::Mat_<cv::Vec3b>::iterator it = srcImage.begin<cv::Vec3b>();    //由于利用图像迭代器处理图像像素，因此返回类型必须在编译时知道
	cv::Mat_<cv::Vec3b>::iterator itend = srcImage.end<cv::Vec3b>();
	int div = 64;
	for(;it != itend; ++it)
	{
		(*it)[0] = (*it)[0]/div*div+div/2;        //利用operator[]处理每个通道的像素
		(*it)[1] = (*it)[1]/div*div+div/2;
		(*it)[2] = (*it)[2]/div*div+div/2;
		cout << (*it)[0] << endl;
	}



//	DrivingLicense *driverLicense = new DrivingLicense(srcImage);

//	waitKey(0);
	return 0;
}



