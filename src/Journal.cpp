#include "Journal.h"
#include <iostream>

void Journal::logSavedFrame(int frameNumber) {
    std::lock_guard<std::mutex> lock(mutex_);
    savedFrames_.push_back(frameNumber);
}

void Journal::logDroppedFrame(int frameNumber) {
    std::lock_guard<std::mutex> lock(mutex_);
    droppedFrames_.push_back(frameNumber);
}

void Journal::printSummary() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << "----- Journal Summary -----" << std::endl;
    std::cout << "Saved Frames: ";
    for (int num : savedFrames_) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    std::cout << "Dropped Frames: ";
    for (int num : droppedFrames_) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    std::cout << "---------------------------" << std::endl;
}
