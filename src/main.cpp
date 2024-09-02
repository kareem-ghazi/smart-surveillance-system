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
            std::vector<cv::Rect> faces;
            std::vector<std::string> labels;

            // Capture the frame from the camera and make a copy from it.
            cv::Mat imageMatrix;
            cap >> imageMatrix;

            // Process for faces (if found) and their labels (if found) in each camera frame.
            process(imageMatrix, labels, faces);

            // Render GUI components and the frames from the camera.
            gui.render(imageMatrix, labels, faces.size());
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

void process(cv::Mat& imageMatrix, std::vector<std::string>& labels, std::vector<cv::Rect>& faces)
{
    cv::Mat imageGray;
    cv::Mat imageHist;

    // Preprocess original image for easier detection and recognition.
    cvtColor(imageMatrix, imageGray, cv::COLOR_BGR2GRAY);
    equalizeHist(imageGray, imageHist);

    faceDetector.detect(imageGray, faces);

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

        std::string label = faceDetector.recognize(face);

        // If a label exists for the face, put it inside the vector.
        if (!label.empty())
        {
            labels.at(i) = label;
        }

        // Calculate the position of the text using the rectangular coordinates of each face.
        int pos_x = std::max(faces[i].tl().x, 0);
        int pos_y = std::max(faces[i].tl().y - 10, 0);

        if (label.empty())
        {
            cv::rectangle(imageMatrix, faces[i].tl(), faces[i].br(), cv::Scalar(0, 0, 255), 2, 8, 0);
            continue;
        } else {
            cv::rectangle(imageMatrix, faces[i].tl(), faces[i].br(), cv::Scalar(0, 255, 0), 2, 8, 0);
            cv::putText(imageMatrix, label, cv::Point(pos_x, pos_y), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 255, 0), 1.0);
        }
    }
}