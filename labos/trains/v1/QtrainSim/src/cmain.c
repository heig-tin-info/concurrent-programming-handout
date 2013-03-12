#include <pthread.h>
#include "ctrain_handler.h"
#include <errno.h>
#include <stdio.h>

// structure qui definit une locomotive
typedef struct
{
  int no;
  int vitesse;
} Locomotive;


// Declaration des deux locomotives
Locomotive loco1;


void emergency_stop()
{
  printf("\nSTOP!");

  // on arrete les locomotives.
  arreter_loco(loco1.no);
}


// Contacts a parcourir
#define NB_CTS 7
int parcours[] = {6, 11, 10, 13, 14, 19, 3};


void cmain()
{
  int ct;

  loco1.no =1;
  loco1.vitesse = 12;

 // init_maquette();
  selection_maquette("MAQUET_B");


  afficher_message("Et c'est parti!!");

  // Demande au simulateur de placer une loco entre les contacts 6 et 11
  // Recupere le numero et la vitesse saisis par l'utilisateur.
  assigner_loco(  11,
                  6,
                  loco1.no,
                  loco1.vitesse);

  afficher_message_loco(loco1.no,"Hello, je suis la loco 1");
  // Dirige les aiguillages sur le parcours
  diriger_aiguillage(7,TOUT_DROIT,0);
  diriger_aiguillage(8,DEVIE,0);
  diriger_aiguillage(5,TOUT_DROIT,0);
  diriger_aiguillage(9,DEVIE,0);
  diriger_aiguillage(10,TOUT_DROIT,0);
  diriger_aiguillage(14,TOUT_DROIT,0);
  diriger_aiguillage(13,DEVIE,0);
  diriger_aiguillage(1,TOUT_DROIT,0);


  const char* tmp = getCommand();
  afficher_message(tmp);

  // Demarre la loco
  mettre_vitesse_progressive(loco1.no, loco1.vitesse);

  // Attend que la loco passe sur les differents contacts de son parcours.
  for (ct = 1; ct < NB_CTS; ct++) {
    attendre_contact(parcours[ct]);
    printf("Loco %d de vitesse %d a atteint le contact %d.\n", loco1.no, loco1.vitesse, ct);
  }

  // Stoppe la loco
  arreter_loco(loco1.no);

  // Fin de la simulation (a effectuer une seule fois en fin de programme, sans effet
  // sur le simulateur, mais necessaire sur les maquettes reelles).
  mettre_maquette_hors_service();
}
