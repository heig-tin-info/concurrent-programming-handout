#include "voiedroite.h"

VoieDroite::VoieDroite(qreal longueur)
    :Voie()
{
    setNewPen(COULEUR_DROITE);
    this->longueur = longueur;
    this->orientee = false;
    this->posee = false;
}

void VoieDroite::calculerAnglesEtCoordonnees(Voie *v)
{
    int ordreVoieFixe;
    if(v==NULL)
    {
        ordreVoieFixe = 0;
        setAngleDeg(0, 0.0);
    }
    else
    {
        ordreVoieFixe = ordreLiaison.key(v);
        setAngleDeg(ordreVoieFixe, normaliserAngle(v->getAngleVoisin(this) + 180.0));
    }

    if(ordreVoieFixe == 0)
    {
        setAngleDeg(1, normaliserAngle(getAngleDeg(0) + 180.0));
    }
    else
    {
        setAngleDeg(0, normaliserAngle(getAngleDeg(1) + 180.0));
    }

    //calculer position relative de 0 et 1.
    coordonneesLiaison[0]->setX(0.0);
    coordonneesLiaison[0]->setY(0.0);
    coordonneesLiaison[1]->setX(longueur * cos(getAngleRad(1)));
    coordonneesLiaison[1]->setY(- longueur * sin(getAngleRad(1)));

    if(this->contact != NULL)
        calculerPositionContact();

    orientee = true;

    if(!ordreLiaison[0]->estOrientee())
        ordreLiaison[0]->calculerAnglesEtCoordonnees(this);
    if(!ordreLiaison[1]->estOrientee())
        ordreLiaison[1]->calculerAnglesEtCoordonnees(this);
}

void VoieDroite::calculerPositionContact()
{
    this->contact->setPos(longueur * cos(getAngleRad(1)) / 2.0, longueur * sin(getAngleRad(1)) / 2.0);
    this->contact->setAngle(atan2(- coordonneesLiaison[1]->y(), - coordonneesLiaison[1]->x()) + PI / 2.0);
}

QList<QList<Voie*>*> VoieDroite::explorationContactAContact(Voie* voieAppelante)
{
    QList<QList<Voie*>*> temp;

    if(this->contact == NULL)
    {
        if(ordreLiaison.key(voieAppelante) == 0)
            temp.append(ordreLiaison.value(1)->explorationContactAContact(this));
        else
            temp.append(ordreLiaison.value(0)->explorationContactAContact(this));

        for(int i=0; i< temp.length(); i++)
        {
            temp.at(i)->prepend(this);
        }
    }
    else
    {
        QList<Voie*>* temp2 = new QList<Voie*>();
        temp2->append(this);
        temp.append(temp2);
    }

    return temp;
}

qreal VoieDroite::getLongueurAParcourir()
{
    return longueur;
}

Voie* VoieDroite::getVoieSuivante(Voie *voieArrivee)
{
    return ordreLiaison.value((ordreLiaison.key(voieArrivee) +1) % 2);
}

void VoieDroite::avanceLoco(qreal &dist, qreal &angle, qreal &rayon, qreal angleCumule, QPointF posActuelle, Voie *voieSuivante)
{
    angle = 0.0;
    rayon = 0.0;

    if(sqrt((posActuelle.x() - getPosAbsLiaison(voieSuivante)->x()) *
            (posActuelle.x() - getPosAbsLiaison(voieSuivante)->x()) +
            (posActuelle.y() - getPosAbsLiaison(voieSuivante)->y()) *
            (posActuelle.y() - getPosAbsLiaison(voieSuivante)->y())) < dist)
    {
        dist -= sqrt((posActuelle.x() - getPosAbsLiaison(voieSuivante)->x()) *
                     (posActuelle.x() - getPosAbsLiaison(voieSuivante)->x()) +
                     (posActuelle.y() - getPosAbsLiaison(voieSuivante)->y()) *
                     (posActuelle.y() - getPosAbsLiaison(voieSuivante)->y()));
    }
    else
    {
        dist = 0.0;
    }
}

void VoieDroite::correctionPosition(qreal deltaX, qreal deltaY, Voie *v)
{
    //Correction...
    if(ordreLiaison.key(v) == 0)
    {
        setPos(this->pos().x() + deltaX, this->pos().y() + deltaY);
        coordonneesLiaison[1]->setX(coordonneesLiaison[1]->x() - deltaX);
        coordonneesLiaison[1]->setY(coordonneesLiaison[1]->y() - deltaY);
    }
    else
    {
        coordonneesLiaison[1]->setX(coordonneesLiaison[1]->x() + deltaX);
        coordonneesLiaison[1]->setY(coordonneesLiaison[1]->y() + deltaY);
    }

//    setAngleRad(0, atan2(coordonneesLiaison[1]->y(), coordonneesLiaison[1]->x()));
//    setAngleRad(1, atan2(-coordonneesLiaison[1]->y(), -coordonneesLiaison[1]->x()));


    if(this->contact != NULL)
        calculerPositionContact();
}

#define min(a,b) ((a<b)?(a):(b))

QRectF VoieDroite::boundingRect() const
{
    QRectF rect(min(coordonneesLiaison[0]->x(),coordonneesLiaison[1]->x()),
                min(coordonneesLiaison[0]->y(),coordonneesLiaison[1]->y()),
                abs(coordonneesLiaison[0]->x()-coordonneesLiaison[1]->x()),
                abs(coordonneesLiaison[0]->y()-coordonneesLiaison[1]->y()));
    rect.adjust(-10,-10,10,10);
    return rect;
}

void VoieDroite::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(this->pen());
    painter->drawLine(*coordonneesLiaison[0], *coordonneesLiaison[1]);
    drawBoundingRect(painter);
}

void VoieDroite::setEtat(int nouvelEtat)
{
    qDebug() << "Appel de setEtat sur une voie non variable.";
}
