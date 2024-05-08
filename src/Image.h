#ifndef IMAGE_H
#define IMAGE_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

class Image
{
private:
	cv::Mat imageMatrix;

public:
	Image();
	Image(cv::Mat imageMatrix);

	cv::Mat getImageMatrix() const;
	void setImageMatrix(cv::Mat imageMatrix);
};

#endif