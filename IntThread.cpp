#include "IntThread.h"

IntThread::IntThread(ThreadPool* pool, int qindex)
{
    m_pFlag = nullptr;
    m_thread = std::thread(&IntThread::startFunc, this, pool, qindex);
}

IntThread::~IntThread()
{
    m_thread.join();
}

bool IntThread::checkInterrupted()
{
    return thread_interrupt_flag;
}

void IntThread::startFunc(ThreadPool* pool, int qindex)
{
    {
        std::lock_guard<std::mutex> l(m_defender);
        m_pFlag = &thread_interrupt_flag;
    }
    pool->threadFunc(qindex);
    {
        std::lock_guard<std::mutex> l(m_defender);
        m_pFlag = nullptr;
    }
}

void IntThread::interrupt()
{
    std::lock_guard<std::mutex> l(m_defender);
    if (m_pFlag)
    {
        *m_pFlag = true;
    }
}