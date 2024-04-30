#include "Database.h"

Database::Database()
{
	this->databasePath = "./data";
	this->loadEntries();

	/*ofstream dbCount("./count.txt");
	dbCount << 0;*/
}

Database::Database(string databasePath)
{
	this->databasePath = databasePath;
}

vector<int> Database::getIDs() const
{
	return ids;
}

vector<cv::String> Database::getLabels() const
{
	return labels;
}

vector<Image> Database::getImages() const
{
	return images;
}

vector<Mat> Database::getMatrices() const
{
	return matrices;
}

void Database::addEntry(string label, Image images[10])
{
	ifstream iCount("./data/count.txt");
	int dbCount;
	iCount >> dbCount;
	iCount.close();

	for (int i = 0; i < 10; i++)
	{
		string path = this->databasePath + "/" + label + "_" + to_string(dbCount) + "_" + to_string(i) + ".jpg";
		imwrite(path, images[i].getImageMatrix());
	}

	ofstream oCount("./data/count.txt");
	oCount << ++dbCount;
	oCount.close();
}

void Database::loadEntries()
{
	images.clear();
    labels.clear();
	ids.clear();
	matrices.clear();

    std::vector<cv::String> files;
	cv::glob(".\\data", files);

    for (const auto& file : files) {
		if (file == ".\\data\\count.txt" || file == ".\\data\\eigenface.yml")
		{
			continue;
		}

		std::string fileName = file;
		std::string delimiter = "_";

		size_t position = 0;
		std::string token;

		string label;
		int id;
		std::string filepathDelimeter = ".\\data\\";

		position = fileName.find(delimiter);
		label = fileName.substr(filepathDelimeter.length(), position - filepathDelimeter.length());
		fileName.erase(0, position + 1);

		//cout << label << endl;
		
		position = fileName.find(delimiter);
		id = stoi(fileName.substr(0, position));

		Image image(imread(file, 0));

        images.push_back(image);
		matrices.push_back(image.getImageMatrix());
		
		if (labels.capacity() <= id)
		{
			labels.resize(id + 1);
		}
		labels[id] = label;

		ids.push_back(id);
    }
}


