#include <iostream>
#include <opencv2/opencv.hpp>
#include "DrivingLicense.h"
using namespace std;
using namespace cv;

int main()
{
	// red area problem: 1.1
	// 1.3 --- 70
	Mat srcImage = imread("/Users/whstarlit/Documents/Projects/Git/DrivingLicense/testImg/1.1.jpg");// 1.1 ~ 1.3
//	Mat srcImage = imread("/Users/whstarlit/Documents/Projects/Git/DrivingLicense/testImg/2.0.jpg");// 2.0 ~ 2.2
	if (srcImage.empty())
	{
		cout << "error";
		return -1;
	}

	DrivingLicense *driverLicense = new DrivingLicense(srcImage);

	waitKey(0);
	return 0;
}



