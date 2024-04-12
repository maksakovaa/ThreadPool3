#pragma once

#include "BlQueue.h"

void taskFunc(int id, int delay);

typedef std::function<void()> task_type;
typedef void (*FuncType) (int, int);

class IntThread;

class ThreadPool {
public:
    ThreadPool();
    void start();
//    void stop();
    void push_task(FuncType f, int id, int arg);
    void threadFunc(int qindex);
    void interrupt();
private:
    int m_thread_count;
    std::vector<IntThread*> m_threads;
    std::vector<BlockedQueue<task_type>> m_thread_queues;
    int m_index;
};