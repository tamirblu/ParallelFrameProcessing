#include "FrameCapture.h"
#include "FastModule.h"
#include "SlowModule.h"
#include <iostream>
#include <mutex>



int main(int argc, char* argv[]) {
//    const std::string videoFilePath = R"(C:\Users\koren\Desktop\RoboticanTask\12248500_1920_1080_30fps.mp4)"; // Replace with actual path
//    .\RoboticanTask.exe "C:\Users\koren\Desktop\RoboticanTask\12248500_1920_1080_30fps.mp4"
//   PS C:\Users\koren\Desktop\RoboticanTask\cmake-build-debug-visual-studio\Debug> .\RoboticanTask.exe "C:\Users\koren\Desktop\RoboticanTask\12248500_1920_1080_30fps.mp4"
    int sleepPeriod = 1000; // Default value

//   option 1 hardcoded:
  const std::string videoFilePath = R"(C:\Users\koren\Desktop\RoboticanTask\12248500_1920_1080_30fps.mp4)"; // Replace with actual path

//   option 2 from command line:
//    if (argc < 2) {
//        std::cerr << "Usage: " << argv[0] << " <video_file_path> [sleep_period]" << std::endl;
//        return -1;
//    }
//    std::string videoFilePath = argv[1];
//    if (argc >= 3) {
//        sleepPeriod = std::stoi(argv[2]);
//    }
    cv::VideoCapture cap(videoFilePath);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open video file " << videoFilePath << std::endl;
        return -1;
    }

    // Create processing modules
    FastModule fastModule;
    SlowModule slowModule(sleepPeriod); // Sleep period after file save in milliseconds
    fastModule.videoFPS_ = cap.get(cv::CAP_PROP_FPS);
    // Start processing modules
    fastModule.start();
    slowModule.start();

    // Create FrameCapture and register modules
    FrameCapture frameCapture(videoFilePath);
    frameCapture.addModule(&fastModule);
    frameCapture.addModule(&slowModule);

    // Start frame capture
    frameCapture.start();

    // Wait for frame capture to finish
    frameCapture.join();
    frameCapture.stop();
    // Stop all modules
    fastModule.stop();
    slowModule.stop();

    // Print journal summary
    slowModule.printJournalSummary();

    return 0;
}
