#ifndef SHAREDSECTION_H
#define SHAREDSECTION_H

#include <map>

#include <QDebug>

#include <pcosynchro/pcosemaphore.h>

#include "locomotive.h"
#include "ctrain_handler.h"
#include "sharedsectioninterface.h"

// Cette solution ne fonctionne QUE pour DEUX locomotives et est inutilement compliquée dans le leave()
// Mais elle fonctionne et permet de faire les tests, il serait bien de la refactoriser et de la rendre
// adaptée à plus de locomotives, par exemple avec deux structures dynamiques, une pour la haute priorité
// (requêtes et locos en attente) et la même pour la basse priorité (requêtes et locos en attente)

class SharedSection final : public SharedSectionInterface
{
    std::unique_ptr<PcoSemaphore> mutex;
    //PcoSemaphore mutex;
    std::map<Priority, std::unique_ptr<PcoSemaphore>> wait;
    //std::vector<PcoSemaphore> wait;

    std::map<Priority, bool> requests;
    std::map<Priority, bool> waiting;

    bool occupied;

public:

    SharedSection() : occupied(false) {
        // Initialize the wainting semaphores
        mutex = std::make_unique<PcoSemaphore>(1);
        //wait = std::vector<std::unique_ptr<PcoSemaphore>>(nbLocos, std::make_unique<PcoSemaphore>(0)); // Does a copy, which does not work !
        wait[Priority::LowPriority] = std::make_unique<PcoSemaphore>(0);
        wait[Priority::HighPriority] = std::make_unique<PcoSemaphore>(0);
        // Initialize the request and wainting tables
        requests[Priority::LowPriority] = false;
        requests[Priority::HighPriority] = false;
        waiting[Priority::LowPriority] = false;
        waiting[Priority::HighPriority] = false;
    }

    void request(Locomotive& loco, Priority priority) override {
        mutex->acquire();
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
            wait[priority]->acquire(); // Waiting
            // We get the mutex here from the thread that did wake us
            loco.demarrer();
        } else {
            // Go on, we still have the mutex here
        }
    }

    void enter(Locomotive& loco, Priority priority) {
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
            } else {
                // Else release the mutex
                mutex->release();
            }
        } else if (requests[Priority::LowPriority]) {
            if (waiting[Priority::LowPriority]) {
                // Wakes a waiting thread and passes the mutex
                wait[Priority::LowPriority]->release();
                // Pass the mutex
            } else {
                // Else release the mutex
                mutex->release();
            }
        } else {
            // Else release the mutex
            mutex->release();
        }
        // This ensures that if a locomotive with lower priority is waiting and a higher priority has requested access, the higher prio gets access
    }
};


#endif // SHAREDSECTION_H
