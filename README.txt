~ Parallel Frame Processing 

~~ Requirements

- **C++ Compiler**: Compatible with C++17 or later.
- **OpenCV**: Version 3.0 or higher.
- **CMake**: Version 3.5 or higher (optional, if using CMake for building).

~~ Building the Application

~~~ **Using CMake**

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

~~ **3. Explaining the Design and Code**

In this section, I'll explain the overall design of the application, the reasoning behind key decisions, and reflect on the task.

~~~ **3.1 Design Overview**

~~~~ **Key Components:**

1. **FrameCapture**: Responsible for reading frames from the video file and distributing them to registered processing modules.

2. **Module Base Class**: An abstract class defining the interface for processing modules (`start()`, `stop()`, `processFrame()`).

3. **FastModule**: Calculates and logs the average frame rate every 5 seconds.

4. **SlowModule**: Saves frames as JPEG files, managing a queue with a maximum size to handle backpressure.

5. **Journal**: Logs which frames were saved and which were dropped in the Slow Module.

~~~ **3.2 Object-Oriented Design**

- **Encapsulation**: All classes encapsulate their data and provide public methods for interaction. Member variables are kept private to prevent unintended modifications.

- **Inheritance and Polymorphism**: The `Module` base class allows for polymorphic behavior. Both `FastModule` and `SlowModule` inherit from `Module`, enabling `FrameCapture` to interact with any module implementing the `Module` interface.

- **Extensibility**: New modules can be added easily by inheriting from `Module` and implementing the required methods.

~~~ **3.3 Multithreading and Synchronization**

- **Thread Management**: Each module runs in its own thread, started and stopped using the `start()` and `stop()` methods.

- **Synchronization Mechanisms**:

  - **Mutexes**: Used to protect access to shared resources like queues and logs.

  - **Condition Variables**: Employed to signal threads when new frames are available for processing.

- **Thread Safety**: Careful attention is given to ensure that data shared between threads is accessed in a thread-safe manner to prevent race conditions.

~~~ **3.4 Error Handling and Logging**

- **Robust Error Handling**: The application uses try-catch blocks to handle exceptions, particularly around file I/O and OpenCV operations.

- **Consistent Logging**: Logs messages to both the console and saving "logs" to the journal.

~~~ **3.5 Queue Management in SlowModule**

- **Limited Queue Size**: The Slow Module maintains a queue (deque) with a maximum size of 5 frames to prevent memory overconsumption.

- **Frame Dropping**: If the queue is full, the oldest frame is discarded to make room for new frames, and the event is logged in the journal.

~~~ **3.6 How I Approached the Task**

When I received the task, I began by carefully analyzing the requirements to ensure a thorough understanding of what was expected. I focused on:

- **Meeting the Core Requirements**: Implementing frame capture, processing modules, multithreading, and queue management as specified.

- **Designing for Extensibility**: By using an abstract base class for modules, I ensured that new processing modules could be added with minimal changes to the existing code.

- **Ensuring Thread Safety**: Multithreading introduces complexity, so I paid special attention to synchronization mechanisms to prevent race conditions and deadlocks.

- **Implementing Robust Error Handling**: I focused  on potential points of failure, such as file I/O errors and exceptions from OpenCV, and implemented exception handling to make the application robust.

- **Adding Logging for Transparency**: A consistent logging was important for monitoring the application's behavior and diagnosing issues.

- **Testing and Validation**: I tested the application with various configurations to ensure it performed as expected under different conditions.

~~~ **3.7 Challenges Faced**

- **Synchronization Complexity**: Managing shared resources between multiple threads required careful synchronization to avoid performance bottlenecks.

- **Syntax Problems**: Working with a programming language I'm not familiar with proved challenging due to syntax differences.

~~~ **3.8 Learning Outcomes**

- **Deepened Understanding of Multithreading**: This task reinforced my knowledge of multithreading in C++, particularly in the context of real-time processing.

- **Improved Object-Oriented Design Skills**: Designing the application with extensibility in mind highlighted the importance of good OOD principles.

- **C++ Syntax**: This task Enhanced my knowledge of in C++ programming.
---


**thanks!**

