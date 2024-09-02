#define CVUI_IMPLEMENTATION
#include "cvui.h"

#include "ObjectDetector.h"
#include "Database.h"
#include "GUI.h"

Database database;
ObjectDetector faceDetector("./data/haarcascade_frontalface_default.xml", database);

void process(cv::Mat& imageMatrix, std::vector<std::string>& labels, std::vector<cv::Rect>& faces);

int main() {
    cv::VideoCapture cap(0, cv::CAP_DSHOW); // CAP_DSHOW api for faster camera opening & closing.
    GUI gui(cap, database, faceDetector);

	while (cv::getWindowProperty(WINDOW2_NAME, cv::WND_PROP_VISIBLE) > 0)
	{
		// In case the webcam is closed, just show an empty window with nothing in it.
		if (!cap.isOpened())
		{
            cv::Mat emptyMatrix(480, 640, CV_8UC3, cv::Scalar(49, 52, 49));
            gui.render(emptyMatrix);
		} else {
            std::vector<cv::Rect> foundFaces;
            std::vector<std::string> foundLabels;

            // Capture the frame from the camera and make a copy from it.
            cv::Mat imageMatrix;
            cap >> imageMatrix;

            // Process for foundFaces (if found) and their foundLabels (if found) in each camera frame.
            process(imageMatrix, foundLabels, foundFaces);

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

void process(cv::Mat& imageMatrix, std::vector<std::string>& foundLabels, std::vector<cv::Rect>& foundFaces)
{
    cv::Mat imageGray;
    cv::Mat imageHist;

    // Preprocess original image for easier detection and recognition.
    cvtColor(imageMatrix, imageGray, cv::COLOR_BGR2GRAY);
    equalizeHist(imageGray, imageHist);

    faceDetector.detect(imageGray, foundFaces);

    // Labels vector is used for storing the known and unknown faces.
    // This allows for the display of information by the Settings & Information GUI accurately.
    foundLabels.clear();

    for (int i = 0; i < foundFaces.size(); i++)
    {
        // Preprocess faces for facial recognition.
        cv::Rect ROI(foundFaces[i].tl(), foundFaces[i].br());
        cv::Mat face;

        cv::resize(imageHist(ROI), face, cv::Size(128, 128), 0, 0, cv::INTER_LINEAR);

        std::string label = faceDetector.recognize(face);

        if (label.empty())
        {
            foundLabels.push_back("");

            cv::rectangle(imageMatrix, foundFaces[i].tl(), foundFaces[i].br(), cv::Scalar(0, 0, 255), 2, 8, 0);
            continue;
        } else {
            foundLabels.push_back(label);

            // Calculate the position of the text using the rectangular coordinates of each face.
            int pos_x = std::max(foundFaces[i].tl().x, 0);
            int pos_y = std::max(foundFaces[i].tl().y - 10, 0);

            cv::rectangle(imageMatrix, foundFaces[i].tl(), foundFaces[i].br(), cv::Scalar(0, 255, 0), 2, 8, 0);
            cv::putText(imageMatrix, label, cv::Point(pos_x, pos_y), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 255, 0), 1.0);
        }
    }
}