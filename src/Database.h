#ifndef DATABASE_H
#define DATABASE_H

#include <direct.h>
#include <iostream>
#include <fstream>
#include <cstring>

#include "Image.h"

class Database
{
private:
	std::string databasePath;
	std::vector<int> ids;
	std::vector<cv::String> labels;
	std::vector<Image> images;
	std::vector<cv::Mat> matrices;

public:
	Database();
	Database(std::string databasePath);

	std::vector<int> getIDs() const;
	std::vector<cv::String> getLabels() const;
	std::vector<Image> getImages() const;
	std::vector<cv::Mat> getMatrices() const;

	void addEntry(std::string name, Image images[10]);
	void loadEntries();
	bool deleteEntry(std::string name);
};

#endif
