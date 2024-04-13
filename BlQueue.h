#pragma once
#include <thread>
#include <iostream>
#include <queue>
#include <future>
#include <condition_variable>
#include <vector>



template<class T>
class BlockedQueue
{
public:
    void push(T& item)
    {
        std::lock_guard<std::mutex> l(m_locker);
        m_task_queue.push(item);
        m_notifier.notify_one();
    }

    void pop(T& item)
    {
        std::unique_lock<std::mutex> l(m_locker);
        if (m_task_queue.empty())
            m_notifier.wait(l, [this] {return !m_task_queue.empty(); });
        item = m_task_queue.front();
        m_task_queue.pop();
    }

    bool fast_pop(T& item)
    {
        std::lock_guard<std::mutex> l(m_locker);
        if (m_task_queue.empty())
            return false;
        item = m_task_queue.front();
        m_task_queue.pop();
        return true;
    }
private:
    std::mutex m_locker;
    std::queue<T> m_task_queue;
    std::condition_variable m_notifier;
};