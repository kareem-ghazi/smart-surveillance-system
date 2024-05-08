#include "ObjectDetector.h"

// Initializes the object detector with the path to a cascade classifier.
ObjectDetector::ObjectDetector(std::string cascadePath)
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
			std::cout << "Error: Couldn't create eigenface.yml. No data exists!" << std::endl;
		}
	}

	this->objectCascade.load(cascadePath);

	if (this->objectCascade.empty())
	{
		std::cout << "Error: Cascade classifer does not exist." << std::endl;
		assert(false);
	}
}

// Initializes object detector with a path to the cascade classifier and a database.
ObjectDetector::ObjectDetector(std::string cascadePath, Database database)
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
			std::cout << "Error: Couldn't create eigenface.yml. No data exists!" << std::endl;
		}
	}

	this->objectCascade.load(cascadePath);

	if (this->objectCascade.empty())
	{
		std::cout << "Error: Cascade classifer does not exist." << std::endl;
		assert(false);
	}
}

// Detects an object using the Cascade Classifer.
void ObjectDetector::detect(Image image, std::vector<cv::Rect> &objects)
{
	this->objectCascade.detectMultiScale(image.getImageMatrix(), objects, 1.1, 10, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(20, 20));
}

// Trains an eigenface model with the entries in the database.
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

// Recognize an image with a face.
// NOTE: Only works with a grayscale image that has a size of 127 x 127 pixels.
std::string ObjectDetector::recognize(Image image)
{
	if (database.getIDs().empty() || model.empty()) // shit code!
	{
		return "";
	}

	int id = -1;
	double confidence = 0.0;

	model->predict(image.getImageMatrix(), id, confidence);

	if (confidence >= 4000)
	{
		return "";
	}

	return database.getLabels().at(id);
}



