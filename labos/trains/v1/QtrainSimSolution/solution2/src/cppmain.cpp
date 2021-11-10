#include <QDebug>

#include <pcosynchro/pcosemaphore.h>

#include "ctrain_handler.h"
#include "locomotive.h"

#include "locomotivebehavior.h"
#include "sharedsection.h"

static Locomotive loco0(0, 10);
static Locomotive loco1(1, 12);

//Arret d'urgence
void emergency_stop()
{
    loco0.arreter();
    loco1.arreter();
    //locomotive.arreter();
    afficher_message("\nSTOP!");
}


//Fonction principale
int cmain()
{
    //Choix de la maquette
    selection_maquette(MAQUETTE_A);

    //Initialisation des aiguillages
    diriger_aiguillage(1,  DEVIE,  0);
    diriger_aiguillage(2,  DEVIE,       0);
    diriger_aiguillage(3,  DEVIE,  0);
    diriger_aiguillage(4,  TOUT_DROIT,  0);
    diriger_aiguillage(5,  TOUT_DROIT,  0);
    diriger_aiguillage(6,  TOUT_DROIT,  0);
    diriger_aiguillage(7,  TOUT_DROIT,  0);
    diriger_aiguillage(8,  DEVIE,       0);
    diriger_aiguillage(9,  DEVIE,       0);
    diriger_aiguillage(10, DEVIE,  0);
    diriger_aiguillage(11, TOUT_DROIT,  0);
    diriger_aiguillage(12, TOUT_DROIT,  0);
    diriger_aiguillage(13, TOUT_DROIT,  0);
    diriger_aiguillage(14, DEVIE,       0);
    diriger_aiguillage(15, DEVIE,  0);
    diriger_aiguillage(16, TOUT_DROIT,  0);
    diriger_aiguillage(17, TOUT_DROIT,  0);
    diriger_aiguillage(18, TOUT_DROIT,  0);
    diriger_aiguillage(19, TOUT_DROIT,  0);
    diriger_aiguillage(20, DEVIE,  0);
    diriger_aiguillage(21, DEVIE,       0);
    diriger_aiguillage(22, TOUT_DROIT,  0);
    diriger_aiguillage(23, TOUT_DROIT,  0);
    diriger_aiguillage(24, TOUT_DROIT,  0);
    // diriger_aiguillage(/*NUMERO*/, /*TOUT_DROIT | DEVIE*/, /*0*/);

    // Affiche un message dans la console de l'application graphique
    afficher_message("Hit play to start the simulation...");

    //Initialisation de la locomotive
//    locomotive.fixerNumero(1);
//    locomotive.fixerVitesse(12);
//    locomotive.fixerPosition(16, 23);
//    locomotive.allumerPhares();
//    locomotive.demarrer();
//    locomotive.afficherMessage("Ready!");


    //Attente du passage sur les contacts
//    for (int i = 0; i < parcours.size(); i++) {
//        attendre_contact(parcours.at(i));
//        afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.")
//                                    .arg(locomotive.numero()).arg(parcours.at(i))));
//        locomotive.afficherMessage(QString("I've reached contact no. %1.").arg(parcours.at(i)));
//    }

    //Arreter la locomotive
//    locomotive.arreter();
//    locomotive.afficherMessage("Yeah, piece of cake!");

    std::shared_ptr<SharedSectionInterface> sharedSection = std::make_shared<SharedSection>();

    // Loco 0
    //loco0.fixerVitesse(0);
    loco0.fixerPosition(25, 32);
    std::vector<int> contacts0 {25, 24, 23, 16, 15, 14, 7, 6, 5, 34, 33, 32};

    SharedSectionSettings sssettingsForward0(24, 23, 5,
                                             std::vector{std::pair(9, DEVIE),
                                                         std::pair(2, DEVIE)});
    SharedSectionSettings sssettingsBackward0(33, 34, 16,
                                              std::vector{std::pair(9, DEVIE),
                                                          std::pair(2, DEVIE)});
    Parcours parcours0(contacts0, std::pair(sssettingsForward0, sssettingsBackward0), sharedSection);

    std::unique_ptr<Launchable> locoBehave0 = std::make_unique<LocomotiveBehavior>(loco0, parcours0, SharedSectionInterface::Priority::LowPriority);

    // Loco 1
    //loco1.fixerPosition(28, 29);
    loco1.fixerVitesse(8);
    loco1.fixerPosition(22, 28);
    std::vector<int> contacts1 {22, 21, 20, 19, 13, 15, 14, 7, 6, 1, 31, 30, 29, 28};

    SharedSectionSettings sssettingsForward1(20, 19, 1,
                                             std::vector{std::pair(9, TOUT_DROIT),
                                                         std::pair(2, TOUT_DROIT)});
    SharedSectionSettings sssettingsBackward1(30, 31, 13,
                                              std::vector{std::pair(9, TOUT_DROIT),
                                                          std::pair(2, TOUT_DROIT)});

    Parcours parcours1(contacts1, std::pair(sssettingsForward1, sssettingsBackward1), sharedSection);

    std::unique_ptr<Launchable> locoBehave1 = std::make_unique<LocomotiveBehavior>(loco1, parcours1, SharedSectionInterface::Priority::HighPriority);

    afficher_message("Lancement thread loco 0");
    locoBehave0->startThread();
    afficher_message("Lancement thread loco 1");
    locoBehave1->startThread();

    locoBehave0->join();
    locoBehave1->join();

    //Fin de la simulation
    mettre_maquette_hors_service();

    //Exemple de commande
    //afficher_message("Enter a command in the input field at the top of the window.");
    //QString commande = getCommand();
    //afficher_message(qPrintable(QString("Your command is: ") + commande));

    return EXIT_SUCCESS;
}


/*
//Fonction principale
int cmain()
{
    afficher_message("Hit play to start the simulation...");

    //Choix de la maquette
    selection_maquette(MAQUETTE_B);

    //Initialisation d'un parcours
    QList<int> parcours;
    parcours << 24 << 21 << 16 << 15 << 10 << 11 << 6 << 5;

    //Initialisation des aiguillages
    diriger_aiguillage(16, TOUT_DROIT,  0);
    diriger_aiguillage(15, DEVIE,       0);
    diriger_aiguillage(12, DEVIE,       0);
    diriger_aiguillage(11, DEVIE,       0);
    diriger_aiguillage(9,  TOUT_DROIT,  0);
    diriger_aiguillage(5,  TOUT_DROIT,  0);
    diriger_aiguillage(8,  DEVIE,       0);
    diriger_aiguillage(7,  TOUT_DROIT,  0);
    diriger_aiguillage(4,  TOUT_DROIT,  0);
    diriger_aiguillage(3,  TOUT_DROIT,  0);

    //Initialisation de la locomotive
    locomotive.fixerNumero(1);
    locomotive.fixerVitesse(12);
    locomotive.fixerPosition(24, 5);
    locomotive.allumerPhares();
    locomotive.demarrer();
    locomotive.afficherMessage("Ready!");

    //Attente du passage sur les contacts
    for (int i = 0; i < parcours.size(); i++) {
        attendre_contact(parcours.at(i));
        afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.")
                                    .arg(locomotive.numero()).arg(parcours.at(i))));
        locomotive.afficherMessage(QString("I've reached contact no. %1.").arg(parcours.at(i)));
    }

    //Arreter la locomotive
    locomotive.arreter();
    locomotive.afficherMessage("Yeah, piece of cake!");

    //Fin de la simulation
    mettre_maquette_hors_service();

    //Exemple de commande
    afficher_message("Enter a command in the input field at the top of the window.");
    QString commande = getCommand();
    afficher_message(qPrintable(QString("Your command is: ") + commande));

    return EXIT_SUCCESS;
}
*/
