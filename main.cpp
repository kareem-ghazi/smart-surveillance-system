#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include <opencv2/opencv.hpp>
#define CVUI_IMPLEMENTATION
#include "cvui.h"

#include <iostream>
#include <windows.h>

#include "ImageProcessor.h"
#include "ObjectDetector.h"

#define WINDOW1_NAME "Camera"
#define WINDOW2_NAME "Settings & Information"
#define WINDOW3_NAME "Add a person"
#define WINDOW4_NAME "Remove a person"

using namespace cv;
using namespace std;

bool createGUI(Mat& originalFrame, VideoCapture& capture, int faceCount);
void createInformationComponent(Mat& frame, int faceCount);
void createSettingsComponent(Mat& frame, VideoCapture& capture);
bool addEntry();
bool removeEntry();
void openWindow(const cv::String& name);
void closeWindow(const cv::String& name);

Mat imageMatrix;
Image image;
Image imageGray;
Image imageHist;
vector<Rect> faces;
ObjectDetector faceDetector("Resources/haarcascade_frontalface_default.xml");

bool webcamOn = true;
bool fpsCounter = false;
int faceCounter = 1;
double fps = 0;
bool isAdding = false;
bool isRemoving = false;

int main() {
	VideoCapture cap(0, CAP_DSHOW);

	const cv::String windows[] = { WINDOW1_NAME, WINDOW2_NAME };
	cvui::init(windows, 2);
	bool isRunning = true;

	while (cv::getWindowProperty(WINDOW2_NAME, 0) >= 0 && cv::getWindowProperty(WINDOW1_NAME, 0) >= 0)
	{
		Mat GUIFrame(330, 450, CV_8UC3, cv::Scalar(49, 52, 49));

		if (!cap.isOpened())
		{
			cvui::context(WINDOW2_NAME);
			isRunning = createGUI(GUIFrame, cap, 0);

			cvui::imshow(WINDOW2_NAME, GUIFrame);

			Mat imageMatrix(480, 640, CV_8UC3, Scalar(49, 52, 49));
			cvui::imshow(WINDOW1_NAME, imageMatrix);

			if (cv::waitKey(1) == 27 || !isRunning) {
				break;
			}

			continue;
		}

		cap >> imageMatrix;
		image.setImageMatrix(imageMatrix);

		imageGray = ImageProcessor::grayscale(image);
		imageHist = ImageProcessor::equalizeHistogram(imageGray);

		faceDetector.detect(imageHist, faces);

		for (int i = 0; i < faces.size(); i++)
		{
			//Image croppedFace = ImageProcessor::crop(image, faces[i].tl(), faces[i].br());
			//Image face = ImageProcessor::resize(image, 128, 128);*/
			//Database::saveImage(filepath, image);

			ImageProcessor::drawRectangle(image, faces[i].tl(), faces[i].br(), 'G');

			int pos_x = max(faces[i].tl().x, 0);
			int pos_y = max(faces[i].tl().y - 10, 0);

			ImageProcessor::putText(image, pos_x, pos_y, 'G', 0.75, "Kareem Ghazi");
		}

		cvui::context(WINDOW2_NAME);
		createGUI(GUIFrame, cap, faces.size());

		cvui::imshow(WINDOW1_NAME, image.getImageMatrix());
		cvui::imshow(WINDOW2_NAME, GUIFrame);

		if (cv::waitKey(1) == 27 || !isRunning) {
			break;
		}
	}

	// Release resources
	cap.release();
	cv::destroyAllWindows();
}

// Creates a graphical user interface for interacting with the system. Includes system information & settings.
bool createGUI(Mat& frame, VideoCapture& capture, int faceCount)
{
	Mat copyFrame = frame.clone();
	Mat doubleBuffer = copyFrame.clone();

	doubleBuffer.copyTo(frame);

	if (cvui::button(frame, frame.cols - 90, frame.rows - 40, "Quit")) {
		return false;
	}

	createInformationComponent(frame, faceCount);
	createSettingsComponent(frame, capture);

	cvui::printf(frame, 30, frame.rows - 31, 0.4, 0xCECECE, "Copyright 2024 by Kareem Ghazi");

	return true;
}

// Creates information 'window' component inside of the specified window.
void createInformationComponent(Mat& frame, int faceCount)
{
	cvui::window(frame, 20, 20, 210, 240, "Information");

	cvui::beginColumn(frame, 34, 55, -1, -1, 10);
	if (faceCount >= 1) {
		cvui::printf(0.4, 0xcccccc, "Number of People Detected: ");
		cvui::printf(0.4, 0x00ff00, "%d", faceCount);
	}
	else {
		cvui::printf(0.4, 0xcccccc, "Number of People Detected: ");
		cvui::printf(0.4, 0xcccccc, "%d", faceCount);
	}

	cvui::space(5);
	cvui::printf(0.4, 0xcccccc, "Currently Detecting: ");

	if (faceCount == 1)
	{
		cvui::printf(0.4, 0x00ff00, "Kareem Ghazi");
	}
	else if (faceCount >= 2) {
		cvui::printf(0.4, 0x00ff00, "Kareem Ghazi");
		cvui::printf(0.4, 0xff474c, "...and %d other people.", faceCount - 1);
	}
	cvui::endColumn();
}

// Creates settings 'window' component inside of the specified window.
void createSettingsComponent(Mat& frame, VideoCapture& capture)
{
	cvui::window(frame, frame.cols - 200, 20, 180, 240, "Settings");
	cvui::checkbox(frame, frame.cols - 185, 55, "Webcam", &webcamOn);

	if (webcamOn && !capture.isOpened())
	{
		capture.open(0, CAP_DSHOW);
	}
	else if (!webcamOn && capture.isOpened())
	{
		capture.release();
	}

	cvui::checkbox(frame, frame.cols - 185, 80, "FPS Counter", &fpsCounter);

	if (fpsCounter && webcamOn)
	{
		ImageProcessor::putText(image, 10, 20, 'B', 0.6, to_string(fps));
	}
	else {
		fpsCounter = false;
	}

	cvui::printf(frame, frame.cols - 185, 125, 0.4, 0xcccccc, "Adds a new person.");
	if (cvui::button(frame, frame.cols - 185, 145, "Add")) {
		/*system("cls");
		cout << "Enter the person's name: " << endl;

		string name;
		cin >> name;*/

		openWindow(WINDOW3_NAME);
		isAdding = addEntry();
	}

	if (isAdding)
	{
		isAdding = addEntry();
	}

	cvui::printf(frame, frame.cols - 185, 195, 0.4, 0xcccccc, "Removes a person.");
	if (cvui::button(frame, frame.cols - 185, 215, "Remove")) {
		openWindow(WINDOW4_NAME);
		isRemoving = removeEntry();
	}

	if (isRemoving)
	{
		isRemoving = removeEntry();
	}
}

bool addEntry()
{
	cvui::context(WINDOW3_NAME);

	Mat addEntryGUI = Mat(90, 385, CV_8UC3);
	addEntryGUI = cv::Scalar(1, 31, 20);

	cvui::printf(addEntryGUI, 25, 20, 0.4, 0x00ff00, "Press the record button to record your face. (%d/10)", faceCounter);

	if (cvui::button(addEntryGUI, 145, 45, "Record")) {
		faceCounter++;
	}

	if (faceCounter == 11)
	{
		closeWindow(WINDOW3_NAME);
		faceCounter = 1;
		return false;
	}

	cvui::imshow(WINDOW3_NAME, addEntryGUI);

	return true;
}

bool removeEntry()
{
	cvui::context(WINDOW4_NAME);

	Mat removeSuccess = Mat(90, 350, CV_8UC3);
	removeSuccess = cv::Scalar(1, 31, 20);

	cvui::printf(removeSuccess, 25, 20, 0.4, 0x00ff00, "Successfully removed person from the system!");

	if (cvui::button(removeSuccess, 135, 45, "Close")) {
		closeWindow(WINDOW4_NAME);
		return false;
	}

	cvui::imshow(WINDOW4_NAME, removeSuccess);

	return true;
}

// Open a new OpenCV window and watch it using cvui
void openWindow(const cv::String& name) {
	cv::namedWindow(name);
	cvui::watch(name);
}

// Open an OpenCV window
void closeWindow(const cv::String& name) {
	cv::destroyWindow(name);
	cv::waitKey(1);
}