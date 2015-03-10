#include "ctrain_handler.h"

#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

//Contacts a parcourir
#define NB_CONTACTS 11
static int parcours[] = {7, 15, 14, 7, 6, 5, 34, 33, 32, 25, 24};

//Définition d'une locomotive
class Locomotive
{
public:
    int no;
    int vitesse;
};

//Declaration d'une locomotive
static Locomotive loco1;

//Procédure d'arrêt d'urgence
void emergency_stop()
{
    afficher_message("\nSTOP!");
    arreter_loco(loco1.no);
}

int cmain()
{
    int cpt;

    //Numéro et de la vitesse de la locomotive
    loco1.no = 1;
    loco1.vitesse = 12;

    //Allumage des phares de la locomotive
    mettre_fonction_loco(loco1.no, ALLUME);

    //Sélection et initialisation de la maquette
    selection_maquette("MAQUET_A");
    afficher_message("Et c'est parti, en C++!!");

    //Placement de la locomotive entre les contacts 11 et 6
    assigner_loco(16, 23, loco1.no, loco1.vitesse);
    afficher_message_loco(loco1.no,"Hello, je suis la loco 1");

    //Initialisation des aiguillages du parcours
    diriger_aiguillage(8,  DEVIE,       0);
    diriger_aiguillage(2,  DEVIE,       0);
    diriger_aiguillage(20, DEVIE,       0);
    diriger_aiguillage(14, DEVIE,       0);
    diriger_aiguillage(11, TOUT_DROIT,  0);
    diriger_aiguillage(17, TOUT_DROIT,  0);
    diriger_aiguillage(23, TOUT_DROIT,  0);

    //Demarrage de la locomotive
    mettre_vitesse_progressive(loco1.no, loco1.vitesse);

    //Attente du passage de la locomotive sur chacun des contacts du parcours
    for (cpt = 1; cpt < NB_CONTACTS; cpt++) {
        attendre_contact(parcours[cpt]);
        char message[256];
        sprintf(message,"La locomotive %d a atteint le contact no '%d'.", loco1.no, parcours[cpt]);
        afficher_message(message);
        sprintf(message,"La locomotive a atteint le contact no '%d'.", parcours[cpt]);
        afficher_message_loco(loco1.no,message);
    }

    //Arrêt de la locomotive
    arreter_loco(loco1.no);

    //Fin de la simulation (nécessaire sur les maquettes réelles)
    mettre_maquette_hors_service();

    afficher_message("Entrez une commande quelconque dans le champ d'input");
    const char *command = getCommand();
    afficher_message("Voici la commande: ");
    afficher_message(command);

    //Return success
    return EXIT_SUCCESS;
}
