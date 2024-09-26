#ifndef FRAME_CAPTURE_H
#define FRAME_CAPTURE_H

#include <opencv2/opencv.hpp>
#include <string>
#include <thread>
#include <atomic>
#include <vector>
#include "Module.h"

/**
 * FrameCapture class captures frames from a video file and sends them to registered modules.
 */
class FrameCapture {
public:
    explicit FrameCapture(std::string  videoFilePath);
    ~FrameCapture();
    void join();
    void start();
    void stop();
    void addModule(Module* module);

private:
    void captureLoop();

    std::string videoFilePath_;
    std::thread captureThread_;
    std::atomic<bool> running_;
    std::vector<Module*> modules_;

};

#endif // FRAME_CAPTURE_H
