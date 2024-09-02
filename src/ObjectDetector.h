#ifndef OBJECTDETECTOR_H
#define OBJECTDETECTOR_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/face.hpp>

#include <iostream>
#include <cassert>

#include "Database.h"

class ObjectDetector
{
private:
	cv::Ptr<cv::face::EigenFaceRecognizer> model = cv::face::EigenFaceRecognizer::create();
	cv::CascadeClassifier objectCascade;
	Database& database;

public:
	ObjectDetector();
	ObjectDetector(std::string cascadePath);
	ObjectDetector(std::string cascadePath, Database& database);

    void process(cv::Mat& imageMatrix, std::vector<cv::Rect>& foundFaces);
    void process(cv::Mat& imageMatrix, std::vector<std::string>& foundLabels, std::vector<cv::Rect>& foundFaces);
	void detect(cv::Mat image, std::vector<cv::Rect> &objects);
    std::string recognize(cv::Mat image);
	void trainModel();
};

#endif

