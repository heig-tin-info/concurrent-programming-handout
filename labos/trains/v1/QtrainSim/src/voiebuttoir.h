#ifndef VOIEBUTTOIR_H
#define VOIEBUTTOIR_H

#include <voie.h>
#include <loco.h>

class VoieButtoir : public Voie
{
    Q_OBJECT

public:
    VoieButtoir(qreal longueur);
    void calculerAnglesEtCoordonnees(Voie *v);
    void calculerPositionContact();
    QList<QList<Voie*>*> explorationContactAContact(Voie *voieAppelante);
    qreal getLongueurAParcourir();
    Voie* getVoieSuivante(Voie* voieArrivee);
    void avanceLoco(qreal &dist, qreal &angle, qreal &rayon, qreal angleCumule, QPointF posActuelle, Voie *voieSuivante);
    void correctionPosition(qreal deltaX, qreal deltaY, Voie *v);
    void correctionPositionLoco(qreal &x, qreal &y);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setEtat(int nouvelEtat);

private:
    qreal longueur;
};

#endif // VOIEBUTTOIR_H
