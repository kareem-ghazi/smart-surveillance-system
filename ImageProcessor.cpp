#include "ImageProcessor.h"

// Converts image to grayscale and returns a copy of it.
Image ImageProcessor::grayscale(Image image)
{
	Mat imageMatrix;
	
	cvtColor(image.getImageMatrix(), imageMatrix, COLOR_BGR2GRAY);

	return Image(imageMatrix);
}

// Equalizes the histogram of an image and returns a copy of it.
Image ImageProcessor::equalizeHistogram(Image image)
{
	Mat imageMatrix;

	equalizeHist(image.getImageMatrix(), imageMatrix);

	return Image(imageMatrix);
}

// Resizes image and returns a copy of the image.
Image ImageProcessor::resize(Image image, int width, int height)
{
	Mat imageMatrix;

	cv::resize(image.getImageMatrix(), imageMatrix, Size(128, 128), 0, 0, INTER_LINEAR);

	return Image(imageMatrix);
}

// Crops the image for a defined set of points and returns a copy of the image.
Image ImageProcessor::crop(Image image, Point2i pointOne, Point2i pointTwo)
{
	Mat imageMatrix = image.getImageMatrix();
	Rect ROI(pointOne, pointTwo);

	return Image(imageMatrix(ROI));
}

// Draws a rectangle on an area with a defined set of points on the original passed image.
void ImageProcessor::drawRectangle(Image& image, Point pointOne, Point pointTwo, char rectColor)
{
	Mat imageMatrix = image.getImageMatrix();

	// Decide the color of the rectangle.
	switch (rectColor)
	{
	case 'R':
		cv::rectangle(imageMatrix, pointOne, pointTwo, Scalar(0, 0, 255), 2, 8, 0);
		break;
	case 'G':
		cv::rectangle(imageMatrix, pointOne, pointTwo, Scalar(0, 255, 0), 2, 8, 0);
		break;
	case 'B':
		cv::rectangle(imageMatrix, pointOne, pointTwo, Scalar(255, 0, 0), 2, 8, 0);
		break;
	default:
		cv::rectangle(imageMatrix, pointOne, pointTwo, Scalar(0, 0, 255), 2, 8, 0);
		cout << "Error: Couldn't draw rectangle with the specified color. Defaulting to RED." << endl;
		break;
	}

	image.setImageMatrix(imageMatrix);
}

// Places text on the original passed image.
void ImageProcessor::putText(Image& image, int theX, int theY, char fontColor, double fontScale, string text)
{
	Mat imageMatrix = image.getImageMatrix();

	// Decides the font color based on what was passed.
	switch (fontColor)
	{
	case 'R':
		cv::putText(imageMatrix, text, Point(theX, theY), FONT_HERSHEY_DUPLEX, fontScale, Scalar(0, 0, 255), 1.0);
		break;
	case 'G':
		cv::putText(imageMatrix, text, Point(theX, theY), FONT_HERSHEY_DUPLEX, fontScale, Scalar(0, 255, 0), 1.0);
		break;
	case 'B':
		cv::putText(imageMatrix, text, Point(theX, theY), FONT_HERSHEY_DUPLEX, fontScale, Scalar(255, 0, 0), 1.0);
		break;
	default:
		cv::putText(imageMatrix, text, Point(theX, theY), FONT_HERSHEY_DUPLEX, fontScale, Scalar(0, 0, 255), 1.0);
		cout << "Error: Couldn't place text with the specified color. Defaulting to RED." << endl;
		break;
	}

	image.setImageMatrix(imageMatrix);
}
