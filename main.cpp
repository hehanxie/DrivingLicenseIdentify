#include <iostream>
#include <opencv2/opencv.hpp>
#include "DriverLicense.h"
using namespace std;
using namespace cv;

int main()
{
	// problem: 1.1, 1.3
	Mat srcImage = imread("/Users/whstarlit/Documents/Projects/Git/DriverLicenseIdentify/testImg/1.3.jpg");// 1.0 ~ 1.3
//	Mat srcImage = imread("/Users/whstarlit/Documents/Projects/Git/DriverLicenseIdentify/testImg/2.0.jpg");// 2.0 ~ 2.2
	if (srcImage.empty())
	{
		cout << "error";
		return -1;
	}

	DriverLicense *driverLicense = new DriverLicense(srcImage);
//	imshow("src image", srcImage);
	waitKey(0);
	return 0;
}



