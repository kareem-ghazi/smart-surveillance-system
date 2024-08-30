#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include <opencv2/opencv.hpp>
#define CVUI_IMPLEMENTATION
#include "cvui.h"

#include <iostream>
#include <windows.h>

#include "ObjectDetector.h"
#include "Database.h"

#define WINDOW1_NAME "Camera"
#define WINDOW2_NAME "Settings & Information"
#define WINDOW3_NAME "Add a person"
#define WINDOW4_NAME "Remove a person"

void createGUI(cv::Mat& originalFrame, cv::VideoCapture& capture, int faceCount);
void createInformationComponent(cv::Mat& frame, int faceCount);
void createSettingsComponent(cv::Mat& frame, cv::VideoCapture& capture);
bool addEntry();
bool removeEntry();
void openWindow(const cv::String& name);
void closeWindow(const cv::String& name);

cv::Mat imageMatrix;
cv::Mat copyImage;
cv::Mat imageGray;
cv::Mat imageHist;
std::vector<cv::Rect> faces;
std::vector<std::string> labels;
Database database;
ObjectDetector faceDetector("./data/haarcascade_frontalface_default.xml", database);

cv::Mat images[10];
bool webcamOn = true;
bool fpsCounter = false;
int faceCounter = 1;
int fps = 30; // fake fps counter as apparently, CAP_DSHOW + fps detection doesn't work in OpenCV.
std::string name;
bool isAdding = false;
bool isRemoving = false;

int main() {
	cv::VideoCapture cap(0, cv::CAP_DSHOW); // CAP_DSHOW api for faster camera opening & closing.

	// Initialize the two GUIs.
	const cv::String windows[] = { WINDOW1_NAME, WINDOW2_NAME };
	cvui::init(windows, 2);

	while (cv::getWindowProperty(WINDOW1_NAME, cv::WND_PROP_VISIBLE) > 0 && cv::getWindowProperty(WINDOW2_NAME, cv::WND_PROP_VISIBLE) > 0)
	{
		cv::Mat GUIFrame(330, 450, CV_8UC3, cv::Scalar(49, 52, 49));

		// In case the webcam is closed, just show an empty window with nothing in it.
		if (!cap.isOpened())
		{
			cvui::context(WINDOW2_NAME);
			createGUI(GUIFrame, cap, 0);

			cvui::imshow(WINDOW2_NAME, GUIFrame);

			cv::Mat imageMatrix(480, 640, CV_8UC3, cv::Scalar(49, 52, 49));
			cvui::imshow(WINDOW1_NAME, imageMatrix);

			if (cv::waitKey(1) == 27) {
				break;
			}

			continue;
		}

		// Capture the frame from the camera and make a copy from it.
		cap >> imageMatrix;

		// Preprocess original image for easier detection and recognition.
        cvtColor(imageMatrix, imageGray, cv::COLOR_BGR2GRAY);
        equalizeHist(imageGray, imageHist);

		faceDetector.detect(imageGray, faces); //!!!!

		// Labels vector is used for storing the known and unknown faces.
		// This allows for the display of information by the Settings & Information GUI accurately.
		labels.clear();
		labels.resize(faces.size());

		for (int i = 0; i < faces.size(); i++)
		{
			// Preprocess faces for facial recognition.
            cv::Rect ROI(faces[i].tl(), faces[i].br());
            cv::Mat face;

            cv::resize(imageHist(ROI), face, cv::Size(128, 128), 0, 0, cv::INTER_LINEAR);

			std::string label = faceDetector.recognize(face); //!!!!

			// If a label exists for the face, put it inside the vector.
			if (!label.empty())
			{
				labels.at(i) = label;
			}

            // Calculate the position of the text using the rectangular coordinates of each face.
            int pos_x = max(faces[i].tl().x, 0);
            int pos_y = max(faces[i].tl().y - 10, 0);

            if (label.empty())
            {
                cv::rectangle(imageMatrix, faces[i].tl(), faces[i].br(), cv::Scalar(0, 0, 255), 2, 8, 0);
                continue;
            } else {
                cv::rectangle(imageMatrix, faces[i].tl(), faces[i].br(), cv::Scalar(0, 255, 0), 2, 8, 0);
                cv::putText(imageMatrix, label, cv::Point(pos_x, pos_y), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 255, 0), 1.0);
            }
		}

		// Creation of the Settings & Information GUI.
		cvui::context(WINDOW2_NAME);
		createGUI(GUIFrame, cap, faces.size());

		// Updates the Camera and the Settings & Information GUIs.
		cvui::imshow(WINDOW1_NAME, imageMatrix);
		cvui::imshow(WINDOW2_NAME, GUIFrame);

		// Check whether ESC has been pressed or whether the program has been closed.
		if (cv::waitKey(1) == 27) {
			break;
		}
	}

	// Release resources
	cap.release();
	cv::destroyAllWindows();
}

// Creates a graphical user interface for interacting with the system. Includes system information & settings.
void createGUI(cv::Mat& frame, cv::VideoCapture& capture, int faceCount)
{
//	// Double buffer because apparently, this is known issue in dealing with graphics.
//	// However, I have absolutely no clue or understand the concept behind double buffers.
//	cv::Mat copyFrame = frame.clone();
//	cv::Mat doubleBuffer = copyFrame.clone();
//
//	doubleBuffer.copyTo(frame);

	createInformationComponent(frame, faceCount);
	createSettingsComponent(frame, capture);

	// I mean... gotta do it, right?
	cvui::printf(frame, 30, frame.rows - 31, 0.4, 0xCECECE, "Copyright 2024 by Kareem Ghazi");
}

// Creates information 'window' component inside of the specified window.
void createInformationComponent(cv::Mat& frame, int faceCount)
{
	cvui::window(frame, 20, 20, 210, 240, "Information");

	cvui::beginColumn(frame, 34, 55, -1, -1, 10);
	// Depending on the number of faces, show different messages.
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

	int unknownFaces = 0;
	int knownFaces = 0;

	// For each face label,
	// if the label is empty, add to unknownFaces.
	// otherwise, add to knownFaces and update the currently detected with that face label.
	for (std::string label : labels)
	{
		if (label.empty())
		{
			unknownFaces++;
		}
		else {
			cvui::text(label, 0.4, 0x00ff00);
			knownFaces++;
		}
	}

	// Different messages depending on the amount of unknown faces found.
	if (unknownFaces > 0 && knownFaces > 0)
	{
		cvui::printf(0.4, 0xff474c, "...and %d other people.", unknownFaces);
	}
	else if (unknownFaces == 1) {
		cvui::printf(0.4, 0xff474c, "%d unknown person.", unknownFaces);
	}
	else if (unknownFaces > 1) {
		cvui::printf(0.4, 0xff474c, "%d unknown people.", unknownFaces);
	}
	
	cvui::endColumn();
}

// Creates settings 'window' component inside of the specified window.
void createSettingsComponent(cv::Mat& frame, cv::VideoCapture& capture)
{
	cvui::window(frame, frame.cols - 200, 20, 180, 240, "Settings");
	cvui::checkbox(frame, frame.cols - 185, 55, "Webcam", &webcamOn);

	// Whether the webcam is actually on and the checkbox for the Webcam is checked.
	if (webcamOn && !capture.isOpened())
	{
		capture.open(0, cv::CAP_DSHOW);
	}
	else if (!webcamOn && capture.isOpened())
	{
		capture.release();
	}

	cvui::checkbox(frame, frame.cols - 185, 80, "FPS Counter", &fpsCounter);

	// Fps counter only under the condition that the camera is open.
	// NOTE: Doesn't work due to CAP_DSHOW api doesn't allow fps detection.
	if (fpsCounter && webcamOn)
	{
        cv::putText(imageMatrix, std::to_string(fps), cv::Point(10, 20), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(0, 255, 0), 1.0);
    }
	else {
		fpsCounter = false;
	}

	cvui::printf(frame, frame.cols - 185, 125, 0.4, 0xcccccc, "Adds a new person.");
	// When the add button is pressed.
	if (cvui::button(frame, frame.cols - 185, 145, "Add") && webcamOn) {
		system("cls");
		std::cout << "Enter the person's name: ";

		std::cin >> name;

		openWindow(WINDOW3_NAME);
		isAdding = addEntry();
		system("cls");
	}

	if (isAdding)
	{
		isAdding = addEntry();
	}

	// When the remove button is pressed.
	cvui::printf(frame, frame.cols - 185, 195, 0.4, 0xcccccc, "Removes a person.");
	if (cvui::button(frame, frame.cols - 185, 215, "Remove") && webcamOn) {
		system("cls");
		std::cout << "Enter the person's name: ";

		std::cin >> name;

		openWindow(WINDOW4_NAME);
		isRemoving = removeEntry();
	}

	if (isRemoving)
	{
		isRemoving = removeEntry();
	}
}

// Adds an entry to the database. Returns whether the sucess window has been closed.
// This is because this function updates the frames for the window as well.
bool addEntry()
{
	cvui::context(WINDOW3_NAME);

	cv::Mat addEntryGUI = cv::Mat(90, 385, CV_8UC3);
	addEntryGUI = cv::Scalar(1, 31, 20);

	cvui::printf(addEntryGUI, 25, 20, 0.4, 0x00ff00, "Press the record button to record your face. (%d/10)", faceCounter);

	// When the record button is pressed and there are faces,
	// the image is processed and added to the array of faces.
	if (cvui::button(addEntryGUI, 145, 45, "Record") && faces.size() == 1) {
        cv::Rect ROI(faces[0].tl(), faces[0].br());
        cv::Mat face;

        cv::resize(imageHist(ROI), face, cv::Size(128, 128), 0, 0, cv::INTER_LINEAR);

		images[faceCounter - 1] = face;
		faceCounter++;
	}

	// If we got 10 faces, close the window, add the entry, and train a new model.
	if (faceCounter == 11)
	{
		closeWindow(WINDOW3_NAME);
		database.addEntry(name, images);
		faceDetector.trainModel();

		faceCounter = 1;
		return false;
	}

	cvui::imshow(WINDOW3_NAME, addEntryGUI);

	return true;
}

// Removes entry from the database. Return whether the success or failure window has closed.
// This is because this function updates the frames for the window as well.
bool removeEntry()
{
	cvui::context(WINDOW4_NAME);

	cv::Mat removeSuccess = cv::Mat(90, 350, CV_8UC3);

	// shit code. 
	// why delete every entry at the loading of each frame again if you did it once? 
	// guess i'll never know .-.
	bool status = database.deleteEntry(name);

	// Shows the status window depending on the what occured from the attempt to delete an entry.
	if (status)
	{
		removeSuccess = cv::Scalar(1, 31, 20);
		cvui::printf(removeSuccess, 25, 20, 0.4, 0x00ff00, "Successfully removed person from the system!");
		faceDetector.trainModel();
	}
	else {
		removeSuccess = cv::Scalar(0, 0, 60);
		cvui::printf(removeSuccess, 90, 20, 0.4, 0xff0000, "Error! Person doesn't exist.");
	}

	// Close button for the status window.
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