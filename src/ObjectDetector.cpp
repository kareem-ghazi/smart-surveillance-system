#include "ObjectDetector.h"

// Initializes the object detector with the path to a cascade classifier.
ObjectDetector::ObjectDetector(string cascadePath)
{
	try
	{
		this->model->read("./data/eigenface.yml");
	}
	catch (const std::exception&)
	{
		if (!database.getIDs().empty()) // shit code.
		{
			this->trainModel();
			this->model->read("./data/eigenface.yml");
		}
		else {
			cout << "Error: Couldn't create eigenface.yml. No data exists!" << endl;
		}
	}

	this->objectCascade.load(cascadePath);

	if (this->objectCascade.empty())
	{
		cout << "Error: Cascade classifer does not exist." << endl;
		assert(false);
	}
}

ObjectDetector::ObjectDetector(string cascadePath, Database database)
{
	this->database = database;
	
	try
	{
		this->model->read("./data/eigenface.yml");
	}
	catch (const std::exception&)
	{
		if (!database.getIDs().empty()) // shit code.
		{
			this->trainModel();
			this->model->read("./data/eigenface.yml");
		}
		else {
			cout << "Error: Couldn't create eigenface.yml. No data exists!" << endl;
		}
	}

	this->objectCascade.load(cascadePath);

	if (this->objectCascade.empty())
	{
		cout << "Error: Cascade classifer does not exist." << endl;
		assert(false);
	}
}

void ObjectDetector::detect(Image image, vector<Rect> &objects)
{
	this->objectCascade.detectMultiScale(image.getImageMatrix(), objects, 1.1, 10, 0 | CASCADE_SCALE_IMAGE, Size(20, 20));
}

void ObjectDetector::trainModel()
{
	database.loadEntries();
	
	if (database.getIDs().empty())
	{
		return;
	}

	model->train(database.getMatrices(), database.getIDs());
	model->save("./data/eigenface.yml");
}

/*
* Recognize an image with a face.
* 
* Only works with a grayscale image that has a size of 127 x 127 pixels.
*/
string ObjectDetector::recognize(Image image)
{
	if (database.getIDs().empty()) // shit code!
	{
		return "";
	}

	int id = -1;
	double confidence = 0.0;

	//image.setImageMatrix(ImageProcessor::grayscale(image).getImageMatrix());
	model->predict(image.getImageMatrix(), id, confidence);

	//cout << confidence << endl;

	if (confidence >= 3750)
	{
		return "";
	}

	return database.getLabels().at(id);
}



