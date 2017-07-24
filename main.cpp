#include <iostream>
#include <opencv2/opencv.hpp>
#include "InformationLocate.h"

using namespace std;
using namespace cv;

int main()
{
	// get source image
	Mat image = imread("/Users/whstarlit/Documents/Projects/Git/DriverLicenseIdentify/img/4.jpg");
	if (image.empty())
	{
		cout << "error";
		return -1;
	}

	InformationLocate *inforLocate = new InformationLocate();
	int flag;
	flag = inforLocate->locateBorder(image);
	cout << "border width: " << inforLocate->getBorderWidth() << endl;
	cout << "border height: " << inforLocate->getBorderHeight() << endl;
	flag = inforLocate->locateInfor(image);


	waitKey(0);
	return 0;
}



