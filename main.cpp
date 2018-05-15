#include <iostream>
#include <opencv2/opencv.hpp>
#include "DrivingLicense.h"

using namespace std;
using namespace cv;

int main()
{
//	string Path = "/Users/whstarlit/Documents/Projects/Git/DrivingLicense/exampleImage/";
//	string str = "";
//	for (int i = 1; i <=9; i++)
//	{
//		Path = "/Users/whstarlit/Documents/Projects/Git/DrivingLicense/exampleImage/";
//		str = std::to_string(i);
//		Path = Path + str + ".jpg";
//		Mat srcImage = imread(Path);
//		if (srcImage.empty())
//		{
//			cout << Path << "error";
//			return -1;
//		}
//
//		cout << "default image size: " << srcImage.cols << " x " << srcImage.rows << endl;
//		while (srcImage.cols >= 1500 || srcImage.rows >= 1000)
//		{
//			resize(srcImage, srcImage, Size(srcImage.cols * 0.9, srcImage.rows * 0.9));
//		}
//		cout << "source image size: " << srcImage.cols << " x " << srcImage.rows << endl;
//		imshow("src", srcImage);
//
//		DrivingLicense *driverLicense = new DrivingLicense(srcImage);
//
//		// use command line tool to execute python
//		system("python /Users/whstarlit/Documents/Projects/Git/DrivingLicense/Tesseract.py");
//	}

	Mat srcImage = imread("/Users/whstarlit/Documents/Projects/Git/DrivingLicense/exampleImage/1.jpg");
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
	imshow("src", srcImage);

	DrivingLicense *driverLicense = new DrivingLicense(srcImage);

	// use command line tool to execute python
	system("python /Users/whstarlit/Documents/Projects/Git/DrivingLicense/Tesseract.py");
	waitKey(0);
	return 0;
}

