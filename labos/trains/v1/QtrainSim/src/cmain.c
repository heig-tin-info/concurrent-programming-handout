#include <pthread.h>
#include "ctrain_handler.h"
#include <errno.h>
#include <stdio.h>

//Engine definition
typedef struct
{
    int no;
    int vitesse;
} Locomotive;


//Engines declaration
Locomotive loco1;
Locomotive loco2;

//Engines itineraries
#define NB_CONTACTS 7
int it1[] = {6, 11, 10, 13, 14, 19, 3};
int it2[] = {6, 11, 10, 13, 14, 19, 3};

//Emergency stop for the engines
void emergency_stop()
{
    afficher_message("\nEMERGENCY STOP!!");

    //Stop the engines
    arreter_loco(loco1.no);
    arreter_loco(loco2.no);
}

//Main function
void cmain()
{
    //Itinary counter
    int ct;

    //Engine 1 settings
    loco1.no = 1;
    loco1.vitesse = 12;

    //Engine 2 settings
    loco2.no = 2;
    loco2.vitesse = 12;

    //Initializes the maquet
    selection_maquette("MAQUET_B");
    afficher_message("Et c'est parti!!");

    //Initializes engine 1 initial position
    assigner_loco(11, 6, loco1.no, loco1.vitesse);
    afficher_message_loco(loco1.no,"Hello, je suis la loco 1");

    //Initializes engine 2 initial position
    assigner_loco(4, 9, loco2.no, loco2.vitesse);
    afficher_message_loco(loco2.no,"Hello, je suis la loco 2");

    //Initializes the points
    diriger_aiguillage(7,TOUT_DROIT,0);
    diriger_aiguillage(8,DEVIE,0);
    diriger_aiguillage(5,TOUT_DROIT,0);
    diriger_aiguillage(9,TOUT_DROIT,0);
    diriger_aiguillage(10,TOUT_DROIT,0);
    diriger_aiguillage(14,TOUT_DROIT,0);
    diriger_aiguillage(13,TOUT_DROIT,0);
    diriger_aiguillage(1,TOUT_DROIT,0);

    //Start the engines
    mettre_vitesse_progressive(loco1.no, loco1.vitesse);
    mettre_vitesse_progressive(loco2.no, loco2.vitesse);

  // Demarre la loco
  mettre_vitesse_progressive(loco1.no, loco1.vitesse);

    //Stop the engines
    arreter_loco(loco1.no);
    arreter_loco(loco2.no);

    //Shutdown the maquet
    mettre_maquette_hors_service();
}
