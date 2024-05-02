#ifndef FACE_H
#define FACE_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include "Image.h"

using namespace std;
using namespace cv;

class Face
{
private:
	Mat faceMatrix;
	string faceLabel;
	int faceID;
	
	int generateFaceID();

public:
	Face(Mat faceMatrix);
	Face(Mat faceMatrix, string faceLabel);

	Mat getFaceMatrix() const;
	void setFaceMatrix(Mat getFaceMatrix);
	string getFaceLabel() const;
	void setFaceLabel(string faceLabel);
};

#endif
