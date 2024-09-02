#include "Database.h"

// Initializes database and reads entries.
Database::Database()
{
	this->databasePath = "./data";
	this->loadEntries();

	//std::ofstream dbCount("./data/count.txt");

	//if (false) // doesn't exist
	//{
	//	dbCount << 0;
	//}

	//dbCount.close();
}

// Initializes a specific database using a file path.
Database::Database(std::string databasePath)
{
	this->databasePath = databasePath;
}

// Gets the ids from the database.
std::vector<int> Database::getIDs() const
{
	return ids;
}

// Gets the labels from the database.
std::vector<cv::String> Database::getLabels() const
{
	return labels;
}

// Gets the images from the database.
std::vector<cv::Mat> Database::getImages() const
{
	return images;
}

// Gets the image matrices from the database.
std::vector<cv::Mat> Database::getMatrices() const
{
	return matrices;
}

// Adds an entry to the database given a label and 10 images.
// NOTE: Images are preprocessed before given to this function.
void Database::addEntry(std::string label, std::vector<cv::Mat> images)
{
	std::ifstream iCount("./data/count.txt");
	int dbCount;
	iCount >> dbCount;
	iCount.close();

	for (int i = 0; i < 10; i++)
	{
		std::string path = this->databasePath + "/" + label + "_" + std::to_string(dbCount) + "_" + std::to_string(i) + ".jpg";
		imwrite(path, images[i]);
	}

	std::ofstream oCount("./data/count.txt");
	oCount << ++dbCount;
	oCount.close();

	loadEntries();
}

// Deletes an entry from the database (if it exists).
void Database::deleteEntry(std::string name)
{
    int position = getEntryPosition(name);

    for (int j = 0; j < 10; j++)
    {
        std::string image = this->databasePath + "/" + labels[position] + "_" + std::to_string(position) + "_" + std::to_string(j) + ".jpg";

        const char* cImage = image.c_str();
        remove(cImage);
    }
}

bool Database::findEntry(std::string name)
{
    return std::distance(labels.begin(), std::find(labels.begin(), labels.end(), name)) != labels.size();
}

int Database::getEntryPosition(std::string name)
{
    return std::distance(labels.begin(), std::find(labels.begin(), labels.end(), name));
}

// Loads and updates entries from the database.
void Database::loadEntries()
{
	images.clear();
    labels.clear();
	ids.clear();
	matrices.clear();

    std::vector<cv::String> files;
	cv::glob(".\\data", files);

    for (const auto& file : files) {
		if (file == ".\\data\\count.txt" || file == ".\\data\\eigenface.yml" || file == ".\\data\\haarcascade_frontalface_default.xml")
		{
			continue;
		}

		std::string fileName = file;
		std::string delimiter = "_";

		size_t position = 0;
		std::string token;

		std::string label;
		int id;
		std::string filepathDelimeter = ".\\data\\";

		position = fileName.find(delimiter);
		label = fileName.substr(filepathDelimeter.length(), position - filepathDelimeter.length());
		fileName.erase(0, position + 1);

		position = fileName.find(delimiter);
		id = stoi(fileName.substr(0, position));

		cv::Mat matrix(cv::imread(file, 0));

		matrices.push_back(matrix);
		
		if (labels.capacity() <= id)
		{
			labels.resize(id + 1);
		}

		labels[id] = label;

		ids.push_back(id);
    }
}


