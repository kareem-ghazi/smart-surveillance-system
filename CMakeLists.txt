cmake_minimum_required(VERSION 3.28)
project(smart_surveillance_system)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(smart_surveillance_system
        src/cvui.h
        src/Database.cpp
        src/Database.h
        src/main.cpp
        src/ObjectDetector.cpp
        src/ObjectDetector.h
        src/GUI.cpp
        src/GUI.h)

target_link_libraries(smart_surveillance_system ${OpenCV_LIBS})
