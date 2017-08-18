#include <iostream>
#include <opencv2/opencv.hpp>
#include "DrivingLicense.h"

using namespace std;
using namespace cv;

int main()
{
	Mat srcImage = imread("/Users/whstarlit/Documents/Projects/Git/DrivingLicense/testImg/1.1.jpg");// 1.1 ~ 1.4
	if (srcImage.empty())
	{
		cout << "error";
		return -1;
	}

	cout << "default image size: " << srcImage.cols << " x " << srcImage.rows << endl;
//    resize(srcImage, srcImage, Size(srcImage.cols * 0.2, srcImage.rows * 0.2));
	while (srcImage.cols >= 1500 || srcImage.rows >= 1000)
	{
		resize(srcImage, srcImage, Size(srcImage.cols * 0.9, srcImage.rows * 0.9));
	}
	cout << "source image size: " << srcImage.cols << " x " << srcImage.rows << endl;
//	imshow("src", srcImage);


	DrivingLicense *driverLicense = new DrivingLicense(srcImage);
	waitKey(0);
	return 0;
}

