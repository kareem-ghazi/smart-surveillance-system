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
#include <map>

class Database
{
private:
	std::string databasePath;
	std::vector<int> ids;
	std::map<int, cv::String> labels;
	std::vector<cv::Mat> matrices;

public:
	Database();
    Database(std::string databasePath);

	std::vector<int> getIDs() const;
    std::map<int, cv::String> getLabels() const;
	std::vector<cv::Mat> getMatrices() const;

	void addEntry(std::string name, std::vector<cv::Mat> images);
    bool findEntry(std::string name);
    int getEntryID(std::string name);
	void loadEntries();
	void deleteEntry(std::string name);
};

#endif
