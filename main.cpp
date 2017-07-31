#include <iostream>
#include <opencv2/opencv.hpp>
#include "DriverLicense.h"
using namespace std;
using namespace cv;

int main()
{
	// problem: 1.1
	Mat srcImage = imread("/Users/whstarlit/Documents/Projects/Git/DrivingLicenseIdentify/testImg/1.2.jpg");// 1.0 ~ 1.3
//	Mat srcImage = imread("/Users/whstarlit/Documents/Projects/Git/DriverLicenseIdentify/testImg/2.0.jpg");// 2.0 ~ 2.2
	if (srcImage.empty())
	{
		cout << "error";
		return -1;
	}

//	DriverLicense *driverLicense = new DriverLicense(srcImage);
//	imshow("src image", srcImage);


//	Mat srcGray;
//	cvtColor( srcImage, srcGray, CV_BGR2GRAY );
//	/// Parameters for Shi-Tomasi algorithm
//	vector<Point2f> corners;
//	double qualityLevel = 0.1;
//	double minDistance = 10;
//	int maxCorners = 4;
//	int blockSize = 3;
//	bool useHarrisDetector = false;
//	double k = 0.04;
//	/// Copy the source image
//	Mat cormat;
//	/// Apply corner detection :Determines strong corners on an image.
//	goodFeaturesToTrack(srcGray, corners, maxCorners, qualityLevel, minDistance, Mat(), blockSize, useHarrisDetector,
//						k);
//	/// Draw corners detected
//	for (int i = 0; i < corners.size(); i++)
//	{
//		circle(srcImage, corners[i], 4, Scalar(255), 2, 8, 0);
//	}
//
//	/// Showing the result
//	imshow("src", srcImage);
	waitKey(0);
	return 0;
}



