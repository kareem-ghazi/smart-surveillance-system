#include "Face.h"

Face::Face(Mat faceMatrix) : faceMatrix(faceMatrix)
{ 
	this->faceID = generateFaceID();
	this->faceLabel = "Face " + faceID;
}

Face::Face(Mat faceMatrix, string faceLabel) : faceMatrix(faceMatrix), faceLabel(faceLabel)
{
	this->faceID = generateFaceID();
}

Mat Face::getFaceMatrix() const
{
	return this->faceMatrix.clone();
}

void Face::setFaceMatrix(Mat faceMatrix)
{
	this->faceMatrix = faceMatrix.clone();
}

string Face::getFaceLabel() const
{
	return this->faceLabel;
}

void Face::setFaceLabel(string faceLabel)
{
	this->faceLabel = faceLabel;
}

int Face::generateFaceID()
{
	srand(time(0));

	int newFaceID = rand() % (999999 - 100000 + 1) + 100000;
	
	return newFaceID;
}
