#include "XMutex.h"

#include <windows.h>

XMutex::XMutex()
{
    this->setion = new CRITICAL_SECTION();
    ::InitializeCriticalSection((CRITICAL_SECTION *)this->setion);
}

void XMutex::Lock()
{
    ::EnterCriticalSection((CRITICAL_SECTION *)this->setion);
}

void XMutex::Unlock()
{
    ::LeaveCriticalSection((CRITICAL_SECTION *)this->setion);
}
