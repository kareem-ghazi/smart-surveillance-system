#include "Face.h"

Face::Face(cv::Mat faceMatrix) : faceMatrix(faceMatrix)
{ 
	this->faceID = generateFaceID();
	this->faceLabel = "Face " + faceID;
}

Face::Face(cv::Mat faceMatrix, std::string faceLabel) : faceMatrix(faceMatrix), faceLabel(faceLabel)
{
	this->faceID = generateFaceID();
}

cv::Mat Face::getFaceMatrix() const
{
	return this->faceMatrix.clone();
}

void Face::setFaceMatrix(cv::Mat faceMatrix)
{
	this->faceMatrix = faceMatrix.clone();
}

std::string Face::getFaceLabel() const
{
	return this->faceLabel;
}

void Face::setFaceLabel(std::string faceLabel)
{
	this->faceLabel = faceLabel;
}

int Face::generateFaceID()
{
	srand(time(0));

	int newFaceID = rand() % (999999 - 100000 + 1) + 100000;
	
	return newFaceID;
}
