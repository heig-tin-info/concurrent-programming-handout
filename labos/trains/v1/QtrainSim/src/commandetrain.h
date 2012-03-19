#ifndef COMMANDETRAIN_H
#define COMMANDETRAIN_H

#include <QObject>
#include <QString>
#include <general.h>

/**
  Toutes les methodes de cette classe doivent �tre reentrantes!!!!!!!
  */
class CommandeTrain : public QObject
{
    Q_OBJECT
protected:
    /**
      Constructeur vide.
      */
    CommandeTrain();

    /**
      Unique instance de la commande de train.
      */
    static CommandeTrain *m_instance;
public:

    /**
     * Retourne l'unique instance de la commande de train.
     * Si le singleton n'a pas encore ete cree il l'est de maniere automatique.
     */
    static CommandeTrain *getInstance();



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
     * Realimente la maquette. Inutile apres init_maquette
     */
    void mettre_maquette_en_service(void);

    /**
     * Ajoute une loco dans la simulation.
     * Remarque : n'a pas d'effet sur la maquette reelle.
     * Remarque bis : Cette m�thode est appel�e automatiquement lors de l'appel de
     *                assigner_loco(...);
     * \param no_loco Num�ro de la locomotive.
     */
    void ajouter_loco(int no_loco);

    /**
     * Change la direction d'un aiguillage.
     * \param no_aiguillage  No de l'aiguillage a diriger.
     * \param direction      Nouvelle direction. (DEVIE ou TOUT_DROIT)
     * \param temps_alim     Temps l'alimentation minimal du bobinage de l'aiguillage.
     * Remarque : temps_alim n'a pas d'effet dans le simulateur.
     */
    void diriger_aiguillage(int no_aiguillage, int direction, int temps_alim);

    /**
     * M�thode bloquante, permettant d'attendre l'activation du contact voulu.
     * Remarque : le contact peut �tre activ� par n'importe quelle locomotive.
     * \param no_contact  Num�ro du contact dont on attend l'activation.
     */
    void attendre_contact(int no_contact);

    /**
     * Arrete une locomotive (met sa vitesse �  VITESSE_NULLE).
     * \param no_loco  Num�ro de la loco �  stopper.
     */
    void arreter_loco(int no_loco);

    /**
     * Change la vitesse d'une loco par palier.
     * \param no_loco         No de la loco a stopper.
     * \param vitesse_future  Vitesse apres changement.
     *
     * Remarque : Dans le simulateur cette procedure agit comme la
     *            fonction "Mettre_Vitesse_Loco". Le comportement
     *            d�pend de l'option "Inertie" dans le menu ad hoc.
     */
    void mettre_vitesse_progressive(int no_loco, int vitesse_future);

    /**
     * Permettre d'allumer ou d'eteindre les phares de la locomotive.
     * \param no_loco  No de la loco a controler.
     * \param etat     Nouvel etat des phares. (ETEINT ou ALLUME)
     * Remarque : dans le simulateur, les phares sont toujours allum�s.
     *            Cette m�thode n'a pas d'effet.
     */
    void mettre_fonction_loco(int no_loco, char etat);

    /**
     * Inverse le sens d'une locomotive, en conservant ou retrouvant sa vitesse originale.
     * \param no_loco  Num�ro de la loco � inverser.
     * Remarque : Le comportement de la loco d�pend du param�tre "Inertie"
     *            dans le menu ad hoc.
     */
    void inverser_sens_loco(int no_loco);

    /**
     * Change la vitesse d'une loco.
     * \param no_loco  No de la loco � contr�ler.
     * \param vitesse  Nouvelle vitesse.
     */
    void mettre_vitesse_loco(int no_loco, int vitesse);

    /**
     * Indique au simulateur de demander une loco � l'utilisateur. L'utilisateur
     * entre le numero et la vitesse de la loco. Celle-ci est ensuite placee entre
     * les contacts "contact_a" et "contact_b".
     * \param contact_a    Contact vers lequel la loco va se diriger.
     * \param contact_b    Contact � l'arriere de la loco.
     * \param numero_loco  Numero de loco choisi par l'utilisateur.
     * \param vitesse      Vitesse choisie par l'utilisateur.
     * Remarque : Cette m�thode n'est pas impl�ment�e dans le simulateur.
     *            Veuillez utiliser la m�thode assigner_loco(...);
     */
    void demander_loco(int contact_a, int contact_b, int *no_loco, int *vitesse);


    /**
     * Indique au simulateur que la loco d'identifiant no_loco dont �tre utilis�e.
     * La m�thode indique �galement entre quels contacts elle se trouve, ainsi que sa vitesse.
     * \param contact_a  Identifiant du contact vers lequel la loco va se diriger.
     * \param contact_b  Identifiant du contact � l'arri�re de la loco.
     * \param no_loco    Num�ro de la loco choisie.
     * \param vitesse    Vitesse � laquelle la loco devra se d�placer.
     */
    void assigner_loco(int contact_a,int contact_b,int no_loco,int vitesse);

    /**
      * S�lectionne la maquette �  utiliser.
      * Cette fonction termine l'application si la maquette n'est pas trouv�e.
      * \param maquette Nom de la maquette.
      */
    void selection_maquette(QString maquette);

    void afficher_message(const char *message);

    void afficher_message_loco(int numLoco,const char *message);

protected slots:
    void timerTrigger();

signals:
    void addLoco(int no_loco);
    void setLoco(int contactA, int contactB, int numLoco, int vitesseLoco);
    void askLoco(int contactA, int contactB);
    void setVitesseLoco(int numLoco, int vitesseLoco);
    void reverseLoco(int numLoco);
    void setVitesseProgressiveLoco(int numLoco, int vitesseLoco);
    void stopLoco(int numLoco);
    void setVoieVariable(int numVoieVariable, int direction);
    void selectMaquette(QString maquette);
    void afficheMessage(QString message);
    void afficheMessageLoco(int numLoco,QString message);
};

#endif // COMMANDETRAIN_H
