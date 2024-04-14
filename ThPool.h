#pragma once
#include "BlQueue.h"

extern thread_local bool thread_interrupt_flag;

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
private:
    std::mutex m_defender;
    bool* m_pFlag;
    std::thread m_thread;
};

class ThreadPool
{
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