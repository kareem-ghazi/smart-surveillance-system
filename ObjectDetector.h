#ifndef OBJECTDETECTOR_H
#define OBJECTDETECTOR_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/face.hpp>

#include <iostream>
#include <cassert>

#include "Image.h"
#include "ImageProcessor.h"
#include "Face.h"
#include "Database.h"

using namespace std;
using namespace cv;

class ObjectDetector
{
private:
	Ptr<face::EigenFaceRecognizer> model = face::EigenFaceRecognizer::create();
	CascadeClassifier objectCascade;
	Database database;

public:
	ObjectDetector(string cascadePath);
	ObjectDetector(string cascadePath, Database database);

	void detect(Image image, vector<Rect> &objects);
	void trainModel();
	string recognize(Image image);
};

#endif

