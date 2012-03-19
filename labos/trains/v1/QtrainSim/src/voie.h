#ifndef VOIE_H
#define VOIE_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QPointF>
#include <math.h>
#include <QDebug>
#include <QRectF>
#include <QAbstractGraphicsShapeItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QMap>
#include <general.h>
#include <contact.h>

class Voie : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT

public:
    /** Constructeur de classe.
      */
    Voie();

    /** M�thode permettant de calculer la position de la voie, en fonction d'une voie
      * voisine d�j� pos�e. S'il s'agit de la premi�re voie pos�e, on lui attribue une position
      * par d�faut.
      * \param v pointeur sur la voie voisine d�j� pos�e.
      */
    void calculerPosition(Voie* v=0);

    /** m�thode virtuelle visant � calculer les angles et coordonn�es (locales) de chaque extr�mit�
      * de la voie.
      * Cette m�thode est appel�e r�cursivement, jusqu'� ce que toutes les voies aient �t� orient�es.
      * \param v pointeur sur la voie voisine d�j� orient�e.
      */
    virtual void calculerAnglesEtCoordonnees(Voie* v=0)=0;

    /** permet de lier deux voies, c'est-�-dire d'indiquer qu'elles doivent se connecter l'une � l'autre,
      * et sur quelle extr�mit�.
      * \param v pointeur sur la voie � lier.
      * \param ordre indique l'extr�mit� sur laquelle v doit se fixer.
      */
    void lier(Voie* v, int ordre);

    /** permet de savoir si la voie est orient�e, � savoir si la m�thode calculerAnglesEtCoordonnees(...)
      * a d�j� �t� appel�e.
      * \return vrai si la voie est d�j� orient�e, faux sinon.
      */
    bool estOrientee();

    /** permet de savoir si la voie est pos�e, � savoir si la m�thode calculerPosition(...)
      * a d�j� �t� appel�e.
      * \return vrai si la voie est d�j� pos�e, faux sinon.
      */
    bool estPosee();

    /** retourne la position en coordonn�es absolue de l'extr�mit� de la voie reli�e � la voie pass�e en param�tre.
      * \param v la voie reli�e � l'extr�mit� dont la position est d�sir�e.
      * \return la position absolue de l'extr�mit�.
      */
    QPointF* getPosAbsLiaison(Voie* v);

    /** attribue le contact pass� en param�tre � la voie.
      * \param c le contact attribu�.
      */
    void setContact(Contact* c);

    /** retourne le contact de la voie. retourne NULL s'il n'y a pas de contact.
      * \return pointeur sur le contact de la voie.
      */
    Contact* getContact() const;

    /** positionne le contact correctement au milieu de la voie.
      *
      */
    virtual void calculerPositionContact()=0;

    /** Permet l'exploration r�cursive des voies en vue de la cr�ation des segments.
      * \param voieAppelante pointeur sur la voie appelant r�cursivement celle-ci.
      * \return Une liste de liste de voies, correspondant � tous chemins partant de cette voie
      *         et terminant sur un contact, sauf le chemin partant depuis la voie appelante.
      */
    virtual QList<QList<Voie*>*> explorationContactAContact(Voie* voieAppelante)=0;

    /** D�marre l'exploration contact � contact en vue de la cr�ation des segments.
      * \return Une liste de liste de voies, correspondant � tous chemins partant de cette voie
      *         et terminant sur un contact.
      */
    QList<QList<Voie*>*> startExplorationContactAContact();

    /** retourne le nombre de liaisons (en d'autres termes d'extr�mit�s) de la voie.
      * \return le nombre de liaisons de la voie.
      */
    int getNbreLiaisons() const;

    /** retourne la valeur X de l'extr�mit� la plus � gauche de la voie.
      * \return la valeur X de l'extr�mit� la plus � gauche de la voie.
      */
    qreal getXmin() const;

    /** retourne la valeur X de l'extr�mit� la plus � droite de la voie.
      * \return la valeur X de l'extr�mit� la plus � droite de la voie.
      */
    qreal getXmax() const;

    /** retourne la valeur Y de l'extr�mit� la plus haute de la voie.
      * \return la valeur Y de l'extr�mit� la plus haute de la voie.
      */
    qreal getYmin() const;

    /** retourne la valeur Y de l'extr�mit� la plus basse de la voie.
      * \return la valeur Y de l'extr�mit� la plus basse de la voie.
      */
    qreal getYmax() const;

    /** permet de connaitre l'angle de l'extr�mit� connectant cette voie � la voisine.
      * \param voisin la voie voisine.
      * \return l'angle de liaison en degr�s.
      */
    qreal getAngleVoisin(Voie* voisin) const;

    /** permet de connaitre l'angle r�ajust� lorsqu'une locomotive arrive sur une voie.
      * \param voisin la voie d'ou vient la loco.
      * \return l'angle ajust� de la loco.
      */
    qreal getNouvelAngle(Voie* voisin) const;

    /** retourne l'angle en degr�s de l'extr�mit� d'ordre sp�cifi� en param�tre.
      * \param liaison l'ordre de l'extr�mit� dont on veut l'angle.
      * \return l'angle en degr�s.
      */
    qreal getAngleDeg(int liaison) const;

    /** retourne l'angle en radians de l'extr�mit� d'ordre sp�cifi� en param�tre.
      * \param liaison l'ordre de l'extr�mit� dont on veut l'angle.
      * \return l'angle en radians.
      */
    qreal getAngleRad(int liaison) const;

    /** permet de fixer l'angle de l'extr�mit� d'ordre sp�cifi� en param�tre,
      * en passant un angle en degr�s.
      * \param liaison l'ordre de l'extr�mit�
      * \param l'angle en degr�s.
      */
    void setAngleDeg(int liaison, qreal angle);

    /** permet de fixer l'angle de l'extr�mit� d'ordre sp�cifi� en param�tre,
      * en passant un angle en radians.
      * \param liaison l'ordre de l'extr�mit�
      * \param l'angle en radians.
      */
    void setAngleRad(int liaison, qreal angle);

    /** retourne la longueur a parcourir pour traverser la voie.
      * \return la longueur a parcourir pour traverser la voie.
      */
    virtual qreal getLongueurAParcourir()=0;

    /** retourne la voie suivante, en fonction de la voie d'arrivee.
      * \param voieArrivee la voie d'arrivee
      * \return le voie suivante.
      */
    virtual Voie* getVoieSuivante(Voie* voieArrivee)=0;

    /** fournit les informations n�cessaires a l'avancement de la locomotive d'une certaine distance.
      * \param dist la distance � parcourir
      * \param angle l'angle de rotation
      * \param rayon le rayon de rotation
      * \param angleCumule l'angle actuel de la loco
      * \param posActuelle la position actuelle de la loco en coordonnees absolues.
      * \param voieSuivante la voie vers laquelle se dirige la loco.
      */
    virtual void avanceLoco(qreal &dist, qreal &angle, qreal &rayon, qreal angleCumule, QPointF posActuelle, Voie* voieSuivante)=0;

    /** retourne la voie voisine sp�cifi�e par son ordre.
      * \param n l'ordre de la voie
      * \return la voie d'ordre n.
      */
    Voie* getVoieVoisineDOrdre(int n);

    /** permet de sp�cifier la mani�re dont la voie sera dessin�e.
      * \param color la couleur de la voie.
      */
    void setNewPen(const QColor &color);

    /** permet, pour les voies variables, de modifier leur etat.
      * \param nouvelEtat le nouvel etat de la voie variable.
      */
    virtual void setEtat(int nouvelEtat)=0;

    /** permet de corriger la position de l'extr�mit� liant cette voie � la voie v.
      * \param deltaX la correction en X
      * \param deltaY la correction en Y
      * \param v la voie a l'extr�mit� � corriger.
      */
    virtual void correctionPosition(qreal deltaX, qreal deltaY, Voie* v)=0;

    /** permet d'afficher le rectangle englobant la voie. Utile pour le d�buggage.
      * \param painter l'outil de dessin.
      */
    void drawBoundingRect(QPainter *painter);

    void setIdVoie(int id);
protected:
    QMap<int, Voie*> ordreLiaison;
    QMap<int, QPointF*> coordonneesLiaison;
    bool orientee, posee;

    /** normalise l'angle entre 0 et 360 degr�s.
      * \param angle l'angle � normaliser
      * \return l'angle normalis�
      */
    double normaliserAngle(double angle) const;
    QPointF* position;
    QRectF* bRect;
    Contact* contact;
    int idVoie;

    //virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
private:
    QMap<int, qreal> angleLiaison;
};

#endif // VOIE_H
