#ifndef SHAREDSECTION_H
#define SHAREDSECTION_H

#include <map>

#include <QDebug>

#include <pcosynchro/pcosemaphore.h>

#include "locomotive.h"
#include "ctrain_handler.h"
#include "sharedsectioninterface.h"

#ifndef ERRNO
#define ERRNO 0
#else
#warning "WARNING ERRNO is set !"
#if ERRNO == 0
#warning "ERRNO : but should not break the design"
#else
#warning "ERRNO : with a design breaking value !"
// Comments in reference to unit tests for this lab
// ERRNO 1 - Shows problem in valgrind --tool=helgrind
// ERRNO 2 - Blocks
// ERRNO 3 - Fails tests (does not block when should), also shows in valgrind --tool=helgrind
// ERRNO 4 - Blocks, fails concurrent test
// ERRNO 5 - Blocks
// ERRNO 6 - Blocks
// ERRNO 7 - Shows problem in valgrind --tool=helgrind
// ERRNO 8 - Blocks
#endif
#endif

class SharedSection final : public SharedSectionInterface
{
    std::unique_ptr<PcoSemaphore> mutex;
    std::map<Priority, std::unique_ptr<PcoSemaphore>> wait;

    std::map<Priority, bool> requests;
    std::map<Priority, bool> waiting;

    bool occupied;

public:

    SharedSection() : occupied(false) {
        // Initialize the wainting semaphores
        mutex = std::make_unique<PcoSemaphore>(1);
        wait[Priority::LowPriority] = std::make_unique<PcoSemaphore>(0);
        wait[Priority::HighPriority] = std::make_unique<PcoSemaphore>(0);
        // Initialize the request and wainting tables
        requests[Priority::LowPriority] = false;
        requests[Priority::HighPriority] = false;
        waiting[Priority::LowPriority] = false;
        waiting[Priority::HighPriority] = false;
#if ERRNO != 0
        qDebug() << "ERRNO is set to" << ERRNO;
#endif
    }

    void request(Locomotive& loco, Priority priority) override {
#if ERRNO != 1
        mutex->acquire();
#else
        // ERRNO 1 is missing an acquire
#endif

#if ERRNO == 2
        // ERRNO 2 two acquires
        mutex->acquire();
#endif
        afficher_message(qPrintable(QString("The engine no. %1 requested the critical section.")
                                     .arg(loco.numero())));
        requests[priority] = true;
        mutex->release();
    }

private:
    void waitOnPermission(Locomotive& loco, Priority priority) {
        mutex->acquire();
        if (occupied or (requests[Priority::HighPriority] && (priority == Priority::LowPriority))) {
            // Stop
            waiting[priority] = true;
            mutex->release();
            loco.arreter();
            afficher_message(qPrintable(QString("The engine no. %1 will wait.")
                                         .arg(loco.numero())));
#if ERRNO != 3
            wait[priority]->acquire(); // Waiting
#else
            // ERRNO 3 missing wait statement
#endif
#if ERRNO == 4
            // ERRNO 4 double wait
            wait[priority]->acquire(); // Waiting
#endif
            // We get the mutex here from the thread that did wake us
            loco.demarrer();
        } else {
#if ERRNO == 5
            // ERRNO 5 wait when it is our turn
            wait[priority]->acquire(); // Waiting
#endif
            // Go on, we still have the mutex here
        }
    }

    void enter(Locomotive& loco, Priority priority) {
#if ERRNO == 6
        // ERRNO 6 mutex is already owned, extra acquire
        mutex->acquire(); // Bad
#endif
        requests[priority] = false;
        waiting[priority] = false;
        afficher_message(qPrintable(QString("The engine no. %1 enters.")
                                     .arg(loco.numero())));
        occupied = true;
        mutex->release();
    }
public:
    void getAccess(Locomotive &loco, Priority priority) override {
        waitOnPermission(loco, priority);
        enter(loco, priority);
    }

    void leave(Locomotive& loco) override {
        mutex->acquire();
        afficher_message(qPrintable(QString("The engine no. %1 leaves.")
                                     .arg(loco.numero())));
        occupied = false;

        // The next to get access is the loco with highest priority (between waiting and current requests)
        if (requests[Priority::HighPriority]) {
            if (waiting[Priority::HighPriority]) {
                // Wakes a waiting thread and passes the mutex
                wait[Priority::HighPriority]->release();
                // Pass the mutex
#if ERRNO == 7
                // Mutex should not be released here
                mutex->release();
#endif
            } else {
                // Else release the mutex
#if ERRNO != 8
                mutex->release();
#else
                // Missing a release here
#endif
            }
        } else if (requests[Priority::LowPriority]) {
            if (waiting[Priority::LowPriority]) {
                // Wakes a waiting thread and passes the mutex
                wait[Priority::LowPriority]->release();
                // Pass the mutex
#if ERRNO == 7
                // Mutex should not be released here
                mutex->release();
#endif
            } else {
                // Else release the mutex
#if ERRNO != 8
                mutex->release();
#else
                // Missing a release here
#endif
            }
        } else {
            // Else release the mutex
#if ERRNO != 8
            mutex->release();
#else
            // Missing a release here
#endif
        }
        // This ensures that if a locomotive with lower priority is waiting and a higher priority has requested access, the higher prio gets access
    }
};

#endif // SHAREDSECTION_H
