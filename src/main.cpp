#include "FrameCapture.h"
#include "FastModule.h"
#include "SlowModule.h"
#include <iostream>
#include <mutex>



/**
 *  Entry point of the application.
 * Parses command-line arguments, initializes modules, and starts the frame capture process.
 */
int main(int argc, char* argv[]) {
   try{
       // Default value (can be overridden by command line argument)
       int sleepPeriod = 1000;

       //   option 1 hardcoded:
       std::string videoFilePath = R"(mp4_path.mp4)";
       //   option 2 from command line:
       if (argc >= 2){
            videoFilePath = argv[1];
        }
        if (argc >= 3) {
            sleepPeriod = std::stoi(argv[2]);
        }

       cv::VideoCapture cap(videoFilePath);

       if (!cap.isOpened()) {
           std::cerr << "Error: Could not open video file " << videoFilePath << std::endl;
           return -1;
       }

       // Create processing modules
       FastModule fastModule;
       SlowModule slowModule(sleepPeriod); // Sleep period after file save in milliseconds

       // Set FPS for FastModule
       fastModule.fpsModify(cap.get(cv::CAP_PROP_FPS));

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
   }
   catch (const cv::Exception& e) {
       std::cerr << "OpenCV exception in main: " << e.what() << std::endl;
   }
    catch (std::exception& e) {
         std::cerr << "Exception in main: " << e.what() << std::endl;
    }
    return 0;
}
