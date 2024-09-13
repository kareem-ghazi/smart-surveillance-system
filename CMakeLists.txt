cmake_minimum_required(VERSION 3.28)
project(smart_surveillance_system VERSION 1.0)

find_package(OpenCV REQUIRED)

add_executable(smart_surveillance_system
        lib/tinyfiledialogs/tinyfiledialogs.cpp
        src/Database.cpp
        src/Database.h
        src/main.cpp
        src/ObjectDetector.cpp
        src/ObjectDetector.h
        src/GUI.cpp
        src/GUI.h)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/haarcascade_frontalface_default.xml ${CMAKE_CURRENT_BINARY_DIR}/data/haarcascade_frontalface_default.xml COPYONLY)

target_include_directories(smart_surveillance_system PRIVATE lib/)
target_link_libraries(smart_surveillance_system PRIVATE ${OpenCV_LIBS})
target_compile_features(smart_surveillance_system PRIVATE cxx_std_14)