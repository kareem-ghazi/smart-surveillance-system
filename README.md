
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

### Building OpenCV
1. Download the source of the latest version of [opencv](https://github.com/opencv/opencv) and [opencv_contrib](https://github.com/opencv/opencv_contrib) (make sure both versions are the same).

2. Download [CMake](https://cmake.org/download/) and [Visual Studio 2022](https://visualstudio.microsoft.com/).

3. Place the opencv and opencv_contrib sources in the same folder along with an empty folder called build.

4. Set the source path to be your opencv folder and the build path to be the build folder. After setting, configure. 

5. Set the extra module parameter to be the opencv_contrib/modules folder. After setting, configure. 

6. Check the opencv_world and opencv_module_face options. After setting, configure and generate files. 

7. Open the project through CMake. Search for the INSTALL module and build it. (two times, release & debug configurations) 

8. You're done! The binaries, lib, and include files are in the install folder. Do not forget to edit your environmental variables to include OpenCV in your users PATH. 

- INCLUDING OPENCV IN PROJECT
1. Open the properties for the project in Visual Studio 

2. Configure the include and library directories for OpenCV in Visual Studio 

3. Add "opencv_world4xx.lib" to your linker input

Clone the project

```bash
  git clone https://link-to-project
```


## License

[MIT](https://choosealicense.com/licenses/mit/)

