#include "ObjectDetector.h"

//// Initializes the object detector with the path to a cascade classifier.
//ObjectDetector::ObjectDetector(std::string cascadePath)
//{
//	try
//	{
//		this->model->read("./data/eigenface.yml");
//	}
//	catch (const std::exception&)
//	{
//		if (!database.getIDs().empty()) // shit code.
//		{
//			this->trainModel();
//			this->model->read("./data/eigenface.yml");
//		}
//		else {
//			std::cout << "Error: Couldn't create eigenface.yml. No data exists!" << std::endl;
//		}
//	}
//
//	this->objectCascade.load(cascadePath);
//
//	if (this->objectCascade.empty())
//	{
//		std::cout << "Error: Cascade classifer does not exist." << std::endl;
//		assert(false);
//	}
//}

// Initializes object detector with a path to the cascade classifier and a database.
ObjectDetector::ObjectDetector(std::string cascadePath, Database& database) : database(database)
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

void ObjectDetector::process(cv::Mat& imageMatrix, std::vector<cv::Rect>& foundFaces)
{
    cv::Mat imageGray;
    cv::Mat imageHist;

    // Preprocess original image for easier detection and recognition.
    cvtColor(imageMatrix, imageGray, cv::COLOR_BGR2GRAY);
    equalizeHist(imageGray, imageHist);

    detect(imageGray, foundFaces);
}

void ObjectDetector::process(cv::Mat& imageMatrix, std::vector<std::string>& foundLabels, std::vector<cv::Rect>& foundFaces)
{
    cv::Mat imageGray;
    cv::Mat imageHist;

    // Preprocess original image for easier detection and recognition.
    cvtColor(imageMatrix, imageGray, cv::COLOR_BGR2GRAY);
    equalizeHist(imageGray, imageHist);

    detect(imageGray, foundFaces);

    // Labels vector is used for storing the known and unknown faces.
    // This allows for the display of information by the Settings & Information GUI accurately.
    foundLabels.clear();

    for (int i = 0; i < foundFaces.size(); i++)
    {
        // Preprocess faces for facial recognition.
        cv::Rect ROI(foundFaces[i].tl(), foundFaces[i].br());
        cv::Mat face;

        cv::resize(imageHist(ROI), face, cv::Size(128, 128), 0, 0, cv::INTER_LINEAR);

        std::string label = recognize(face);

        if (label.empty())
        {
            foundLabels.push_back("");

            cv::rectangle(imageMatrix, foundFaces[i].tl(), foundFaces[i].br(), cv::Scalar(0, 0, 255), 2, 8, 0);
            continue;
        } else {
            foundLabels.push_back(label);

            // Calculate the position of the text using the rectangular coordinates of each face.
            int pos_x = std::max(foundFaces[i].tl().x, 0);
            int pos_y = std::max(foundFaces[i].tl().y - 10, 0);

            cv::rectangle(imageMatrix, foundFaces[i].tl(), foundFaces[i].br(), cv::Scalar(0, 255, 0), 2, 8, 0);
            cv::putText(imageMatrix, label, cv::Point(pos_x, pos_y), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 255, 0), 1.0);
        }
    }
}

// Detects an object using the Cascade Classifer.
void ObjectDetector::detect(cv::Mat image, std::vector<cv::Rect> &objects)
{
	this->objectCascade.detectMultiScale(image, objects, 1.1, 10, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(20, 20));
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
std::string ObjectDetector::recognize(cv::Mat image)
{
	if (database.getIDs().empty() || model.empty()) // shit code!
	{
		return "";
	}

	int id = -1;
	double confidence = 0.0;

	model->predict(image, id, confidence);

	if (confidence >= 4000)
	{
		return "";
	}

	return database.getLabels()[id];
}



