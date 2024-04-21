#ifndef OBJECTDETECTOR_H
#define OBJECTDETECTOR_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include <iostream>
#include <cassert>

#include "Image.h"
#include "Face.h"

using namespace std;
using namespace cv;

class ObjectDetector
{
private:
	CascadeClassifier objectCascade;

public:
	ObjectDetector(string cascadePath);

	void detect(Image image, vector<Rect> &objects);
	void recognizeFace(Face face);
};

#endif

