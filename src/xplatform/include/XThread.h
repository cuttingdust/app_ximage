#ifndef XTHREAD_H
#define XTHREAD_H

#include "XPlatfrom_Global.h"

class XPLATFORM_EXPORT XThread
{
public:
    bool         Start();
    void         Wait();
    virtual void Main() = 0;
    void         Suspend();
    void         Resume();

private:
    unsigned int th_ = 0;
};


#endif // XTHREAD_H
