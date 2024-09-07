#ifndef GUI_H
#define GUI_H

#include <opencv2/opencv.hpp>

#include <string>
#include <vector>
#include "Database.h"
#include "ObjectDetector.h"

#define CAMERA "Camera"
#define SETTINGS_INFORMATION "Settings & Information"
#define ADD_WINDOW "Add a person"
#define REMOVE_WINDOW "Remove a person"

class GUI {
private:
    static const unsigned int GRAY = 0xcccccc;
    static const unsigned int GREEN = 0x00ff00;
    static const unsigned int RED = 0xff474c;
    static const unsigned int DARK_RED = 0xff0000;

    cv::VideoCapture& capture;
    Database database;
    ObjectDetector faceDetector;
    cv::Mat imageMatrix;
    cv::Mat matrix;
    std::vector<std::string> windows;
    bool isAdding = false;
    bool isRemoving = false;
    std::string name;

    void renderInformationComponent(std::vector<std::string> labels, int faces);
    void renderSettingsComponent();
    bool addEntry();
    bool removeEntry();
    void openWindow(const cv::String& name);
    void closeWindow(const cv::String& name);
public:
    GUI(cv::VideoCapture& capture, Database& database, ObjectDetector& faceDetector);

    void render(cv::Mat& imageMatrix);
    void render(cv::Mat& imageMatrix, std::vector<std::string> labels, int faces);
};

#endif GUI_H
