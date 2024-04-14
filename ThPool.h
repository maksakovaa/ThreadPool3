#pragma once
#include <thread>
#include <iostream>
#include <queue>
#include <future>
#include <condition_variable>
#include <vector>

struct thread_interrupt_struct
{
    thread_interrupt_struct(): thread_interrupt_flag(false), cv(nullptr) {}
    std::mutex m_defender;
    bool thread_interrupt_flag;
    std::condition_variable* cv;
};

extern thread_local thread_interrupt_struct interrup_flag;

class ThreadPool;
void taskFunc(int id, int delay);

typedef std::function<void()> task_type;
typedef void (*FuncType) (int, int);

class IntThread
{
public:
    IntThread(ThreadPool* pool, int qindex);
    ~IntThread();
    void interrupt();
    void startFunc(ThreadPool* pool, int qindex);
    static bool checkInterrupted();
    static void IntWait(std::condition_variable& cv, std::unique_lock<std::mutex> &ul, std::function<bool()> predicate);
private:
    std::mutex m_defender;
    thread_interrupt_struct* m_pFlag;
    std::thread m_thread;
};

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
        {
            IntThread::IntWait(m_notifier, l, [this]{ return !m_task_queue.empty(); });
        }
        if (m_task_queue.empty()) return;
        item = move(m_task_queue.front());
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

class ThreadPool {
public:
    ThreadPool();
    void start();
    void stop();
    void push_task(FuncType f, int id, int arg);
    void threadFunc(int qindex);
    void interrupt();
private:
    int m_thread_count;
    std::vector<IntThread*> m_threads;
    std::vector<BlockedQueue<task_type>> m_thread_queues;
    int m_index;
};