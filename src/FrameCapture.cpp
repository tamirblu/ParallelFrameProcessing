#include "FrameCapture.h"
#include <iostream>
#include "slowModule.h"

// constructor
//Initializes member variables efficiently.
//videoFilePath_(videoFilePath): Directly initializes videoFilePath_ with the passed parameter.
FrameCapture::FrameCapture(const std::string& videoFilePath)
        : videoFilePath_(videoFilePath), running_(false) {}

// destructor
FrameCapture::~FrameCapture() {
    stop();
}

void FrameCapture::start() {
    running_ = true;
    captureThread_ = std::thread(&FrameCapture::captureLoop, this);
}
void FrameCapture::join() {
    if (captureThread_.joinable()) {
        captureThread_.join();
    }
}

void FrameCapture::stop() {
    if (running_) {
        running_ = false;
        if (captureThread_.joinable()) {
            captureThread_.join();
        }
    }
}

void FrameCapture::addModule(Module* module) {
    modules_.push_back(module);
}

void FrameCapture::captureLoop() {
    cv::VideoCapture cap(videoFilePath_);

    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open video file " << videoFilePath_ << std::endl;
        running_ = false;
        return;
    }

    double fps = cap.get(cv::CAP_PROP_FPS);
    if (fps <= 0) {
        fps = 30; // Default to 30 FPS if retrieval fails
        std::cerr << "[FrameCapture] Warning: Could not get FPS from video. Defaulting to 30 FPS." << std::endl;
    }

    double frameInterval = 1000.0 / fps; // Frame interval in milliseconds

    int frameNumber = 0;
    cv::Mat frame;
    auto nextFrameTime = std::chrono::steady_clock::now();

    while (running_) {
        auto currentTime = std::chrono::steady_clock::now();
        if (currentTime < nextFrameTime) {
            std::this_thread::sleep_until(nextFrameTime);
        }

        bool ret = cap.read(frame);
        if (!ret) {
            std::cout << "[FrameCapture] End of video or cannot read the frame." << std::endl;
            running_ = false; // Stop the loop
            break;
        }

        frameNumber++;
        for (auto module : modules_) {
            module->processFrame(frame, frameNumber);
        }

        nextFrameTime += std::chrono::milliseconds(static_cast<int>(frameInterval));
    }

    cap.release();
}

