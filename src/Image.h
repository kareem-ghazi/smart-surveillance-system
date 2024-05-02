#ifndef IMAGE_H
#define IMAGE_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;

class Image
{
private:
	Mat imageMatrix;

public:
	Image();
	Image(Mat imageMatrix);

	Mat getImageMatrix() const;
	void setImageMatrix(Mat imageMatrix);
};

#endif