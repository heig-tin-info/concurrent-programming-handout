#include <pthread.h>
#include "ctrain_handler.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define VITESSE_FAIBLE 8;


// structure qui definit une locomotive
typedef struct
{
  int no;
  int vitesse;
} Locomotive;


// Declaration des deux locomotives
Locomotive loco1;
Locomotive loco2;

// Derniere position des locos
int position1;
int position2;

// Temps d'arret en gare
const unsigned int ARRET = 5;

// nombre de tours a faire
const int maxTours = 1;

// Verrous
// Initialisation
pthread_mutex_t init = PTHREAD_MUTEX_INITIALIZER;
// Attentes pour voies partagees
pthread_mutex_t attente1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t attente2 = PTHREAD_MUTEX_INITIALIZER;
// Attente pour une section de voie devant etre utilisee par l'autre loco d'abord
pthread_mutex_t attenteAutre = PTHREAD_MUTEX_INITIALIZER;
// Attente pour la gare intermediaire
pthread_mutex_t attenteGare = PTHREAD_MUTEX_INITIALIZER;
// Attentes en gare pour la correspondance
pthread_mutex_t gare1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t gare2 = PTHREAD_MUTEX_INITIALIZER;
// Section critique pour depart dans le sens inverse
pthread_mutex_t departInverse = PTHREAD_MUTEX_INITIALIZER;

// Declaration des threads (necessaire pour l'arret d'urgence)
pthread_t thread1;
pthread_t thread2;


void emergency_stop()
{
  afficher_message("\nSTOP!\nArret d'urgence");

  pthread_cancel(thread1);
  pthread_cancel(thread2);

  // on arrete les locomotives.
  arreter_loco(loco1.no);
  arreter_loco(loco2.no);
  mettre_fonction_loco(loco1.no, ETEINT);
  mettre_fonction_loco(loco2.no, ETEINT);
}


void *Loco1(void * arg)
{
    // Parcours d'initialisation
    const int kilometrageInit = 6;
    const int parcoursInit[] = {31, 1, 6, 8, 9, 35};
    // Parcours a repeter
    const int kilometrage = 24;
    const int parcours[] = {35, 36, 33, 31,  1,  2,  3,  4, 10, 11, 12, 13, 19,
                            20, 21, 22, 28, 33, 34,  5,  6,  8,  9, 35};

    // detremine si un arret imprevu a eu lieu
    int arretImprevu;
    // compteur utilise pour compter le nombre de capteurs non-surveilles apres
    // un arret imprevu
    int special;

    // utilise pour iterer les tableaux par boucles for
    int i;

    // Compte le nombre de tours
    int tours = 0;

    //determine le sens de rotation : 1 -> normal, 0 -> inverse
    int normal = 1;

    char message[80];

    // Autorisation de l'arret du thread
    int ancien_etat, ancien_type;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &ancien_etat);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &ancien_type);

    assigner_loco(parcoursInit[1],
                  parcoursInit[0],
                  loco1.no,
                  loco1.vitesse);

    diriger_aiguillage(1, DEVIE, 0);
    diriger_aiguillage(2, TOUT_DROIT, 0);
    diriger_aiguillage(3, DEVIE, 0);
    diriger_aiguillage(5, DEVIE, 0);
    diriger_aiguillage(6, TOUT_DROIT, 0);
    diriger_aiguillage(24, TOUT_DROIT, 0);

    loco1.vitesse = VITESSE_MAXIMUM;

    for(i = 1; i < kilometrageInit; i++) {
        mettre_vitesse_progressive(loco1.no, loco1.vitesse);
        attendre_contact(parcoursInit[i]);
        sprintf(message, "Loco %d de vitesse %d, a atteint le contact %d.\n",
                loco1.no, loco1.vitesse, parcoursInit[i]);
        afficher_message_loco(loco1.no, message);
        if(i == kilometrageInit - 2)
            loco1.vitesse = VITESSE_MINIMUM;
    }

    arreter_loco(loco1.no);
    pthread_mutex_unlock(&init);

    sprintf(message, "Loco %d s'est arretee.\n", loco1.no);
    afficher_message_loco(loco1.no, message);

    //sleep(ARRET);

    while(1)
    {
        // Initialisation des variables necessaires a l'arret imprevu
        arretImprevu = 0;
        special = 0;

        // Attente de l'autre locomotive
        pthread_mutex_unlock(&gare1);
        pthread_mutex_lock(&gare2);

        // Sens normal
        if(normal)
        {
            diriger_aiguillage(23, DEVIE, 0);
            diriger_aiguillage(20, DEVIE, 0);
            diriger_aiguillage(21, TOUT_DROIT, 0);
            diriger_aiguillage(22, DEVIE, 0);
            diriger_aiguillage(1, TOUT_DROIT, 0);

            loco1.vitesse = VITESSE_MAXIMUM;

            for(i = 1; i < kilometrage; i++)
            {
                mettre_vitesse_progressive(loco1.no, loco1.vitesse);
                attendre_contact(parcours[i]);
                position1 = parcours[i];
                sprintf(message, "Loco %d de vitesse %d, a atteint le contact "
                        "%d.\n", loco1.no, loco1.vitesse, parcours[i]);
                afficher_message_loco(loco1.no, message);

                // Ralentissement preventif en cas d'arret au croisement 1
                if(i == 4)
                {
                    loco1.vitesse = VITESSE_FAIBLE;
                }
                // Croisement 1
                else if(i == 5)
                {
                    pthread_mutex_unlock(&attente1);
                    if(pthread_mutex_trylock(&attente2))
                    {
                        arreter_loco(loco1.no);
                        pthread_mutex_lock(&attente2);
                    }
                    diriger_aiguillage(4, TOUT_DROIT, 0);
                    diriger_aiguillage(7, TOUT_DROIT, 0);
                    diriger_aiguillage(10, TOUT_DROIT, 0);
                    diriger_aiguillage(13, TOUT_DROIT, 0);
                    diriger_aiguillage(16, TOUT_DROIT, 0);
                    diriger_aiguillage(19, DEVIE, 0);

                    loco1.vitesse = VITESSE_MAXIMUM;
                }
                // Preparation a l'arret a la gare (capteur 19)
                else if(i == 11)
                {
                    loco1.vitesse = VITESSE_MINIMUM;
                }
                // Arret a la gare (capteur 19)
                else if(i == 12)
                {
                    arreter_loco(loco1.no);
                    sprintf(message, "Loco %d s'est arretee.\n", loco1.no);
                    afficher_message_loco(loco1.no, message);
                    //sleep(ARRET);

                    loco1.vitesse = VITESSE_MAXIMUM;
                }
                // Autorise le train derriere a arriver en gare
                else if(i == 13)
                {
                    pthread_mutex_unlock(&attenteGare);
                }
                // Ralentissement preventif en cas d'arret au croisement 2
                else if(i == 14)
                {
                    loco1.vitesse = VITESSE_FAIBLE;
                }
                // Croisement 2
                else if(i == 16)
                {
                    if(pthread_mutex_trylock(&attenteAutre))
                    {
                        arreter_loco(loco1.no);
                        pthread_mutex_lock(&attenteAutre);
                    }

                    diriger_aiguillage(20, TOUT_DROIT, 0);
                    diriger_aiguillage(21, DEVIE, 0);
                    diriger_aiguillage(2, DEVIE, 0);
                    diriger_aiguillage(3, DEVIE, 0);
                    diriger_aiguillage(5, DEVIE, 0);
                    diriger_aiguillage(6, TOUT_DROIT, 0);
                    diriger_aiguillage(24, TOUT_DROIT, 0);

                    loco1.vitesse = VITESSE_MAXIMUM;
                }
                // Ralentissement en prevision de l'arret de fin de tour
                else if(i == kilometrage - 2)
                {
                    loco1.vitesse = VITESSE_MINIMUM;
                }
            }
            arreter_loco(loco1.no);
            sprintf(message, "Loco %d s'est arretee apres %d tours.\n",
                    loco1.no, ++tours);
            afficher_message_loco(loco1.no, message);
            //sleep(ARRET);
        }

        // Sens inverse
        else
        {
            pthread_mutex_lock(&departInverse);

            diriger_aiguillage(24, TOUT_DROIT, 0);
            diriger_aiguillage(6, TOUT_DROIT, 0);
            diriger_aiguillage(5, DEVIE, 0);
            diriger_aiguillage(3, DEVIE, 0);
            diriger_aiguillage(2, DEVIE, 0);
            diriger_aiguillage(21, DEVIE, 0);
            diriger_aiguillage(20, TOUT_DROIT, 0);
            diriger_aiguillage(19, DEVIE, 0);
            diriger_aiguillage(16, TOUT_DROIT, 0);
            diriger_aiguillage(13, TOUT_DROIT, 0);

            loco1.vitesse = VITESSE_MAXIMUM;

            for(i = kilometrage - 2; i >= 0; i--)
            {
                mettre_vitesse_progressive(loco1.no, loco1.vitesse);

                // Si la loco s'est arretee de maniere imprevue, on ne controle
                // pas le passage sur les deux contacts suivants
                if (arretImprevu)
                {
                    special++;
                    if (special == 2)
                    {
                        special = 0;
                        arretImprevu = 0;
                    }
                }
                // Sinon on controle le passage sur tous les capteurs
                else
                {
                    attendre_contact(parcours[i]);
                }

                position1 = parcours[i];
                sprintf(message, "Loco %d de vitesse %d, a atteint le contact "
                        "%d.\n", loco1.no, loco1.vitesse, parcours[i]);
                afficher_message_loco(loco1.no, message);

                // Permet le depart de la loco suivante
                if(i == kilometrage - 2)
                {
                    pthread_mutex_unlock(&departInverse);
                }

                // Controle la position de la loco devant et stop si elle est
                // trop pres
                if(i <= kilometrage - 2 && i >= kilometrage - 11)
                {
                    while(position2 == parcours[i - 1] ||
                          position2 == parcours[i - 2])
                    {
                        arreter_loco(loco1.no);
                        arretImprevu = 1;
                        //sleep(1);
                    }

                    // Signale a l'autre loco qu'elle peut passer
                    if(i == kilometrage - 8)
                    {
                        pthread_mutex_unlock(&attenteAutre);
                    }

                    // Ralentissement en prevision de l'arret en gare (capteur 19)
                    if(i == kilometrage - 11)
                    {
                        loco1.vitesse = VITESSE_MINIMUM;
                    }
                }
                else if(i == kilometrage - 12)
                {
                    arreter_loco(loco1.no);
                    sprintf(message, "Loco %d s'est arretee.\n", loco1.no);
                    afficher_message_loco(loco1.no, message);
                    //sleep(ARRET);

                    // Controle que la voie est libre devant
                    while(position2 == parcours[i - 1] ||
                          position2 == parcours[i - 2])
                        //sleep(1);

                    diriger_aiguillage(10, TOUT_DROIT, 0);
                    diriger_aiguillage(7, TOUT_DROIT, 0);
                    diriger_aiguillage(4, TOUT_DROIT, 0);
                    loco1.vitesse = VITESSE_MAXIMUM;
                }
                // Ralentissement en prevision du croisement avec l'autre loco
                else if(i == kilometrage - 17)
                {
                    loco1.vitesse = VITESSE_FAIBLE;
                }
                // Croisement avec l'autre loco
                else if(i == kilometrage - 18)
                {
                    pthread_mutex_unlock(&attente1);
                    if(pthread_mutex_trylock(&attente2))
                    {
                        arreter_loco(loco1.no);
                        pthread_mutex_lock(&attente2);
                    }

                    diriger_aiguillage(1, TOUT_DROIT, 0);
                    diriger_aiguillage(22, DEVIE, 0);
                    diriger_aiguillage(21, TOUT_DROIT, 0);
                    diriger_aiguillage(20, DEVIE, 0);
                    diriger_aiguillage(23, DEVIE, 0);

                    loco1.vitesse = VITESSE_MAXIMUM;
                }
                // Approche de la gare
                else if(i == 1)
                {
                    loco1.vitesse = VITESSE_MINIMUM;
                }
            }
            arreter_loco(loco1.no);
            sprintf(message, "Loco %d s'est arretee apres %d tours.\n",
                    loco1.no, ++tours);
            afficher_message_loco(loco1.no, message);
            //sleep(ARRET);
        }
        // Changement de sens de la loco apres 5 tours
        if(tours == maxTours)
        {
            tours = 0;
            if(normal)
            {
                normal = 0;
            }
            else
            {
                normal = 1;
            }
            inverser_sens_loco(loco1.no);
        }
    }

    return NULL;
}

void *Loco2(void * arg)
{
    const int kilometrageInit = 6;
    /*
     *
     *
     * !!!!!!
     *
     *
     * MODIFIER LE PARCOURS INIT POUR LA MAQUETTE
     *
     *
     * !!!!!
     *
     */
    const int parcoursInit[] = {30, 31, 6, 8, 9, 27};
    const int kilometrage = 31;
    const int parcours[] = {27, 26, 24, 19, 13, 12, 11, 10,  4,  6,  1, 31, 33,
                            36, 35, 17, 18, 15, 13, 19, 20, 21, 22, 28, 33, 34,
                             5,  6,  8,  9, 27};

    // detremine si un arret imprevu a eu lieu
    int arretImprevu;
    // compteur utilise pour compter le nombre de capteurs non-surveilles apres
    // un arret imprevu
    int special;

    // utilise pour iterer les tableaux par boucles for
    int i;

    // Compte le nombre de tours
    int tours = 0;

    //determine le sens de rotation : 1 -> normal, 0 -> inverse
    int normal = 1;

    char message[80];

    // Autorisation de l'arret du thread
    int ancien_etat, ancien_type;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &ancien_etat);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &ancien_type);

    assigner_loco(parcoursInit[1],
                  parcoursInit[0],
                  loco2.no,
                  loco2.vitesse);

    pthread_mutex_lock(&init);

    diriger_aiguillage(1, DEVIE, 0);
    diriger_aiguillage(2, TOUT_DROIT, 0);
    diriger_aiguillage(3, DEVIE, 0);
    diriger_aiguillage(5, DEVIE, 0);
    diriger_aiguillage(6, DEVIE, 0);
    diriger_aiguillage(18, DEVIE, 0);

    loco2.vitesse = VITESSE_MAXIMUM;

    for(i = 1; i < kilometrageInit; i++) {
        mettre_vitesse_progressive(loco2.no, loco2.vitesse);
        attendre_contact(parcoursInit[i]);
        sprintf(message, "Loco %d de vitesse %d, a atteint le contact %d.\n",
                loco2.no, loco2.vitesse, parcoursInit[i]);
        afficher_message_loco(loco2.no, message);
        if(i == kilometrageInit - 2)
        {
            loco2.vitesse = VITESSE_MINIMUM;
        }
    }

    arreter_loco(loco2.no);
    pthread_mutex_unlock(&init);

    sprintf(message, "Loco %d s'est arretee.\n", loco2.no);
    afficher_message_loco(loco2.no, message);

    //sleep(ARRET);

    while(1)
    {
        // Initialisation des variables necessaires a l'arret imprevu
        arretImprevu = 0;
        special = 0;

        // Attente de l'autre locomotive
        pthread_mutex_unlock(&gare2);
        pthread_mutex_lock(&gare1);

        // Sens normal
        if(normal)
        {
            diriger_aiguillage(17, DEVIE, 0);
            diriger_aiguillage(15, DEVIE, 0);
            diriger_aiguillage(14, TOUT_DROIT, 0);
            diriger_aiguillage(13, DEVIE, 0);
            diriger_aiguillage(10, TOUT_DROIT, 0);
            diriger_aiguillage(7, TOUT_DROIT, 0);
            diriger_aiguillage(4, DEVIE, 0);
            diriger_aiguillage(3, TOUT_DROIT, 0);
            diriger_aiguillage(2, TOUT_DROIT, 0);

            loco2.vitesse = VITESSE_MAXIMUM;

            for(i = 1; i < kilometrage; i++) {
                mettre_vitesse_progressive(loco2.no, loco2.vitesse);

                // Si la loco s'est arretee de maniere imprevue, on ne controle
                // pas le passage sur les deux contacts suivants
                if (arretImprevu)
                {
                    special++;
                    if (special == 2)
                    {
                        special = 0;
                        arretImprevu = 0;
                    }
                }
                // Sinon on controle le passage sur tous les capteurs
                else
                {
                    attendre_contact(parcours[i]);
                }

                position2 = parcours[i];
                sprintf(message, "Loco %d de vitesse %d, a atteint le contact "
                        "%d.\n", loco2.no, loco2.vitesse, parcours[i]);
                afficher_message_loco(loco2.no, message);
                // Ralentissement preventif en cas d'arret au croisement 1
                if(i == 8)
                {
                    loco2.vitesse = VITESSE_FAIBLE;
                }
                // Croisement 1
                else if(i == 9)
                {
                    pthread_mutex_unlock(&attente2);
                    if(pthread_mutex_trylock(&attente1))
                    {
                        arreter_loco(loco2.no);
                        pthread_mutex_lock(&attente1);
                    }
                    diriger_aiguillage(1, DEVIE, 0);
                    diriger_aiguillage(22, DEVIE, 0);
                    diriger_aiguillage(21, TOUT_DROIT, 0);
                    diriger_aiguillage(20, DEVIE, 0);
                    diriger_aiguillage(23, DEVIE, 0);
                    diriger_aiguillage(24, DEVIE, 0);
                    diriger_aiguillage(12, DEVIE, 0);
                    diriger_aiguillage(11, DEVIE, 0);
                    diriger_aiguillage(8, DEVIE, 0);
                    diriger_aiguillage(9, TOUT_DROIT, 0);

                    loco2.vitesse = VITESSE_MAXIMUM;
                }

                // Ralentissement en prevision de l'arret car la loco1 est devant
                // Liberation du croisement 2
                else if(i == 16)
                {
                    loco2.vitesse = VITESSE_FAIBLE;
                    pthread_mutex_unlock(&attenteAutre);
                }
                // Arret si la loco1 est devant
                else if(i == 17)
                {
                    if(pthread_mutex_trylock(&attenteGare))
                    {
                        arreter_loco(loco2.no);
                        pthread_mutex_lock(&attenteGare);
                    }

                    diriger_aiguillage(10, DEVIE, 0);
                }
                // Preparation a l'arret a la gare (capteur 19)
                else if(i == 18)
                {
                    loco2.vitesse = VITESSE_MINIMUM;
                }
                /* Pour les deux conditions suivantes, nous ne faisons pas de
                   changement d'aiguillages car la loco precedente passe par le
                   meme chemin et donc les aiguillages sont deja alignes. En
                   revanche, nous controlons en permanace la position du train
                   devant pour nous assurer que nous ne lui rentrons pas dedans
                   en le rattrapant*/
                // Arret a la gare (capteur 19)
                else if(i == 19)
                {
                    arreter_loco(loco2.no);
                    sprintf(message, "Loco %d s'est arretee.\n", loco2.no);
                    afficher_message_loco(loco2.no, message);
                    //sleep(ARRET);

                    /* Dans un cas normal, il ne devrait y avoir aucune attente.
                       Nous avons donc choisi de faire une simple boucle while*/
                    while(position1 == parcours[i + 1] ||
                          position1 == parcours[i + 2])
                        //sleep(1);

                    loco2.vitesse = VITESSE_MAXIMUM;
                }
                // Controle que la voie est libre
                else if(i > 19 && i < kilometrage - 3)
                {
                    sprintf(message, "Position1 = %d, position2 = %d, parcours[i+1] = %d, parcours[i+2] = %d", position1, position2, parcours[i+1], parcours[i+2]);
                    afficher_message(message);
                    while(position1 == parcours[i + 1] ||
                          position1 == parcours[i + 2])
                    {
                        arreter_loco(loco2.no);
                        arretImprevu = 1;
                        //sleep(1);
                    }

                }
                // Controle que la voie est libre (derniere section)
                else if(i == kilometrage - 3)
                {
                    sprintf(message, "Position1 = %d Position2 = %d", position1, position2);
                    afficher_message(message);
                    while(position1 != 35)
                    {
                        arreter_loco(loco2.no);
                        arretImprevu = 1;
                        //sleep(1);
                    }
                    sprintf(message, "Position1 = %d Position2 = %d", position1, position2);
                                        afficher_message(message);
                    diriger_aiguillage(6, DEVIE, 0);
                    diriger_aiguillage(18, DEVIE, 0);
                }
                // Ralentissement en prevision de l'arret de fin de tour
                else if(i == kilometrage - 2)
                {
                    loco2.vitesse = VITESSE_MINIMUM;
                }
            }
            arreter_loco(loco2.no);
            sprintf(message, "Loco %d s'est arretee apres %d tours.\n",
                    loco2.no, ++tours);
            afficher_message_loco(loco2.no, message);
            //sleep(ARRET);
        }

        // Sens inverse
        else
        {
            pthread_mutex_lock(&departInverse);

            diriger_aiguillage(24, TOUT_DROIT, 0);
            diriger_aiguillage(6, TOUT_DROIT, 0);
            diriger_aiguillage(5, DEVIE, 0);
            diriger_aiguillage(3, DEVIE, 0);
            diriger_aiguillage(2, DEVIE, 0);
            diriger_aiguillage(21, DEVIE, 0);
            diriger_aiguillage(20, TOUT_DROIT, 0);
            diriger_aiguillage(19, DEVIE, 0);
            diriger_aiguillage(16, TOUT_DROIT, 0);
            diriger_aiguillage(13, TOUT_DROIT, 0);

            loco2.vitesse = VITESSE_MAXIMUM;

            for(i = kilometrage - 2; i >= 0; i--)
            {
                mettre_vitesse_progressive(loco2.no, loco2.vitesse);

                // Si la loco s'est arretee de maniere imprevue, on ne controle
                // pas le passage sur les deux contacts suivants
                if (arretImprevu)
                {
                    special++;
                    if (special == 2)
                    {
                        special = 0;
                        arretImprevu = 0;
                    }
                }
                // Sinon on controle le passage sur tous les capteurs
                else
                    attendre_contact(parcours[i]);

                position1 = parcours[i];
                sprintf(message, "Loco %d de vitesse %d, a atteint le contact "
                        "%d.\n", loco2.no, loco2.vitesse, parcours[i]);
                afficher_message_loco(loco2.no, message);

                // Permet le depart de la loco suivante
                if(i == kilometrage - 2)
                {
                    pthread_mutex_unlock(&departInverse);
                }

                // Controle la position de la loco devant et stop si elle est
                // trop pres
                if(i <= kilometrage - 2 && i >= kilometrage - 11)
                {
                    while(position1 == parcours[i - 1] ||
                          position1 == parcours[i - 2])
                    {
                        arreter_loco(loco2.no);
                        arretImprevu = 1;
                        //sleep(1);
                    }

                    // Ralentissement en prevision de l'arret en gare (capteur 19)
                    if(i == kilometrage - 11)
                    {
                        loco2.vitesse = VITESSE_MINIMUM;
                    }
                }
                else if(i == kilometrage - 12)
                {
                    arreter_loco(loco2.no);
                    sprintf(message, "Loco %d s'est arretee.\n", loco2.no);
                    afficher_message_loco(loco2.no, message);
                    //sleep(ARRET);

                    // Controle que la voie est libre devant
                    while(position1 == parcours[i - 1] ||
                          position1 == parcours[i - 2])
                        //sleep(1);

                    diriger_aiguillage(9, TOUT_DROIT, 0);
                    diriger_aiguillage(8, DEVIE, 0);
                    diriger_aiguillage(11, DEVIE, 0);
                    diriger_aiguillage(12, DEVIE, 0);
                    diriger_aiguillage(24, DEVIE, 0);
                    diriger_aiguillage(23, DEVIE, 0);

                    loco2.vitesse = VITESSE_MAXIMUM;
                }
                else if(i == kilometrage - 16)
                {
                    loco2.vitesse = VITESSE_FAIBLE;
                }
                else if(i == kilometrage - 17)
                {
                    if(pthread_mutex_trylock(&attenteAutre))
                    {
                        arreter_loco(loco2.no);
                        pthread_mutex_lock(&attenteAutre);
                    }

                    diriger_aiguillage(20, DEVIE, 0);
                    diriger_aiguillage(21, TOUT_DROIT, 0);
                    diriger_aiguillage(22, DEVIE, 0);
                    diriger_aiguillage(1, DEVIE, 0);
                    diriger_aiguillage(2, TOUT_DROIT, 0);
                    diriger_aiguillage(3, TOUT_DROIT, 0);
                }
                else if(i == kilometrage - 21)
                {
                    loco2.vitesse = VITESSE_FAIBLE;
                }
                else if(i == kilometrage - 22)
                {
                    pthread_mutex_unlock(&attente2);
                    if(pthread_mutex_trylock(&attente1))
                    {
                        arreter_loco(loco2.no);
                        pthread_mutex_lock(&attente1);
                    }

                    diriger_aiguillage(4, DEVIE, 0);
                    diriger_aiguillage(7, TOUT_DROIT, 0);
                    diriger_aiguillage(10, TOUT_DROIT, 0);
                    diriger_aiguillage(13, DEVIE, 0);
                    diriger_aiguillage(14, TOUT_DROIT, 0);
                    diriger_aiguillage(15, TOUT_DROIT, 0);
                    diriger_aiguillage(17, TOUT_DROIT, 0);

                    loco2.vitesse = VITESSE_MAXIMUM;
                }
                else if(i == 1)
                {
                    loco2.vitesse = VITESSE_MINIMUM;
                }
            }
            arreter_loco(loco2.no);
            sprintf(message, "Loco %d s'est arretee apres %d tours.\n",
                    loco2.no, ++tours);
            afficher_message_loco(loco2.no, message);
            //sleep(ARRET);
        }

        // Changement de sens de la loco apres 5 tours
        if(tours == maxTours)
        {
            tours = 0;
            if(normal)
            {
                normal = 0;
            }
            else
            {
                normal = 1;
            }
            inverser_sens_loco(loco2.no);
        }
    }

    return NULL;
}

void cmain()
{
    //init_maquette();

    selection_maquette("MAQUET_A");

    // Preparation des verrous
    pthread_mutex_lock(&init);
    pthread_mutex_lock(&attente1);
    pthread_mutex_lock(&attente2);
    pthread_mutex_lock(&attenteAutre);
    pthread_mutex_lock(&attenteGare);
    pthread_mutex_lock(&gare1);
    pthread_mutex_lock(&gare2);

    // Assignation des numeros au locos
    loco1.no = 15;
    loco1.vitesse = 0;
    loco2.no = 9;
    loco2.vitesse = 0;

    mettre_fonction_loco(loco1.no, ALLUME);
    mettre_fonction_loco(loco2.no, ALLUME);

    pthread_create(&thread1, NULL, Loco1, NULL);
    pthread_create(&thread2, NULL, Loco2, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
