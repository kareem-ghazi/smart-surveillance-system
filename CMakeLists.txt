cmake_minimum_required(VERSION 3.28)
project(smart_surveillance_system)

set(CMAKE_CXX_STANDARD 14)
set(ENV{OPENCV_DIR} "D:\\opencv-face")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(smart_surveillance_system
        src/cvui.h
        src/Database.cpp
        src/Database.h
        src/main.cpp
        src/ObjectDetector.cpp
        src/ObjectDetector.h)

set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

target_link_libraries(smart_surveillance_system ${OpenCV_LIBS})
