#ifndef SHAREDSECTIONINTERFACE_H
#define SHAREDSECTIONINTERFACE_H

#include "locomotive.h"

class SharedSectionInterface
{
public:

    enum class Priority {
        LowPriority,
        HighPriority
    };

    virtual void request(Locomotive& loco, Priority priority) = 0;
    virtual void getAccess(Locomotive& loco, Priority priority) = 0;
    virtual void leave(Locomotive& loco) = 0;
};

#endif // SHAREDSECTIONINTERFACE_H
