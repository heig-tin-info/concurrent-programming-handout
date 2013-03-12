#ifndef SIMVIEW_H
#define SIMVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <connect.h>
#include <voie.h>
#include <voievariable.h>
#include <loco.h>
#include <segment.h>


class ExplosionItem :  public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
public:
    ExplosionItem(){};

};

class SimView : public QGraphicsView
{
    Q_OBJECT
public:
    /** Constructeur de classe
      *
      */
    explicit SimView(QWidget *);

    /** Permet d'ajouter une voie � la simulation.
      * \param v la voie � ajouter
      * \param ID le num�ro de la voie
      */
    void addVoie(Voie* v, int ID);

    /** Permet d'ajouter une voie variable � la liste idoine de la simulation.
      * \param vv la voie variable � ajouter
      * \param ID le num�ro de la voie variable
      * Remarque : une voie variable a donc deux num�ros :
      *            - son num�ro de voie
      *            - son num�ro de voie variable
      */
    void addVoieVariable(VoieVariable* vv, int ID);

    /** Permet d'ajouter une contact � la simulation.
      * \param c le contact � ajouter
      * \param ID le num�ro du contact
      */
    void addContact(Contact* c, int ID);

    /** Permet d'indiquer la premi�re voie � poser, par rapport � laquelle
      * toutes les autres voies vont se positionner.
      * \param v le voie a poser en premier.
      */
    void setPremiereVoie(Voie* v);

    /** Permet de modifier une voie variable.
      * \param n le num�ro de la voie variable.
      * \param v l'etat de la voie variable (DEVIE ou TOUT_DROIT)
      */
    void modifierAiguillage(int n, int v);

    /** Lance la construction de la maquette (placement des voies, etc...)
      */
    void construireMaquette();

    /** supprime toutes les voies, contacts, etc... en vue d'un nouveau chargement.
      */
    void viderMaquette();

    /** G�n�re la liste des segments de la maquette.
      *
      */
    void genererSegments();

    /** Ajoute une locomotive.
      * \param l la loco � ajouter.
      * \param ID le num�ro de la loco.
      */
    void addLoco(Loco* l, int ID);

    /** Attribue des couleurs � chaque locomotive pr�sente, de telle mani�re qu'il soit
      * le plus facile de les distinguer.
      * Remarque : la premi�re loco est toujours rouge.
      */
    void peintLocos();

    /** permet de zoomer dans le vue.
      *
      */
    void zoomIn();

    /** permet de d�zoomer de la vue.
      *
      */
    void zoomOut();

    /** ajuste le zoom de telle mani�re que la maquette soit enti�rement affich�e.
      *
      */
    void zoomFit();

    /** retourne le contact ayant le num�ro n.
      * \param n le num�ro du contact
      * \return le contact correspondant.
      */
    Contact* getContact(int n);

    /** raffraichit l'affichage.
      *
      */
    void redraw();
signals:

    /** Signale qu'une loco a chang� de segment, et se trouve que le segment s.
      * \param s, le segment occup�.
      */
    void locoSurSegment(Segment* s);

    /** Signale le changment d'�tat d'une voie variable.
      * \param v la voie variable ayant chang�.
      */
    void notificationVoieVariableModifiee(Voie* v);
public slots:

    /** effectue un nouveau pas d'animation.
      *
      */
    void animationStep();

    /** d�marre l'animation
      *
      */
    void animationStart();

    /** stoppe l'animation
      *
      */
    void animationStop();

    /** pr�pare la locomotive au d�part.
      * \param contactA le premier contact d�finissant le segment sur lequel se trouve la loco.
      * \param contactB le second contact d�finissant le segment sur lequel se trouve la loco.
      * \param numLoco le num�ro de la loco � placer.
      * \param vitesseLoco la vitesse de la loco.
      */
    void setLoco(int contactA, int contactB, int numLoco, int vitesseLoco);

    /** pas impl�ment�.
      *
      */
    void askLoco(int, int);

    /** permet de changer la vitesse d'une loco.
      * \param numLoco le num�ro de la loco � changer
      * \param vitesseLoco la nouvelle vitesse de la loco.
      */
    void setVitesseLoco(int numLoco, int vitesseLoco);

    /** Inverse le sens de la loco.
      * \param numLoco le num�ro de la loco � inverser.
      */
    void reverseLoco(int numLoco);

    /** permet de changer la vitesse d'une loco.
      * \param numLoco le num�ro de la loco � changer
      * \param vitesseLoco la nouvelle vitesse de la loco.
      */
    void setVitesseProgressiveLoco(int numLoco, int vitesseLoco);

    /** arrete la loco
      * \param numLoco le num�ro de la loco.
      */
    void stopLoco(int numLoco);

    /** modifie l'etat d'une voie variable.
      * \param numVoieVariable le num�ro de la voie variable.
      * \param direction la nouvelle direction de la voie (DEVIE ou TOUT_DROIT)
      */
    void setVoieVariable(int numVoieVariable, int direction);

    /** re�oit l'information qu'une loco a chang� de segment.
      * \param ctc1 et ctc2 d�finissent le segment.
      * \param l la loco ayant chang� de segment.
      */
    void locoSurNouveauSegment(Contact* ctc1, Contact* ctc2, Loco* l);

    /** re�oit l'information qu'une voie variable a �t� modifi�e.
      * \param v la voie variable modifi�e.
      */
    void voieVariableModifiee(Voie* v);

private:
    QTimer* timer;
    QGraphicsScene * scene;
    QMap<int, Voie*> Voies;
    QMap<int, VoieVariable*> VoiesVariables;
    QMap<int, Contact*> contacts;
    Voie* premiereVoie;
    QMap<int, Loco*> Locos;
    QList<Segment*> segments;

    /** retourne le segment correspondant � la paire de contacts pass�e en param�tre
      * \param contactA et contactB les contacts d�finissant les segment.
      * \return le segment correspondant.
      */
    Segment* getSegmentByContacts(int contactA, int contactB);
};

#endif // SIMVIEW_H
