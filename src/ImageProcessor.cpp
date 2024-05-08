#include "ImageProcessor.h"

// Converts image to grayscale and returns a copy of it.
Image ImageProcessor::grayscale(Image image)
{
	cv::Mat imageMatrix;
	
	cvtColor(image.getImageMatrix(), imageMatrix, cv::COLOR_BGR2GRAY);

	return Image(imageMatrix);
}

// Equalizes the histogram of an image and returns a copy of it.
Image ImageProcessor::equalizeHistogram(Image image)
{
	cv::Mat imageMatrix;

	equalizeHist(image.getImageMatrix(), imageMatrix);

	return Image(imageMatrix);
}

// Resizes image and returns a copy of the image.
Image ImageProcessor::resize(Image image, int width, int height)
{
	cv::Mat imageMatrix;

	cv::resize(image.getImageMatrix(), imageMatrix, cv::Size(128, 128), 0, 0, cv::INTER_LINEAR);

	return Image(imageMatrix);
}

// Crops the image for a defined set of points and returns a copy of the image.
Image ImageProcessor::crop(Image image, cv::Point2i pointOne, cv::Point2i pointTwo)
{
	cv::Mat imageMatrix = image.getImageMatrix();
	cv::Rect ROI(pointOne, pointTwo);

	return Image(imageMatrix(ROI));
}

// Draws a rectangle on an area with a defined set of points on the original passed image.
void ImageProcessor::drawRectangle(Image& image, cv::Point pointOne, cv::Point pointTwo, char rectColor)
{
	cv::Mat imageMatrix = image.getImageMatrix();

	// Decide the color of the rectangle.
	switch (rectColor)
	{
	case 'R':
		cv::rectangle(imageMatrix, pointOne, pointTwo, cv::Scalar(0, 0, 255), 2, 8, 0);
		break;
	case 'G':
		cv::rectangle(imageMatrix, pointOne, pointTwo, cv::Scalar(0, 255, 0), 2, 8, 0);
		break;
	case 'B':
		cv::rectangle(imageMatrix, pointOne, pointTwo, cv::Scalar(255, 0, 0), 2, 8, 0);
		break;
	default:
		cv::rectangle(imageMatrix, pointOne, pointTwo, cv::Scalar(0, 0, 255), 2, 8, 0);
		std::cout << "Error: Couldn't draw rectangle with the specified color. Defaulting to RED." << std::endl;
		break;
	}

	image.setImageMatrix(imageMatrix);
}

// Places text on the original passed image.
void ImageProcessor::putText(Image& image, int theX, int theY, char fontColor, double fontScale, std::string text)
{
	cv::Mat imageMatrix = image.getImageMatrix();

	// Decides the font color based on what was passed.
	switch (fontColor)
	{
	case 'R':
		cv::putText(imageMatrix, text, cv::Point(theX, theY), cv::FONT_HERSHEY_DUPLEX, fontScale, cv::Scalar(0, 0, 255), 1.0);
		break;
	case 'G':
		cv::putText(imageMatrix, text, cv::Point(theX, theY), cv::FONT_HERSHEY_DUPLEX, fontScale, cv::Scalar(0, 255, 0), 1.0);
		break;
	case 'B':
		cv::putText(imageMatrix, text, cv::Point(theX, theY), cv::FONT_HERSHEY_DUPLEX, fontScale, cv::Scalar(255, 0, 0), 1.0);
		break;
	default:
		cv::putText(imageMatrix, text, cv::Point(theX, theY), cv::FONT_HERSHEY_DUPLEX, fontScale, cv::Scalar(0, 0, 255), 1.0);
		std::cout << "Error: Couldn't place text with the specified color. Defaulting to RED." << std::endl;
		break;
	}

	image.setImageMatrix(imageMatrix);
}
