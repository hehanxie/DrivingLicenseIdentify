#include <iostream>
#include <opencv2/opencv.hpp>
#include "DrivingLicense.h"

using namespace std;
using namespace cv;

int main()
{
	// red area problem: 1.1
	// 1.3 --- 703
	Mat srcImage = imread("/Users/whstarlit/Documents/Projects/Git/DrivingLicense/testImg/1.1.jpg");// 1.1 ~ 1.4
	if (srcImage.empty())
	{
		cout << "error";
		return -1;
	}

//	resize(srcImage, srcImage, Size(srcImage.cols * 0.2, srcImage.rows * 0.2));
	cout << "source image size: " << srcImage.cols << " x " << srcImage.rows << endl;
//	imshow("src", srcImage);


	DrivingLicense *driverLicense = new DrivingLicense(srcImage);
	waitKey(0);
	return 0;
}

