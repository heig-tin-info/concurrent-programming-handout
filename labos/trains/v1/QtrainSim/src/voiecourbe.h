#ifndef VOIECOURBE_H
#define VOIECOURBE_H

#include <voie.h>
#include <loco.h>

class VoieCourbe : public Voie
{
    Q_OBJECT

public:
    VoieCourbe(qreal angle, qreal rayon, int direction);
    void calculerAnglesEtCoordonnees(Voie *v);
    void calculerPositionContact();
    QList<QList<Voie*>*> explorationContactAContact(Voie* voieAppelante);
    qreal getLongueurAParcourir();
    Voie* getVoieSuivante(Voie* voieArrivee);
    void avanceLoco(qreal &dist, qreal &angle, qreal &rayon, qreal angleCumule, QPointF posActuelle, Voie *voieSuivante);
    void correctionPosition(qreal deltaX, qreal deltaY, Voie *v);
    void correctionPositionLoco(qreal &x, qreal &y);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setEtat(int nouvelEtat);

private:
    QPointF* centre;
    qreal rayon, angle;
    int direction;
};

#endif // VOIECOURBE_H
