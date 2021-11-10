#include "ctrain_handler.h"
#include "locomotive.h"

#include <QList>
#include <QSemaphore>
#include <QThread>


#define REQUEST

#define ACTION_NOOP          0
#define ACTION_ENTER_SECTION 1
#define ACTION_LEAVE_SECTION 2
#define ACTION_ASK_SECTION   3



#ifdef REQUEST
class SharedSection
{
    QSemaphore mutex;
    QSemaphore attente;
    bool occupied;
    int enAttente;
    std::vector<bool> requests;

public:
    SharedSection() : mutex(1), attente(0), occupied(false), enAttente(0) {
        requests = std::vector<bool>(2);
        requests[0] = false;
        requests[1] = false;
    }

    void request(Locomotive &loco)
    {
        requests[loco.priority] = true;
    }

    void enter(Locomotive &loco)
    {
        mutex.acquire();
        if ((occupied) || ((loco.priority == 0) && (requests[1]))) {
            enAttente ++;
            requests[loco.priority] = false;
            mutex.release();
            loco.arreter();
            attente.acquire();
            loco.demarrer();
        }
        else {
            occupied = true;
            requests[loco.priority] = false;
            mutex.release();
        }
    }

    void leave(Locomotive &/*loco*/)
    {
        mutex.acquire();
        if (enAttente) {
            enAttente --;
            attente.release();
        }
        else
            occupied = false;
        mutex.release();
    }

};

#else

class SharedSection
{
    QSemaphore mutex;
    QSemaphore attente;
    bool occupied;
    int enAttente;
public:
    SharedSection() : mutex(1), attente(0), occupied(false), enAttente(0) {}

    void enter(Locomotive &loco)
    {
        mutex.acquire();
        if (occupied) {
            enAttente ++;
            mutex.release();
            loco.arreter();
            attente.acquire();
            loco.demarrer();
        }
        else {
            occupied = true;
            mutex.release();
        }
    }

    void leave(Locomotive &/*loco*/)
    {
        mutex.acquire();
        if (enAttente) {
            enAttente --;
            attente.release();
        }
        else
            occupied = false;
        mutex.release();
    }

};
#endif // REQUEST


/*
 * Using structures could ease the declaration of parcours.
 * However it is not necessarily more readable
 */
typedef struct
{
    int number;
    int sens;
} ActionAiguille_t;

typedef struct
{
    int contact;
    int specificAction;
    std::vector<ActionAiguille_t> aiguillages;
} ActionContact_t;

typedef struct
{
    int depart0;
    int depart1;
    std::vector<ActionContact_t> list;
} Parcours_t;

ActionContact_t a = {0, 1, {{0, 1}}};
Parcours_t p00 = {
    0,
    1,
    {
        ActionContact_t {
            0,
            1,
            {
                ActionAiguille_t{0, 1}
            }
        },
        ActionContact_t {
            0,
            1,
            {
                ActionAiguille_t{0, 1}
            }
        }
    }
};

Parcours_t p01 = {
    0,
    1,
    {
        {
            0,
            1,
            {
                {0, 1}
            }
        },
        {
            0,
            1,
            {
                {0, 1}
            }
        }
    }
};

class ActionAiguille
{
public:
    ActionAiguille(int number, int sens) : number(number), sens(sens) {}
    int number;
    int sens;
};

class ActionContact
{
public:
    int contact;
    int specificAction;
    QList<ActionAiguille> aiguillages;
};

class Parcours
{
public:
    int depart0;
    int depart1;
    QList<ActionContact> list;
};


class LocoThread : public QThread
{
public:
    Locomotive locomotive;
    SharedSection *sharedSection;
    int locoId;
    Parcours parcours0;
    Parcours parcours1;
    bool stopped;

    // Could be called from another thread
    void emergencyStop()
    {
        stopped = true;
        locomotive.emergencyStop();
    }

    void doSpecificAction(ActionContact &action)
    {
        switch (action.specificAction) {
        case ACTION_ENTER_SECTION : sharedSection->enter(locomotive); break;
#ifdef REQUEST
        case ACTION_ASK_SECTION : sharedSection->request(locomotive); break;
#endif // REQUEST
        case ACTION_LEAVE_SECTION : sharedSection->leave(locomotive); break;
        default : break;
        }
    }

    void execute(ActionContact &action)
    {
        attendre_contact(action.contact);
        afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.")
                                    .arg(locoId).arg(action.contact)));
        afficher_message_loco(locoId, qPrintable(QString("I've reached contact no. %1.").arg(action.contact)));

        doSpecificAction(action);
        for(int i = 0; i < action.aiguillages.size() ; i++) {
            diriger_aiguillage(action.aiguillages[i].number,
                               action.aiguillages[i].sens,
                               0);
        }
    }

    void run()
    {
        //Initialisation de la locomotive
        locomotive.fixerNumero(locoId);
        locomotive.fixerVitesse(12);
        locomotive.fixerPosition(parcours0.depart0, parcours0.depart1);
        locomotive.allumerPhares();
        locomotive.demarrer();
        locomotive.afficherMessage("Ready!");


        while (true) {
            for(int tour = 0; tour < 2; tour ++) {
                for(int i=0;i<parcours0.list.size(); i++) {
                    execute(parcours0.list[i]);
                }
            }
            locomotive.inverserSens();
            for(int tour = 0; tour < 2; tour ++) {
                for(int i=0;i<parcours1.list.size(); i++) {
                    execute(parcours1.list[i]);
                }
            }
            locomotive.inverserSens();
        }
    }

};


static LocoThread loco1;
static LocoThread loco2;

//Arret d'urgence
void emergency_stop()
{
    loco1.emergencyStop();
    loco2.emergencyStop();
    afficher_message("\nSTOP!");
}


//Fonction principale
int cmain()
{
    afficher_message("Hit play to start the simulation...");

    //Choix de la maquette
    selection_maquette(MAQUETTE_A);

    SharedSection section;
    loco1.sharedSection = &section;
    loco2.sharedSection = &section;
    loco1.locomotive.priority = 0;
    loco2.locomotive.priority = 1;

    loco1.locoId = 1;
    loco2.locoId = 2;


    {
        Parcours parcoursLoco1Forward;
        parcoursLoco1Forward.depart0 = 16;
        parcoursLoco1Forward.depart1 = 23;

        {
            ActionContact a;
            a.contact = 16;
            a.specificAction = 0;
            a.aiguillages << ActionAiguille(9, DEVIE)
                          << ActionAiguille(8, DEVIE)
                          << ActionAiguille(11, TOUT_DROIT)
                          << ActionAiguille(5, TOUT_DROIT)
                          << ActionAiguille(3, DEVIE)
                          << ActionAiguille(2, TOUT_DROIT);
            parcoursLoco1Forward.list << a;
        }
#ifdef REQUEST
        {
            ActionContact a;
            a.contact = 15;
            a.specificAction = ACTION_ASK_SECTION;
            parcoursLoco1Forward.list << a;
        }
#endif // REQUEST
        {
            ActionContact a;
            a.contact = 7;
            a.specificAction = ACTION_ENTER_SECTION;
            a.aiguillages << ActionAiguille(1, DEVIE)
                          << ActionAiguille(22, DEVIE)
                          << ActionAiguille(21, TOUT_DROIT)
                          << ActionAiguille(20, DEVIE)
                          << ActionAiguille(23, TOUT_DROIT)
                          << ActionAiguille(17, TOUT_DROIT)
                          << ActionAiguille(15, DEVIE)
                          << ActionAiguille(14, DEVIE);
            parcoursLoco1Forward.list << a;
        }
        {
            ActionContact a;
            a.contact = 33;
            a.specificAction = ACTION_LEAVE_SECTION;
            parcoursLoco1Forward.list << a;
        }
        {
            ActionContact a;
            a.contact = 23;
            a.specificAction = ACTION_NOOP;
            parcoursLoco1Forward.list << a;
        }
        loco1.parcours0 = parcoursLoco1Forward;
    }

    {
        Parcours parcoursLoco1Back;
        {
            ActionContact a;
            a.contact = 23;
            a.specificAction = 0;
            a.aiguillages << ActionAiguille(14, DEVIE)
                          << ActionAiguille(15, DEVIE)
                          << ActionAiguille(17, TOUT_DROIT)
                          << ActionAiguille(23, TOUT_DROIT)
                          << ActionAiguille(20, DEVIE)
                          << ActionAiguille(21, TOUT_DROIT);
            parcoursLoco1Back.list << a;
        }
#ifdef REQUEST
        {
            ActionContact a;
            a.contact = 25;
            a.specificAction = ACTION_ASK_SECTION;
            parcoursLoco1Back.list << a;
        }
#endif // REQUEST
        {
            ActionContact a;
            a.contact = 32;
            a.specificAction = ACTION_ENTER_SECTION;
            a.aiguillages << ActionAiguille(22, DEVIE)
                          << ActionAiguille(1, DEVIE)
                          << ActionAiguille(2, TOUT_DROIT)
                          << ActionAiguille(3, DEVIE)
                          << ActionAiguille(5, TOUT_DROIT)
                          << ActionAiguille(11, TOUT_DROIT)
                          << ActionAiguille(8, DEVIE)
                          << ActionAiguille(9, DEVIE);
            parcoursLoco1Back.list << a;
        }
        {
            ActionContact a;
            a.contact = 6;
            a.specificAction = ACTION_LEAVE_SECTION;
            parcoursLoco1Back.list << a;
        }
        {
            ActionContact a;
            a.contact = 16;
            parcoursLoco1Back.list << a;
        }
        loco1.parcours1 = parcoursLoco1Back;
    }



    {
        Parcours parcoursLoco2Forward;
        parcoursLoco2Forward.depart0 = 13;
        parcoursLoco2Forward.depart1 = 19;

        {
            ActionContact a;
            a.contact = 13;
            a.specificAction = 0;
            a.aiguillages << ActionAiguille(10, TOUT_DROIT)
                          << ActionAiguille(7, TOUT_DROIT)
                          << ActionAiguille(4, TOUT_DROIT);
            parcoursLoco2Forward.list << a;
        }
#ifdef REQUEST
        {
            ActionContact a;
            a.contact = 4;
            a.specificAction = ACTION_ASK_SECTION;
            parcoursLoco2Forward.list << a;
        }
#endif // REQUEST

        {
            ActionContact a;
            a.contact = 3;
            a.specificAction = ACTION_ENTER_SECTION;
            a.aiguillages << ActionAiguille(1, TOUT_DROIT)
                          << ActionAiguille(22, TOUT_DROIT)
                          << ActionAiguille(19, TOUT_DROIT)
                          << ActionAiguille(16, TOUT_DROIT)
                          << ActionAiguille(13, TOUT_DROIT);
            parcoursLoco2Forward.list << a;
        }
        {
            ActionContact a;
            a.contact = 30;
            a.specificAction = ACTION_LEAVE_SECTION;
            parcoursLoco2Forward.list << a;
        }
        {
            ActionContact a;
            a.contact = 19;
            a.specificAction = ACTION_NOOP;
            parcoursLoco2Forward.list << a;
        }
        loco2.parcours0 = parcoursLoco2Forward;
    }

    {
        Parcours parcoursLoco2Back;
        {
            ActionContact a;
            a.contact = 19;
            a.specificAction = 0;
            a.aiguillages << ActionAiguille(13, TOUT_DROIT)
                          << ActionAiguille(16, TOUT_DROIT)
                          << ActionAiguille(19, TOUT_DROIT);
            parcoursLoco2Back.list << a;
        }
#ifdef REQUEST
        {
            ActionContact a;
            a.contact = 28;
            a.specificAction = ACTION_ASK_SECTION;
            parcoursLoco2Back.list << a;
        }
#endif // REQUEST

        {
            ActionContact a;
            a.contact = 29;
            a.specificAction = ACTION_ENTER_SECTION;
            a.aiguillages << ActionAiguille(22, TOUT_DROIT)
                          << ActionAiguille(1, TOUT_DROIT)
                          << ActionAiguille(4, TOUT_DROIT)
                          << ActionAiguille(7, TOUT_DROIT)
                          << ActionAiguille(10, TOUT_DROIT);
            parcoursLoco2Back.list << a;
        }
        {
            ActionContact a;
            a.contact = 2;
            a.specificAction = ACTION_LEAVE_SECTION;
            parcoursLoco2Back.list << a;
        }
        {
            ActionContact a;
            a.contact = 13;
            parcoursLoco2Back.list << a;
        }
        loco2.parcours1 = parcoursLoco2Back;
    }




    loco1.start();
    loco2.start();

    loco1.wait();
    loco2.wait();

    //Fin de la simulation
    mettre_maquette_hors_service();

    return EXIT_SUCCESS;
}
