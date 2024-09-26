cmake_minimum_required(VERSION 3.10)

project(RoboticanTask)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output folder where your program will be created
set(CMAKE_BUILD_TYPE Debug)

# Find OpenCV
find_package(OpenCV REQUIRED)

include_directories(
        ${OpenCV_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/src
)

# Add all source files
set(SOURCES
        src/main.cpp
        src/FrameCapture.cpp
        src/FastModule.cpp
        src/SlowModule.cpp
        src/Journal.cpp
        src/Module.h)

# Add your source files
add_executable(RoboticanTask ${SOURCES})

# Link OpenCV libraries
target_link_libraries(RoboticanTask ${OpenCV_LIBS})