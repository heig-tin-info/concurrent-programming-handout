#ifndef VOIEAIGUILLAGE_H
#define VOIEAIGUILLAGE_H

#include <voievariable.h>
#include <loco.h>

class VoieAiguillage : public VoieVariable
{
    Q_OBJECT

public:
    VoieAiguillage(qreal angle, qreal rayon, qreal longueur, qreal direction);
    void setNumVoieVariable(int numVoieVariable);
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

    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
private:
    qreal rayon, angle, longueur, direction;
    QPointF* centre;
};

#endif // VOIEAIGUILLAGE_H
