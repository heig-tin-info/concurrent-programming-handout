#ifndef LOCOMOTIVEBEHAVIOR_H
#define LOCOMOTIVEBEHAVIOR_H

#include "locomotive.h"
#include "parcours.h"
#include "launchable.h"

class LocomotiveBehavior : public Launchable
{
public:
    /*!
     * \brief locomotiveBehavior Constructeur de la classe
     * \param loco la locomotive dont on représente le comportement
     */
    LocomotiveBehavior(Locomotive& loco, Parcours& parcours, SharedSectionInterface::Priority highPriority) : loco(loco), parcours(parcours), priority(highPriority) {}

protected:
    /*!
     * \brief run Fonction lancée par le thread, représente le comportement de la locomotive
     */
    void run() override;

    /*!
     * \brief printStartMessage Message affiché lors du démarrage du thread
     */
    void printStartMessage() override;

    /*!
     * \brief printCompletionMessage Message affiché lorsque le thread a terminé
     */
    void printCompletionMessage() override;

    void doAction(int contact);

    Locomotive& loco;
    Parcours& parcours;
    SharedSectionInterface::Priority priority;
    bool forward = true;
};

#endif // LOCOMOTIVEBEHAVIOR_H
