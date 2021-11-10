#include "locomotivebehavior.h"
#include "ctrain_handler.h"

void LocomotiveBehavior::doAction(int contact)
{
    auto& currentSettings = forward ? parcours.settings.first : parcours.settings.second;

    if (contact == currentSettings.requestContact) {
        parcours.sharedSection->request(loco, priority);
    }
    if (contact == currentSettings.enterOrWaitContact) {
        parcours.sharedSection->getAccess(loco, priority);
        //parcours.sharedSection->waitOnPermission(loco);
        for (auto s : currentSettings.switchSettings) {
            diriger_aiguillage(s.first, s.second, 0);
        }
        //parcours.sharedSection->enter(loco);
    }
    if (contact == currentSettings.leaveContact) {
        parcours.sharedSection->leave(loco);
    }
}

void LocomotiveBehavior::run()
{
    //Initialisation de la locomotive
    loco.allumerPhares();
    loco.demarrer();
    loco.afficherMessage("Ready!");

    for(;;) {
        const unsigned int TOURS = 1;
        //const unsigned int TOURS = loco.numero() == 1 ? 1 : 2;
        for(unsigned int tour = 0; tour < TOURS; tour++) {
            for(unsigned int i=0;i < parcours.contacts.size(); i++) {
                auto contact = forward ? parcours.contacts[i] : parcours.contacts[(i == parcours.contacts.size()-1) ? i : parcours.contacts.size()-(i+2)];
                loco.afficherMessage(qPrintable(QString("The engine no. %1 wait for contact no. %2.")
                                                                .arg(loco.numero()).arg(contact)));
                attendre_contact(contact);
                    loco.afficherMessage(qPrintable(QString("The engine no. %1 has reached contact no. %2.")
                                                                .arg(loco.numero()).arg(contact)));
                doAction(contact);
            }
        }
        loco.inverserSens();
        forward = not forward;
    }
}

void LocomotiveBehavior::printStartMessage()
{
    qDebug() << "[START] Thread de la loco" << loco.numero() << "lancé";
    loco.afficherMessage("Je suis lancée !");
}

void LocomotiveBehavior::printCompletionMessage()
{
    qDebug() << "[STOP] Thread de la loco" << loco.numero() << "a terminé correctement";
    loco.afficherMessage("J'ai terminé");
}
