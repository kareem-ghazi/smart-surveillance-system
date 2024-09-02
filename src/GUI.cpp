#include "GUI.h"
#include "cvui.h"

GUI::GUI(cv::VideoCapture& capture, Database& database, ObjectDetector& faceDetector)
: capture(capture), database(database), faceDetector(faceDetector) {
    // Initialize the two GUIs.
    const cv::String windows[] = { WINDOW1_NAME, WINDOW2_NAME };
    cvui::init(windows, 2);
}

void GUI::render(cv::Mat& imageMatrix) {
    render(imageMatrix, std::vector<std::string>(), 0);
}

void GUI::render(cv::Mat& imageMatrix, std::vector<std::string> labels, int faces) {
    this->matrix = cv::Mat(330, 450, CV_8UC3, cv::Scalar(49, 52, 49));
    this->imageMatrix = imageMatrix;

    renderInformationComponent(labels, faces);
    renderSettingsComponent();

    cvui::printf(matrix, 30, matrix.rows - 31, 0.4, 0xCECECE, "Copyright 2024 by Kareem Ghazi");

    cvui::context(WINDOW2_NAME);
    cvui::imshow(WINDOW1_NAME, imageMatrix);
    cvui::imshow(WINDOW2_NAME, matrix);
}

void GUI::renderInformationComponent(std::vector<std::string> labels, int faces) {
    cvui::window(matrix, 20, 20, 210, 240, "Information");

    cvui::beginColumn(matrix, 34, 55, -1, -1, 10);
    // Depending on the number of faces, show different messages.
    if (faces >= 1) {
        cvui::printf(0.4, 0xcccccc, "Number of People Detected: ");
        cvui::printf(0.4, 0x00ff00, "%d", faces);
    }
    else {
        cvui::printf(0.4, 0xcccccc, "Number of People Detected: ");
        cvui::printf(0.4, 0xcccccc, "%d", faces);
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

void GUI::renderSettingsComponent() {
    cvui::window(matrix, matrix.cols - 200, 20, 180, 240, "Settings");

    static bool webcamOn = true;
    cvui::checkbox(matrix, matrix.cols - 185, 55, "Webcam", &webcamOn);

    // Whether the webcam is actually on and the checkbox for the Webcam is checked.
    if (webcamOn && !capture.isOpened())
    {
        capture.open(0, cv::CAP_DSHOW);
    }
    else if (!webcamOn && capture.isOpened())
    {
        capture.release();
    }

    static bool fpsCounter = false;
    cvui::checkbox(matrix, matrix.cols - 185, 80, "FPS Counter", &fpsCounter);

    // Fps counter only under the condition that the camera is open.
    // NOTE: Doesn't work due to CAP_DSHOW api doesn't allow fps detection.
    if (fpsCounter && webcamOn)
    {
        cv::putText(imageMatrix, std::to_string(30), cv::Point(10, 20), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(0, 255, 0), 1.0);
    }
    else {
        fpsCounter = false;
    }

    cvui::printf(matrix, matrix.cols - 185, 125, 0.4, 0xcccccc, "Adds a new person.");
    // When the add button is pressed.
    if (cvui::button(matrix, matrix.cols - 185, 145, "Add") && webcamOn) {
        std::cout << "Enter the person's name: ";
        std::cin >> name;
        std::cout << std::endl;

        openWindow(WINDOW3_NAME);
        isAdding = addEntry();
    }

    if (isAdding)
    {
        isAdding = addEntry();
    }

    // When the remove button is pressed.
    cvui::printf(matrix, matrix.cols - 185, 195, 0.4, 0xcccccc, "Removes a person.");
    if (cvui::button(matrix, matrix.cols - 185, 215, "Remove") && webcamOn) {
        std::cout << "Enter the person's name: ";
        std::cin >> name;
        std::cout << std::endl;

        openWindow(WINDOW4_NAME);
        isRemoving = removeEntry();
    }

    if (isRemoving)
    {
        isRemoving = removeEntry();
    }
}

bool GUI::addEntry()
{
    cvui::context(WINDOW3_NAME);
    cv::Mat addEntryGUI = cv::Mat(90, 385, CV_8UC3, cv::Scalar(1, 31, 20));

    static std::vector<cv::Mat> images;

    cvui::printf(addEntryGUI, 25, 20, 0.4, 0x00ff00, "Press the record button to record your face. (%d/10)", images.size() + 1);
    bool pressedRecord = cvui::button(addEntryGUI, 145, 45, "Record");

    cvui::imshow(WINDOW3_NAME, addEntryGUI);

    // When the record button is pressed and there are faces,
    // the image is processed and added to the array of faces.
    if (!pressedRecord) {
        return true;
    }

    cvui::imshow(WINDOW3_NAME, addEntryGUI);

    capture >> imageMatrix;
    std::vector<cv::Rect> faces;

    faceDetector.detect(imageMatrix, faces);

    if (faces.size() != 1)
    {
        return true;
    }

    cv::Rect ROI(faces[0].tl(), faces[0].br());
    cv::Mat face;

    cv::resize(imageMatrix(ROI), face, cv::Size(128, 128), 0, 0, cv::INTER_LINEAR);

    images.push_back(face);

    // If we got 10 faces, close the window, add the entry, and train a new model.
    if (images.size() == 10)
    {
        closeWindow(WINDOW3_NAME);
        database.addEntry(name, images);
        faceDetector.trainModel();

        images.resize(0);
        return false;
    }

    return true;
}

// Removes entry from the database. Return whether the success or failure window has closed.
// This is because this function updates the frames for the window as well.
bool GUI::removeEntry()
{
    cvui::context(WINDOW4_NAME);
    cv::Mat removeSuccess(90, 350, CV_8UC3);

    bool found = database.findEntry(name);

    // Shows the status window depending on the what occured from the attempt to delete an entry.
    if (found)
    {
        removeSuccess = cv::Scalar(1, 31, 20);
        cvui::printf(removeSuccess, 25, 20, 0.4, 0x00ff00, "Successfully removed person from the system!");
        database.deleteEntry(name);
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
void GUI::openWindow(const cv::String& name) {
    cv::namedWindow(name);
    cvui::watch(name);
}

// Open an OpenCV window
void GUI::closeWindow(const cv::String& name) {
    cv::destroyWindow(name);
    cv::waitKey(1);
}
