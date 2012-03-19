/**  \file ctrain_handler.h
 *
 * Fournit les fonctions de controle du simulateur/maquette
 * de trains. Mappage en C des fonctions fournies par
 * "train_handler.ads".
 *
 * \author Kevin Georgy
 * \date 9.2.2009
 */

#ifndef H_CTRAIN_HANDLER

#define H_CTRAIN_HANDLER

/** Vitesse à l'arret */
#define	VITESSE_NULLE (0)

/** Vitesse minimum */
#define	VITESSE_MINIMUM (3)

/** Vitesse maximum */
#define	VITESSE_MAXIMUM (14)

/** Numero max. d'aiguillage */
#define	MIN_AIGUILLAGES (1)

/** Numero max. d'aiguillage */
#define	MAX_AIGUILLAGES (80)

/** Numero max. de contact */
#define MAX_CONTACTS (64)

/** Numero min. de loco */
#define	MIN_LOCOS (1)

/** Numero max. de loco */
#define	MAX_LOCOS (80)

/** Direction des aiguillages DEVIE*/
#define DEVIE (0)

/** Direction des aiguillages TOUT_DROIT*/
#define TOUT_DROIT (1)

/** Etat des phares eteints*/
#define ETEINT (0)

/** Etat des phares allumes*/
#define ALLUME (1)

/**
 * Initialise la communication avec la maquette/simulateur.
 * A appeler en debut de programme client.
 */
void init_maquette(void);


/**
 * Met fin a la simulation. A appeler en fin de programme client
 */
void mettre_maquette_hors_service(void);

/**
 * Realimente la maquette. Inutil apres init_maquette
 */
void mettre_maquette_en_service(void);

/**
 * Change la direction d'un aiguillage.
 * \param[in] no_aiguillage No de l'aiguillage a diriger.
 * \param[in] direction Nouvelle direction. (DEVIE ou TOUT_DROIT)
 * \param[in] temps_alim Temps l'alimentation minimal du bobinage de l'aiguillage.
 */
void diriger_aiguillage(int no_aiguillage, int direction, int temps_alim);

/**
 * Attend l'activation du contact donne.
 * \param[in] no_contact No du contact dont on attend l'activation.
 */
void attendre_contact(int no_contact);

/**
 * Attend l'activation d'un contact.
 */
int attendre_contact_x(void);

/**
 * Arrete une locomotive (met sa vitesse a VITESSE_NULLE).
 * \param[in] no_loco No de la loco a stopper.
 */
void arreter_loco(int no_loco);

/**
 * Change la vitesse d'une loco par palier.
 * \param[in] no_loco No de la loco a stopper.
 * \param[in] vitesse_future Vitesse apres changement.
 *
 * \attention Dans le simulateur cette procedure agit comme la
 * fonction "Mettre_Vitesse_Loco". c'est-a-dire que
 * l'acceleration est immediate( de la vitesse actuelle
 * a la vitesse specifiee )
 */
void mettre_vitesse_progressive(int no_loco, int vitesse_future);

/**
 * Permettre d'allumer ou d'eteindre les phares de la locomotive.
 * \param[in] no_loco No de la loco a controler.
 * \param[in] etat Nouvel etat des phares. (ETEINT ou ALLUME)
 *
 * \attention Dans le simulateur cette fonction n'a aucun effet.
 * Les locomotive representee par des rectangles
 * possedent une partie jaune indiquant le sens de
 * deplacement. L'utilisation des phares n'est donc
 * plus utile.
 */
void mettre_fonction_loco(int no_loco, char etat);

/**
 * Inverse le sens d'une locomotive, en conservant sa vitesse.
 * \param[in] no_loco No de la loco a inverser.
 */
void inverser_sens_loco(int no_loco);

/**
 * Change la vitesse d'une loco.
 * \param[in] no_loco No de la loco a controler.
 * \param[in] vitesse Nouvelle vitesse.
 */
void mettre_vitesse_loco(int no_loco, int vitesse);

/**
 * Indique au simulateur de demander une loco à l'utilisateur. L'utilisateur
 * entre le numero et la vitesse de la loco. Celle-ci est ensuite placee entre
 * les contacts "contact_a" et "contact_b".
 * \param[in] contact_a Contact vers lequel la loco va se diriger.
 * \param[in] contact_b Contact à l'arriere de la loco.
 * \param[out] no_loco Numero de loco choisi par l'utilisateur.
 * \param[out] vitesse Vitesse choisie par l'utilisateur.
 */
void demander_loco(int contact_a, int contact_b, int *no_loco, int *vitesse);

#endif

