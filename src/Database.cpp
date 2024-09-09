#include "Database.h"

// Initializes database and reads entries.
Database::Database()
{
	this->databasePath = "./data";
	this->loadEntries();

	std::ifstream dbCount("./data/count.txt");

    // If the file doesn't exist, create a new one.
	if (!dbCount.good())
	{
        std::ofstream newDBCount("./data/count.txt");
        newDBCount << 0;
        newDBCount.close();
	}

	dbCount.close();
}

// Gets the ids from the database.
std::vector<int> Database::getIDs() const
{
	return ids;
}

// Gets the labels from the database.
std::map<int, cv::String> Database::getLabels() const
{
	return labels;
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

	for (int i = 0; i < IMAGE_COUNT; i++)
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
    int id = getEntryID(name);

    for (int j = 0; j < IMAGE_COUNT; j++)
    {
        std::string image = this->databasePath + "/" + labels[id] + "_" + std::to_string(id) + "_" + std::to_string(j) + ".jpg";

        const char* cImage = image.c_str();
        remove(cImage);
    }
}

bool Database::findEntry(std::string name)
{
    for (auto it = labels.begin(); it != labels.end(); ++it)
    {
        if (it->second == name)
            return true;
    }

    return false;
}

int Database::getEntryID(std::string name)
{
    for (auto it = labels.begin(); it != labels.end(); ++it)
    {
        if (it->second == name)
            return it->first;
    }

    return -1;
}

// Loads and updates entries from the database.
void Database::loadEntries()
{
    labels.clear();
	ids.clear();
	matrices.clear();

    std::vector<std::string> files;
	cv::glob(".\\data", files);

    for (std::string file : files) {
		if (file == ".\\data\\count.txt" || file == ".\\data\\eigenface.yml" || file == ".\\data\\haarcascade_frontalface_default.xml")
		{
			continue;
		}

        std::string fileName = file;
        std::stringstream stringstr(fileName.erase(0, 7));

		std::string label;
        getline(stringstr, label, '_');

		std::string id;
        getline(stringstr, id, '_');

        cv::Mat matrix(cv::imread(file, 0));

        matrices.push_back(matrix);
        labels[stoi(id)] = label;
        ids.push_back(stoi(id));
    }
}


