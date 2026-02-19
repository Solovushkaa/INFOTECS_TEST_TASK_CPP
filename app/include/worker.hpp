#ifndef WORKER_HPP
#define WORKER_HPP

#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <functional>
#include <utility>
#include <condition_variable>
#include "loglevel.hpp"

/**
 * @brief A class that provides execution of functions in a separate thread.
 */
class Worker
{
    // --- Constructors ---
public:
    /**
     * @brief Starts a task execution thread.
     */
    Worker();

    // --- Destructors ---
public:
    ~Worker();

    // --- Public Methods ---
public:
    /**
     * @brief Places the submitted task into the task queue.
     */
    void execute(std::function<void()> &&func);

    // --- Private Methods ---
private:
    /**
     * @brief Executes the first task in the task queue.
     */
    void runWorker();

    // --- Members ---
private:
    std::thread m_worker;                       ///< Thread to execute
    std::queue<std::function<void()>> m_tasks;  ///< Queue for storing tasks
    std::mutex m_queueMutex;                    ///< Mutex for synchronization the task queue

    std::condition_variable m_cv; ///< Condition variable for thread idle optimization

    bool m_stopFlag; ///< Flag for stopping execution of tasks
};

#endif // WORKER_HPP