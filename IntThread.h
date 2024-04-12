#pragma once
#include "ThPool.h"
thread_local bool thread_interrupt_flag = false;
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