#ifndef XTHREAD_H
#define XTHREAD_H

#include "XPlatfrom_Global.h"

class XPLATFORM_EXPORT XThread
{
public:
    XThread();
    virtual ~XThread();

public:
    bool         Start();
    void         Wait();
    virtual void Main() = 0;

private:
    unsigned int th = 0;
};


#endif // XTHREAD_H
