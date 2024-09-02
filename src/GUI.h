#ifndef GUI_H
#define GUI_H

#include <opencv2/opencv.hpp>

#include "Database.h"
#include "ObjectDetector.h"
#include <string>
#include <vector>

#define WINDOW1_NAME "Camera"
#define WINDOW2_NAME "Settings & Information"
#define WINDOW3_NAME "Add a person"
#define WINDOW4_NAME "Remove a person"

class GUI {
private:
    cv::VideoCapture& capture;
    Database database;
    ObjectDetector faceDetector;
    cv::Mat imageMatrix;
    cv::Mat matrix;
    std::vector<std::string> windows;
    bool isAdding = false;
    bool isRemoving = false;
    std::string name;

public:
    GUI(cv::VideoCapture& capture, Database& database, ObjectDetector& faceDetector);

    void render(cv::Mat& imageMatrix);
    void render(cv::Mat& imageMatrix, std::vector<std::string> labels, int faces);
    void renderInformationComponent(std::vector<std::string> labels, int faces);
    void renderSettingsComponent();
    bool addEntry();
    bool removeEntry();
    void setFaceCount(int faceCount);
    void setWebcam(bool value);
    void addWindow(std::string windowName);
    void openWindow(const cv::String& name);
    void closeWindow(const cv::String& name);
};

#endif GUI_H
