#ifndef FRAME_CAPTURE_H
#define FRAME_CAPTURE_H

#include <opencv2/opencv.hpp>
#include <string>
#include <thread>
#include <atomic>
#include <vector>
#include <functional> // Include functional header
#include "Module.h"

class FrameCapture {
public:
//  Constructor :  explicit: Prevents implicit conversions, ensuring the constructor is called only with a std::string.
    explicit FrameCapture(std::string  videoFilePath);
//Destructor (~FrameCapture()): Ensures resources are cleaned up when the object is destroyed.
    ~FrameCapture();

//    Control the lifecycle of the frame capture thread.
    void join();
    void start();
    void stop();

    // Register processing modules
    //    Allows registration of processing modules using std::function, which can store lambdas, function pointers, or bind expressions.
//    void addModule(std::function<void(const cv::Mat&, int)> moduleFunc);
    void addModule(Module* module);

private:
    void captureLoop();
    //    std::string videoFilePath_: Stores the path to the video file.
    //std::thread captureThread_: Thread for capturing frames.
    //std::atomic<bool> running_: Atomic flag to control the running state of the thread safely across multiple threads.
    //std::vector<std::function<void(const cv::Mat&, int)>> modules_: Stores the registered processing modules.
    std::string videoFilePath_;
    std::thread captureThread_;
    std::atomic<bool> running_;
    std::vector<Module*> modules_;

};

#endif // FRAME_CAPTURE_H
