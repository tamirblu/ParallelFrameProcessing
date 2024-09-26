# Parallel Frame Processing

## Requirements

- **C++ Compiler**: Compatible with C++17 or later.
- **OpenCV**: Version 3.0 or higher.
- **CMake**: Version 3.5 or higher (optional, if using CMake for building).

## Building the Application

### **Using CMake**

1. **Clone the Repository**

   ```bash
   git clone https://github.com/tamirblu/ParallelFrameProcessing.git
   cd ParallelFrameProcessing


2. **Create a Build Directory**

   ```bash
    * mkdir build
    * cd build
    * cmake (cmake -DOpenCV_DIR="...\opencv\build" "...\ParallelFrameProcessing")
    * make (or on windows cmake --build . --config Release)
    * Running the Application - *sleep_period_ms is optional*
    * ./ParallelFrameProcessing <video_file_path> [sleep_period_ms] (or ...\RoboticanTask\build\Release\RoboticanTask.exe)
    * Example: ./ParallelFrameProcessing ../data/video.mp4 1000
    ```


---

## **3. Explaining the Design and Code**

In this section, I'll explain the overall design of the application, the reasoning behind key decisions, and reflect on the task.

### **3.1 Design Overview**

The application is designed to process video frames in parallel using multiple threads, following object-oriented programming principles for scalability and maintainability.

#### **Key Components:**

1. **FrameCapture**: Responsible for reading frames from the video file and distributing them to registered processing modules.

2. **Module Base Class**: An abstract class defining the interface for processing modules (`start()`, `stop()`, `processFrame()`).

3. **FastModule**: Calculates and logs the average frame rate every 5 seconds.

4. **SlowModule**: Saves frames as JPEG files, managing a queue with a maximum size to handle backpressure.

5. **Journal**: Logs which frames were saved and which were dropped in the Slow Module.

6. **Logger**: Provides a thread-safe logging mechanism to standardize log messages across the application.

### **3.2 Object-Oriented Design**

- **Encapsulation**: All classes encapsulate their data and provide public methods for interaction. Member variables are kept private to prevent unintended modifications.

- **Inheritance and Polymorphism**: The `Module` base class allows for polymorphic behavior. Both `FastModule` and `SlowModule` inherit from `Module`, enabling `FrameCapture` to interact with any module implementing the `Module` interface.

- **Extensibility**: New modules can be added easily by inheriting from `Module` and implementing the required methods.

### **3.3 Multithreading and Synchronization**

- **Thread Management**: Each module runs in its own thread, started and stopped using the `start()` and `stop()` methods.

- **Synchronization Mechanisms**:

  - **Mutexes**: Used to protect access to shared resources like queues and logs.

  - **Condition Variables**: Employed to signal threads when new frames are available for processing.

- **Thread Safety**: Careful attention is given to ensure that data shared between threads is accessed in a thread-safe manner to prevent race conditions.

### **3.4 Error Handling and Logging**

- **Robust Error Handling**: The application uses try-catch blocks to handle exceptions, particularly around file I/O and OpenCV operations.

- **Consistent Logging**: All log messages are standardized through the `Logger` class, which logs messages to both the console and a log file with timestamps and log levels.

### **3.5 Queue Management in SlowModule**

- **Limited Queue Size**: The Slow Module maintains a queue with a maximum size of 5 frames to prevent memory overconsumption.

- **Frame Dropping**: If the queue is full, the oldest frame is discarded to make room for new frames, and the event is logged in the journal.

### **3.6 How I Approached the Task**

When I received the task, I began by carefully analyzing the requirements to ensure a thorough understanding of what was expected. I focused on:

- **Meeting the Core Requirements**: Implementing frame capture, processing modules, multithreading, and queue management as specified.

- **Designing for Extensibility**: By using an abstract base class for modules, I ensured that new processing modules could be added with minimal changes to the existing code.

- **Ensuring Thread Safety**: Multithreading introduces complexity, so I paid special attention to synchronization mechanisms to prevent race conditions and deadlocks.

- **Implementing Robust Error Handling**: I anticipated potential points of failure, such as file I/O errors and exceptions from OpenCV, and implemented exception handling to make the application robust.

- **Adding Logging for Transparency**: A consistent logging mechanism was important for monitoring the application's behavior and diagnosing issues.

- **Testing and Validation**: I tested the application with various video files and configurations to ensure it performed as expected under different conditions.

### **3.7 Challenges Faced**

- **Synchronization Complexity**: Managing shared resources between multiple threads required careful synchronization to avoid performance bottlenecks.

- **Balancing Performance and Resource Usage**: Implementing a queue in the Slow Module required balancing memory usage with the need to process frames efficiently.

- **Error Propagation**: Ensuring that exceptions in one thread did not cause the entire application to crash was critical, requiring thoughtful error handling strategies.

### **3.8 Learning Outcomes**

- **Deepened Understanding of Multithreading**: This task reinforced my knowledge of multithreading in C++, particularly in the context of real-time processing.

- **Improved Object-Oriented Design Skills**: Designing the application with extensibility in mind highlighted the importance of good OOD principles.

- **Enhanced Debugging Techniques**: Implementing and utilizing a logging system improved my ability to monitor and debug multithreaded applications.

---

## **4. Conclusion**

By adding comprehensive comments and documentation, a detailed README file, and providing an explanation of the design and code, the project is now more accessible and maintainable. These enhancements not only make it easier for others to understand and contribute to the project but also demonstrate a professional approach to software development.

---

**If you have any further questions or need assistance with any part of your project, feel free to ask!**

