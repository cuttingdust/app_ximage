#ifndef XMUTEX_H
#define XMUTEX_H

#include "XPlatfrom_Global.h"

class XPLATFORM_EXPORT XMutex
{
public:
    XMutex();

public:
    void Lock();
    void Unlock();

private:
    void *setion;
};


#endif // XMUTEX_H
