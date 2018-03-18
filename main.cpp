#include <iostream>
#include <opencv2/opencv.hpp>
#include "DrivingLicense.h"

using namespace std;
using namespace cv;

int main()
{
	// 4,6,7
	Mat srcImage = imread("/Users/whstarlit/Documents/Projects/Git/DrivingLicense/exampleImage/9.2.jpg");
	if (srcImage.empty())
	{
		cout << "error";
		return -1;
	}

	cout << "default image size: " << srcImage.cols << " x " << srcImage.rows << endl;
//    resize(src_image_, src_image_, Size(src_image_.cols * 0.2, src_image_.rows * 0.2));
	while (srcImage.cols >= 1500 || srcImage.rows >= 1000)
	{
		resize(srcImage, srcImage, Size(srcImage.cols * 0.9, srcImage.rows * 0.9));
	}
	cout << "source image size: " << srcImage.cols << " x " << srcImage.rows << endl;
//	imshow("src", src_image_);

	DrivingLicense *driverLicense = new DrivingLicense(srcImage);
	waitKey(0);
	return 0;
}

