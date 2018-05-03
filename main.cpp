#include <iostream>
#include <opencv2/opencv.hpp>
#include "DrivingLicense.h"

using namespace std;
using namespace cv;

int main()
{

	Mat srcImage = imread("/Users/whstarlit/Documents/Projects/Git/DrivingLicense/exampleImage/9.jpg");
	if (srcImage.empty())
	{
		cout << "error";
		return -1;
	}

	cout << "default image size: " << srcImage.cols << " x " << srcImage.rows << endl;
	while (srcImage.cols >= 1500 || srcImage.rows >= 1000)
	{
		resize(srcImage, srcImage, Size(srcImage.cols * 0.9, srcImage.rows * 0.9));
	}
	cout << "source image size: " << srcImage.cols << " x " << srcImage.rows << endl;
//	imshow("src", srcImage);

	DrivingLicense *driverLicense = new DrivingLicense(srcImage);

	// use command line tool to execute python
	system("python /Users/whstarlit/Documents/Projects/Git/DrivingLicense/Tesseract.py");
	//waitKey(0);
	return 0;
}

