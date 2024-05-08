#ifndef FACE_H
#define FACE_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include "Image.h"

class Face
{
private:
	cv::Mat faceMatrix;
	std::string faceLabel;
	int faceID;
	
	int generateFaceID();

public:
	Face(cv::Mat faceMatrix);
	Face(cv::Mat faceMatrix, std::string faceLabel);

	cv::Mat getFaceMatrix() const;
	void setFaceMatrix(cv::Mat getFaceMatrix);
	std::string getFaceLabel() const;
	void setFaceLabel(std::string faceLabel);
};

#endif
