#include "ObjectDetector.h"

ObjectDetector::ObjectDetector(string cascadePath)
{
	this->objectCascade.load(cascadePath);

	if (this->objectCascade.empty())
	{
		cout << "Error: Cascade classifer does not exist." << endl;
		assert(false);
	}
}

void ObjectDetector::detect(Image image, vector<Rect> &objects)
{
	this->objectCascade.detectMultiScale(image.getImageMatrix(), objects, 1.1, 10, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
}



