#include <iostream>
#include <opencv2/opencv.hpp>
#include "InformationLocate.h"
#include "InforDetect.h"

using namespace std;
using namespace cv;

int main()
{
	// get source image
//	Mat srcImage = imread("/Users/whstarlit/Documents/Projects/Git/DriverLicenseIdentify/img/1.jpg");
	Mat srcImage = imread("/Users/whstarlit/Documents/Projects/Git/DriverLicenseIdentify/img/4.jpg");
	if (srcImage.empty())
	{
		cout << "error";
		return -1;
	}
//	imshow("src image", srcImage);
//	Mat dstImage;
//	InformationLocate *inforLocate = new InformationLocate();
//	int flag;
//	flag = inforLocate->locateBorder(image);
//	cout << "border width: " << inforLocate->getBorderWidth() << endl;
//	cout << "border height: " << inforLocate->getBorderHeight() << endl;
//	flag = inforLocate->locateInfor(image);

//	medianBlur(srcImage, dstImage, 3);


//	Mat kernel = (Mat_<int>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
//	filter2D(srcImage, dstImage, srcImage.depth(), kernel);
//	imshow("dst image", dstImage);

	InforDetect *inforDetect = new InforDetect();
	inforDetect->colorMatch(srcImage);

	waitKey(0);
	return 0;
}



