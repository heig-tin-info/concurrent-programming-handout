#ifndef VOIEAIGUILLAGEENROULE_H
#define VOIEAIGUILLAGEENROULE_H

#include <voievariable.h>
#include <loco.h>

class VoieAiguillageEnroule : public VoieVariable
{
    Q_OBJECT
public:
    explicit VoieAiguillageEnroule(qreal angle, qreal rayon, qreal longueur, qreal direction);
    void setNumVoieVariable(int numVoieVariable);
    void calculerAnglesEtCoordonnees(Voie *v);
    void calculerPositionContact();
    QList<QList<Voie*>*> explorationContactAContact(Voie* voieAppelante);
    qreal getLongueurAParcourir();
    Voie* getVoieSuivante(Voie* voieArrivee);
    void avanceLoco(qreal &dist, qreal &angle, qreal &rayon, qreal angleCumule, QPointF posActuelle, Voie *voieSuivante);
    void correctionPosition(qreal deltaX, qreal deltaY, Voie *v);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
private:
    qreal rayonInterieur, rayonExterieur, angle, longueur, direction;
    QPointF* centreInterieur;
    QPointF* centreExterieur;

};

#endif // VOIEAIGUILLAGEENROULE_H
