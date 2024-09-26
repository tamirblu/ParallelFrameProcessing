#ifndef JOURNAL_H
#define JOURNAL_H

#include <mutex>
#include <vector>
/**
 *  Journal class for logging saved and dropped frames.
 */
class Journal {
public:
    void logSavedFrame(int frameNumber);
    void logDroppedFrame(int frameNumber);

    void printSummary();

private:
    std::mutex mutex_;
    std::vector<int> savedFrames_;
    std::vector<int> droppedFrames_;
};

#endif // JOURNAL_H
