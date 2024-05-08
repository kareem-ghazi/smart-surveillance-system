#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include "Image.h"

static class ImageProcessor
{
private:
	static Image lastProcessedImage;

public:
	static Image grayscale(Image image);
	static Image equalizeHistogram(Image image);
	static Image resize(Image image, int width, int height);
	static Image crop(Image image, cv::Point2i pointOne, cv::Point2i pointTwo);

	static void drawRectangle(Image& image, cv::Point pointOne, cv::Point pointTwo, char rectColor);
	static void putText(Image& image, int theX, int theY, 
		char fontColor, double fontScale, std::string text);
};

#endif
