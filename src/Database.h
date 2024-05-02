#pragma once
#include <direct.h>
#include <iostream>
#include <fstream>
#include <cstring>

#include "Image.h"

using namespace std;
using namespace cv;

class Database
{
private:
	string databasePath;
	vector<int> ids;
	vector<cv::String> labels;
	vector<Image> images;
	vector<Mat> matrices;

public:
	Database();
	Database(string databasePath);

	vector<int> getIDs() const;
	vector<cv::String> getLabels() const;
	vector<Image> getImages() const;
	vector<Mat> getMatrices() const;

	void addEntry(string name, Image images[10]);
	void loadEntries();
	void deleteEntry(string name);
};

