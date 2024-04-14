#include "ReqHandler.h"
using namespace std;

thread_local thread_interrupt_struct interrup_flag;

mutex coutLocker;

void taskFunc(int id, int delay)
{
    for (int i = 0; i < delay; i++) {
        if (IntThread::checkInterrupted())
        {
            lock_guard<mutex> l(coutLocker);
            cout << "task " << id << " was interrupted" << endl;
            return;
        }
        this_thread::sleep_for(chrono::seconds(1));
    }
    lock_guard<mutex> l(coutLocker);
    cout << "task " << id << " made by thread_id " << this_thread::get_id() << endl;
}

int main()
{
    srand(0);
    RequestHandler rh;
    for (int i = 0; i < 20; i++)
    {
        rh.pushRequest(taskFunc, i, 1 + rand() % 4);
    }
    this_thread::sleep_for(chrono::seconds(1));
    rh.interruptPool();
    return 0;
}