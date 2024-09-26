#include "SlowModule.h"
#include <iostream>
#include <chrono>
#include <filesystem>
namespace fs = std::filesystem;

/*
 * SlowModule saves frames as JPEG files with a limited 5 queue size.
 */
SlowModule::SlowModule(int sleepPeriodMs)
        : running_(false), sleepPeriodMs_(sleepPeriodMs) {
    // Create the directory if it doesn't exist
    fs::path rootDirectory = fs::current_path().parent_path().parent_path();
    std::string root = rootDirectory.string();
    const std::string directoryPath = root + "/Frames";

    if (!fs::exists(directoryPath)) {
        fs::create_directory(directoryPath);
    }

    std::cout << "[SlowModule] Frame directory location: " << directoryPath << std::endl;
    saveDirectory_ = directoryPath;
}

SlowModule::~SlowModule() = default;

void SlowModule::start() {
    running_ = true;
    processingThread_ = std::thread(&SlowModule::processingLoop, this);
}

void SlowModule::stop() {
    if (running_) {
        running_ = false;
        cv_.notify_all();
        if (processingThread_.joinable()) {
            processingThread_.join();
        }
    }
}

void SlowModule::processFrame(const cv::Mat& frame, int frameNumber) {
    std::lock_guard<std::mutex> lock(queueMutex_);

    if (frameQueue_.size() >= maxQueueSize_) {
        // Remove and discard the oldest frame
        int droppedFrameNumber = frameQueue_.front().second;
        frameQueue_.pop_front();

        journal_.logDroppedFrame(droppedFrameNumber);
    }

    frameQueue_.emplace_back(frame, frameNumber);
    cv_.notify_one();
}

void SlowModule::processingLoop() {
    while (running_ || !frameQueue_.empty()) {
        std::unique_lock<std::mutex> lock(queueMutex_);
        cv_.wait(lock, [this] { return !frameQueue_.empty() || !running_; });

        if ((!running_ && frameQueue_.empty()) || ( frameQueueLen_>frameQueue_.size())) {
            break;
        }

        auto frameData = frameQueue_.front();
        frameQueue_.pop_front();
        lock.unlock();

        // Save the frame as a JPEG file
        std::string filename = saveDirectory_ + "/frame_" + std::to_string(frameData.second) + ".jpeg";
        bool success = cv::imwrite(filename, frameData.first);

        if (success) {
            journal_.logSavedFrame(frameData.second);
            std::cout << "[SlowModule] Saved frame number: " << frameData.second << std::endl;
        } else {
            std::cerr << "[SlowModule] Error saving frame number: " << frameData.second << std::endl;
        }

        // Sleep for the defined period
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepPeriodMs_));
        if (frameQueueLen_ < frameQueue_.size()){
            frameQueueLen_++;
        }
    }
}

void SlowModule::printJournalSummary() {
    journal_.printSummary();
}
