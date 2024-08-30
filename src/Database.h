#ifndef DATABASE_H
#define DATABASE_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include <direct.h>
#include <iostream>
#include <fstream>
#include <cstring>

class Database
{
private:
	std::string databasePath;
	std::vector<int> ids;
	std::vector<cv::String> labels;
	std::vector<cv::Mat> images;
	std::vector<cv::Mat> matrices;

public:
	Database();
	Database(std::string databasePath);

	std::vector<int> getIDs() const;
	std::vector<cv::String> getLabels() const;
	std::vector<cv::Mat> getImages() const;
	std::vector<cv::Mat> getMatrices() const;

	void addEntry(std::string name, cv::Mat images[10]);
	void loadEntries();
	bool deleteEntry(std::string name);
};

#endif
