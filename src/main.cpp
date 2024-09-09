#define CVUI_IMPLEMENTATION
#include "cvui/cvui.h"

#include "ObjectDetector.h"
#include "Database.h"
#include "GUI.h"

int main() {
    cv::VideoCapture cap(0, cv::CAP_DSHOW); // CAP_DSHOW api for faster camera opening & closing.

    Database database;
    ObjectDetector faceDetector("./data/haarcascade_frontalface_default.xml", database);
    GUI gui(cap, database, faceDetector);

	while (cv::getWindowProperty(SETTINGS_INFORMATION, cv::WND_PROP_VISIBLE) > 0)
	{
		// In case the webcam is closed, just show an empty window with nothing in it.
		if (!cap.isOpened())
		{
            cv::Mat emptyMatrix(480, 640, CV_8UC3, cv::Scalar(49, 52, 49));
            gui.render(emptyMatrix);
		} else {
            std::vector<cv::Rect> foundFaces;
            std::vector<std::string> foundLabels;

            // Capture the frame from the camera.
            cv::Mat imageMatrix;
            cap >> imageMatrix;

            // Process for foundFaces (if found) and their foundLabels (if found) in each camera frame.
            faceDetector.process(imageMatrix, foundLabels, foundFaces);

            // Render GUI components and the frames from the camera.
            gui.render(imageMatrix, foundLabels, foundFaces.size());
        }

		// Check whether ESC has been pressed or whether the program has been closed.
		if (cv::waitKey(1) == 27) {
			break;
		}
	}

	// Release resources
	cap.release();
	cv::destroyAllWindows();
}