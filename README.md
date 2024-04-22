
# Smart Surveillance System

This project is a smart surveillance system that could be used to recognize registered people on the system, detect how many people are there, or to monitor for suspicious or unknown people in a certain area.

The system has been built in C++ using [OpenCV](https://github.com/opencv/opencv) along with a header-only library for the GUI called [cvui](https://github.com/Dovyski/cvui). It utilizes the [Viola–Jones](https://en.wikipedia.org/wiki/Viola%E2%80%93Jones_object_detection_framework) algorithm and runs the image processing on the CPU (a GPU port for this project exists as well using NVIDIA CUDA).
## Features

Some of the features currently implemented are: 

- Detecting faces.
- Recognizing registered faces.
- Information panel for keeping track of currently detected people in the frame.
- Options for enabling/disabling the webcam and the FPS counter.
- Adding people to the system.
- Removing people from the system. 



## Screenshots

![App Screenshot](https://via.placeholder.com/468x300?text=App+Screenshot+Here)


## Demo

Insert gif or link to demo


## Installation

Download the latest version from the [releases](https://github.com/kareem-ghazi/smart-surveillance-system/releases) section.
    
## Run Locally

Clone the project

```bash
  git clone https://link-to-project
```

Go to the project directory

```bash
  cd my-project
```

Install dependencies

```bash
  npm install
```

Start the server

```bash
  npm run start
```


## License

[MIT](https://choosealicense.com/licenses/mit/)

