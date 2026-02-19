#include "worker.hpp"
#include <stdexcept>
#include <iostream>

Worker::Worker()
    : m_stopFlag(false)
{
    m_worker = std::thread([this] {
        runWorker();
    });
}

void Worker::execute(std::function<void()> &&func)
{
    std::unique_lock lock(m_queueMutex);
    m_tasks.emplace(std::move(func));
    lock.unlock();

    m_cv.notify_one();
}

Worker::~Worker() {
    {
        std::lock_guard lock(m_queueMutex);
        m_stopFlag = true;
    }
    m_cv.notify_one();
    
    if (m_worker.joinable()) {
        m_worker.join();
    }
}

void Worker::runWorker()
{
    while (true) {
        std::function<void()> task;
        
        std::unique_lock lock(m_queueMutex);
        m_cv.wait(lock, [this] {
            return !m_tasks.empty() || m_stopFlag;
        });

        if (m_stopFlag && m_tasks.empty()) {
            return;
        }

        if (!m_tasks.empty()) {
            task = std::move(m_tasks.front());
            m_tasks.pop();
        }
        lock.unlock();

        if (task) {
            try {
                task();
            } catch (...) {
                std::cerr << "Exception in thread task\n";
            }
        }
    }
}