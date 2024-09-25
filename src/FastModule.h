#ifndef FAST_MODULE_H
#define FAST_MODULE_H

#include "Module.h"
#include <opencv2/opencv.hpp>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <chrono>

class FastModule : public Module {
public:
    explicit FastModule();
    ~FastModule() override;

    void start() override;
    void stop() override;

    void processFrame(const cv::Mat& frame, int frameNumber) override;
    double videoFPS_;

private:
    void processingLoop();
    std::thread processingThread_;
    std::atomic<bool> running_;

    // Queue to hold frames
    std::queue<std::pair<cv::Mat, int>> frameQueue_;
    std::mutex queueMutex_;
    std::condition_variable cv_;

    // For calculating frame rate
    int frameCount_;
    std::chrono::time_point<std::chrono::steady_clock> startTime_;
};

#endif // FAST_MODULE_H
