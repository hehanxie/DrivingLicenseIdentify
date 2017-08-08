#include <iostream>
#include <opencv2/opencv.hpp>
#include "DrivingLicense.h"
using namespace std;
using namespace cv;

int main()
{
	// red area problem: 1.1
	// 1.3 --- 70
	Mat srcImage = imread("/Users/whstarlit/Documents/Projects/Git/DrivingLicense/testImg/1.5.jpg");// 1.1 ~ 1.3
	if (srcImage.empty())
	{
		cout << "error";
		return -1;
	}

//	resize(srcImage, srcImage, Size(srcImage.cols * 0.2, srcImage.rows * 0.2));
//	imshow("srcImage", srcImage);
	DrivingLicense *driverLicense = new DrivingLicense(srcImage);

	waitKey(0);
	return 0;
}



