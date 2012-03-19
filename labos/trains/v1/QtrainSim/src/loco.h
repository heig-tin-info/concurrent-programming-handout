#ifndef LOCO_H
#define LOCO_H

#include <QObject>
#include <QAbstractGraphicsShapeItem>
#include <QStaticText>
#include <QPainter>
#include <general.h>
#include <voie.h>
#include <segment.h>
#include <connect.h>
#include <QTimer>

class panneauNumLoco : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT
public:
    /** Constructeur de classe. Sert � afficher le num�ro des locos.
      * \param numLoco, le num�ro de la loco.
      */
    explicit panneauNumLoco(int numLoco, QObject *parent = 0);

    /** Retourne le rectangle englobant le panneau.
      * \return le rectangle englobant le panneau.
      */
    QRectF boundingRect() const;

    /** m�thode d'affichage.
      *
      */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /** retourne le num�ro de la loco.
      * \return le num�ro de la loco.
      */
    int getNumLoco();
private:
    int numLoco;
};

class LocoCtrl;

class Loco : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT
public:

    /** Constructeur de classe.
      * \param numLoco le num�ro de la loco.
      */
    explicit Loco(int numLoco, QObject *parent = 0);

    /** Permet de changer la vitesse de la loco.
      * Le comportement d�pend de l'option "Inertie" :
      * avec l'inertie, le changement sera progressif.
      * sans l'inertie, le changement sera imm�diat.
      * \param v la nouvelle vitesse de la loco.
      */
    void setVitesse(int v);

    /** Retourne la vitesse actuelle de la loco.
      * \return la vitesse actuelle de la loco.
      */
    int getVitesse();

    /** permet de changer la direction de la loco.
      * N'est pas utilis� : pour changer de sens, on effectue une rotation de 180�.
      * \param d la nouvelle direction (DIRECTION_LOCO_GAUCHE ou DIRECTION_LOCO_DROITE)
      */
    void setDirection(int d);

    /** retourne la direction actuelle de la loco
      * \return la direction actuelle de la loco
      */
    int getDirection();

    /** permet d'assigner la couleur principale de la loco.
      * \param r la valeur du rouge (0-255)
      * \param g la valeur du vert (0-255)
      * \param b la valeur du bleu (0-255)
      */
    void setCouleur(int r, int g, int b);

    /** retourne la couleur principale de la loco
      * \return la couleur principale de la loco
      */
    QColor* getCouleur();

    /** Retourne le rectangle englobant la loco.
      * \return le rectangle englobant la loco.
      */
    QRectF boundingRect() const;

    /** M�thode d'affichage.
      *
      */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /** permet d'indiquer la voie sur laquelle la loco est pos�e.
      * \param v la voie actuelle.
      */
    void setVoie(Voie* v);

    /** retourne la voie sur laquelle est la loco.
      * \return la voie sur laquelle est la loco.
      */
    Voie* getVoie();

    /** permet d'indiquer la voie vers laquelle la loco est dirig�e.
      * \param v la voie vers laquelle la loco est dirig�e.
      */
    void setVoieSuivante(Voie* v);

    /** retourne la voie vers laquelle la loco est dirig�e.
      * \return la voie vers laquelle la loco est dirig�e.
      */
    Voie* getVoieSuivante();

    /** permet d'activer ou desactiver une loco. Une loco non active ne sera pas affich�e.
      * \param active la valeur bool�enne indiquant l'activation ou la desactivation de la loco.
      */
    void setActive(bool active);

    /** retourne la valeur bool�enne indiquant l'activation ou la desactivation de la loco.
      * \return la valeur bool�enne indiquant l'activation ou la desactivation de la loco.
      */
    bool getActive();

    /** effectue la transition d'une voie � l'autre et repositionne la loco (corrige les impr�cisions de calcul).
      *
      */
    void avanceDUneVoie();

    /** Fait avancer la loco d'une certaine distance.
      * \param distance la distance de laquelle il faut faire avancer la loco.
      */
    void avancer(qreal distance);

    /** avance la loco le long d'une droite, d'une distance determinee.
      * \param distance la distance de laquelle il faut faire avancer la loco.
      */
    void avancerDroit(qreal distance);

    /** avance la loco selon une courbe, d�finie par un angle et un rayon.
      * \param angle l'angle de rotation
      * \param rayon le rayon de rotation
      */
    void avancerCourbe(qreal angle, qreal rayon);

    /** permet de mettre � jour l'angle cumule
      * \param a la nouvelle valeur de l'angle cumule
      */
    void setAngleCumule(qreal a);

    /** retourne l'angle cumule
      * \return l'angle cumule
      */
    qreal getAngleCumule();

    /** permet d'indiquer sur quel segment la loco se trouve
      * \param s le segment sur lequel la loco se trouve
      */
    void setSegmentActuel(Segment* s);

    /** permet de changer la valeur bool�enne d'alerte de proximit�.
      * \param b la nouvelle valeur bool�enne d'alerte de proximit�.
      */
    void setAlerteProximite(bool b);

    /** retourne la valeur bool�enne d'alerte de proximit�.
      * \return la valeur bool�enne d'alerte de proximit�.
      */
    bool getAlerteProximite();

    /** retourne le contour de la loco en coordonn�es de la scene.
      * \return le contour de la loco en coordonn�es de la scene.
      */
    QPolygonF getContour();

    /** Inverse le sens de la loco en conservant ou retrouvant la vitesse initiale.
      * Le comportement d�pend de l'option "Inertie" :
      * avec l'inertie, le changement sera progressif.
      * sans l'inertie, le changement sera imm�diat.
      */
    void inverserSens();

    /** Permet de corriger l'angle de la loco suite � des impr�cisions de calcul.
      * \param nouvelAngle le nouvel angle.
      */
    void corrigerAngle(qreal nouvelAngle);

    LocoCtrl *controller;
signals:

    /** signale que la loco a atteint un nouveau segment
      * \param ctc1 le premier contact du segment
      * \param ctc2 le deuxieme contact du segment
      * \param l la loco emettrice du signal.
      */
    void nouveauSegment(Contact* ctc1, Contact* ctc2, Loco* l);

    /** Signale un d�raillement.
      * \param l la loco emettrice du signal.
      */
    void deraillement(Loco* l);
public slots:

    /** Re�oit l'indication qu'une loco est sur le segment s.
      * \param s le segment signal� comme occup�.
      */
    void locoSurSegment(Segment* s);

    /** Re�oit l'indication qu'une voie variable a �t� modifi�e.
      * \param v la voie variable modifi�e.
      */
    void voieVariableModifiee(Voie* v);

    /** Re�oit le signal qu'il faut adapter la vitesse d'un incr�ment / d�cr�ment.
      */
    void adapterVitesse();
private:
    panneauNumLoco* numLoco1;
    panneauNumLoco* numLoco2;
    qreal angleCumule;
    bool active;
    int vitesse;
    int vitesseFuture;
    int direction;
    QColor* couleur;
    Voie* voieActuelle;
    Voie* voieSuivante;
    Segment* segmentActuel;
    bool alerteProximite;
    bool inverser;
    bool deraille;
    QTimer* timer;
    QWaitCondition* VarCond;
    QMutex* mutex;
};

#endif // LOCO_H
