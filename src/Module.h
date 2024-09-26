#ifndef MODULE_H
#define MODULE_H

#include <opencv2/opencv.hpp>
/**
 * The Module class defines the interface for processing modules that can
 * be started, stopped, and can process frames from a video stream.
 */
class Module {
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void processFrame(const cv::Mat& frame, int frameNumber) = 0;
    virtual ~Module() = default;
};

#endif // MODULE_H
