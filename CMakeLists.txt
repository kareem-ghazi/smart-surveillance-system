cmake_minimum_required(VERSION 3.28)
project(smart_surveillance_system)

find_package(OpenCV REQUIRED)

add_executable(smart_surveillance_system
        libs/cvui/cvui.h
        src/Database.cpp
        src/Database.h
        src/main.cpp
        src/ObjectDetector.cpp
        src/ObjectDetector.h
        src/GUI.cpp
        src/GUI.h)

target_link_libraries(smart_surveillance_system PRIVATE ${OpenCV_LIBS})
target_compile_features(smart_surveillance_system PRIVATE cxx_std_14)