#ifndef SLOW_MODULE_H
#define SLOW_MODULE_H

#include "Module.h"
#include <opencv2/opencv.hpp>
#include <thread>
#include <atomic>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <string>
#include "Journal.h"
#include <filesystem>

namespace fs = std::filesystem; // For convenience

/**
 * SlowModule saves frames as JPEG files with a limited 5 queue size.
 * Maintains a thread-safe queue with a maximum capacity and saves frames.
 * Inherits from Module
 */
class SlowModule : public Module {
public:
    explicit SlowModule(int sleepPeriodMs = 1000);
    ~SlowModule() override;

    void start() override;
    void stop() override;

    void processFrame(const cv::Mat& frame, int frameNumber) override;

    // Add this public method
    void printJournalSummary();

private:
    void processingLoop();
//    void popSpecificFrame(int frameNumber);
    std::string saveDirectory_;

    std::thread processingThread_;
    std::atomic<bool> running_;

    // Thread-safe deque with max capacity N=5
    std::deque<std::pair<cv::Mat, int>> frameQueue_;
    std::mutex queueMutex_;
    std::condition_variable cv_;
    const size_t maxQueueSize_ = 5;
    int frameQueueLen_=0;

    int sleepPeriodMs_;
    Journal journal_; // This remains private
};

#endif // SLOW_MODULE_H
