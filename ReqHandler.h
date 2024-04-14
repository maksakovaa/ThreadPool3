#pragma once
#include "ThPool.h"

class RequestHandler
{
public:
    RequestHandler();
    ~RequestHandler();
    void pushRequest(FuncType f, int id, int arg);
    void interruptPool();
private:
    ThreadPool m_tpool;
};