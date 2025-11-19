#include "XThread.h"

#include <process.h>
#include <Windows.h>

static void ThreadMain(void *para)
{
    XThread *th = (XThread *)para;
    th->Main();
    _endthread();
}

XThread::XThread()
{
}

XThread::~XThread()
{
}

bool XThread::Start()
{
    th = ::_beginthread(ThreadMain, 0, this);
    if (((int)th) <= 0)
    {
        return false;
    }
    return true;
}

void XThread::Wait()
{
    ::WaitForSingleObject((HANDLE)th, INFINITE);
}
