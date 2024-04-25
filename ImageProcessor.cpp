#include "ImageProcessor.h"

Image ImageProcessor::grayscale(Image image)
{
	Mat imageMatrix;
	
	cvtColor(image.getImageMatrix(), imageMatrix, COLOR_BGR2GRAY);

	return Image(imageMatrix);
}

Image ImageProcessor::equalizeHistogram(Image image)
{
	Mat imageMatrix;

	equalizeHist(image.getImageMatrix(), imageMatrix);

	return Image(imageMatrix);
}

// didnt implement gpu overload.
Image ImageProcessor::resize(Image image, int width, int height)
{
	Mat imageMatrix;

	cv::resize(image.getImageMatrix(), imageMatrix, Size(128, 128), 0, 0, INTER_LINEAR);

	return Image(imageMatrix);
}

// didnt implement gpu overload.
Image ImageProcessor::crop(Image image, Point2i pointOne, Point2i pointTwo)
{
	Mat imageMatrix = image.getImageMatrix();
	Rect ROI(pointOne, pointTwo);

	return Image(imageMatrix(ROI)); // shit code, dont forget to rectify.
}

void ImageProcessor::drawRectangle(Image& image, Point pointOne, Point pointTwo, char rectColor)
{
	Mat imageMatrix = image.getImageMatrix();

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
		break;
	}

	image.setImageMatrix(imageMatrix);
}

void ImageProcessor::putText(Image& image, int theX, int theY, char fontColor, double fontScale, string text)
{
	Mat imageMatrix = image.getImageMatrix();

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
		break;
	}

	image.setImageMatrix(imageMatrix);
}
