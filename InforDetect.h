//
// Created by Nil on 2017/7/25.
//

#ifndef DRIVERLICENSEIDENTIFY_INFORDETECT_H
#define DRIVERLICENSEIDENTIFY_INFORDETECT_H

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class InforDetect
{
public:
	void colorMatch(Mat src);
	void setROI(Mat src);
};


#endif //DRIVERLICENSEIDENTIFY_INFORDETECT_H
