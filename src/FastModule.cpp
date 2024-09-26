#include "FastModule.h"
#include <iostream>

FastModule::FastModule() : running_(false), frameCount_(0) {
    videoFPS_ = 0;
}

void FastModule::fpsModify(const double videoFPS)   {
    videoFPS_ = videoFPS;
}

FastModule::~FastModule() = default;

void FastModule::start() {
    running_ = true;
    processingThread_ = std::thread(&FastModule::processingLoop, this);
    startTime_ = std::chrono::steady_clock::now();
}

void FastModule::stop() {
    if (running_) {
        running_ = false;
        cv_.notify_all();
        if (processingThread_.joinable()) {
            processingThread_.join();
        }
    }
}

void FastModule::processFrame(const cv::Mat& frame, int frameNumber) {
    std::lock_guard<std::mutex> lock(queueMutex_);
    frameQueue_.emplace(frame, frameNumber);
    cv_.notify_one();
}

void FastModule::processingLoop() {
    while (running_) {
        std::unique_lock<std::mutex> lock(queueMutex_);
        cv_.wait(lock, [this] { return !frameQueue_.empty() || !running_; });

        if (!running_ && frameQueue_.empty()) {
            break;
        }

        auto frameData = frameQueue_.front();
        frameQueue_.pop();
        lock.unlock();

        // Process the frame (here, just count frames)
        frameCount_++;

        // Calculate and print frame rate every 5 seconds
        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedSeconds = currentTime - startTime_;

        if (elapsedSeconds.count() >= 5.0) {
            double fps = frameCount_ / elapsedSeconds.count();
            std::cout << "[FastModule] Average FPS calculated last " << elapsedSeconds.count() << " seconds: " << fps << " Actual video fps: " << videoFPS_ << std::endl;
            // Reset counters
            frameCount_ = 0;
            startTime_ = std::chrono::steady_clock::now();

        }
    }
}

