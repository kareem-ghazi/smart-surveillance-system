#include "Image.h"

Image::Image()
{

}

// Initializes the image matrix with a copy of the passed image matrix.
Image::Image(Mat imageMatrix) : imageMatrix(imageMatrix.clone())
{

}

// Returns a copy of the image matrix.
Mat Image::getImageMatrix() const
{
	return this->imageMatrix.clone();
}

// Sets a copy of the image matrix as the new image matrix.
void Image::setImageMatrix(Mat imageMatrix)
{
	this->imageMatrix = imageMatrix.clone();
}