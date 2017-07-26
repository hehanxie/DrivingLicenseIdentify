#include <iostream>
#include <opencv2/opencv.hpp>
#include "InformationLocate.h"
#include "InforDetect.h"

using namespace std;
using namespace cv;

int main()
{
	// get source image
	Mat srcImage = imread("/Users/whstarlit/Documents/Projects/Git/DriverLicenseIdentify/img/1.jpg");
//	Mat srcImage = imread("/Users/whstarlit/Documents/Projects/Git/DriverLicenseIdentify/img/4.jpg", CV_8UC1);
	if (srcImage.empty())
	{
		cout << "error";
		return -1;
	}
//	imshow("src image", srcImage);
	InformationLocate *informationLocate = new InformationLocate();
	informationLocate->locateBorder(srcImage);
	informationLocate->colorMatch(srcImage);

//

//	Mat kernel = (Mat_<int>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
//	filter2D(srcImage, dstImage, srcImage.depth(), kernel);
//	imshow("dst image", dstImage);

//	InforDetect *inforDetect = new InforDetect();
//	inforDetect->colorMatch(srcImage);

//	Mat dst;
//	adaptiveThreshold(srcImage, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 7, 7);
//
//	Mat blur;
//	medianBlur(dst, blur, 3);

	waitKey(0);
	return 0;
}



