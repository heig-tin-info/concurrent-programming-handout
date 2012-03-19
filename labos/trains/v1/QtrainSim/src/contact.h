#ifndef CONTACT_H
#define CONTACT_H

#include <QObject>
#include <QAbstractGraphicsShapeItem>
#include <QMutex>
#include <QWaitCondition>
#include <QPainter>
#include <general.h>
#include <QDebug>
#include <math.h>

class Contact : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT
public:
    /** Constructeur de Classe.
      * \param numContact le numero du contact.
      * \param numVoiePorteuse le num�ro de la voie porteuse du contact.
      * \param parent le parent du contact. Pas utilis� � la construction.
      */
    explicit Contact(int numContact, int numVoiePorteuse, QObject *parent = 0);

    /** M�thode bloquante, permettant d'attendre sur l'activation du contact.
      */
    void attendContact();

    /** M�thode appel�e quand une loco passe sur le contact.
      * Lib�re les threads en attente.
      */
    void active();

    /** retourne le num�ro de la voie porteuse.
      * \return le num�ro de la voie porteuse.
      */
    int getNumVoiePorteuse();

    /** Permet de sp�cifier selon quel angle le num�ro du contact doit �tre d�cal�.
      * \param angle l'angle en radians.
      */
    void setAngle(qreal angle);

    /** retourne le rectangle englobant le contact. N�cessaire � l'affichage.
      * \return le rectangle englobant le contact.
      */
    QRectF boundingRect() const;

    /** M�thode d'affichage. Appel�e par QGraphicsView lors du dessin de la fenetre.
      * pas besoin de se pr�occuper des parametres.
      */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /** retourne le num�ro du contact.
      * \return le num�ro du contact.
      */
    int getNumContact();
signals:

public slots:

private:
    int numVoiePorteuse;
    int numContact;
    QWaitCondition* VarCond;
    QMutex* mutex;
    qreal angle;
    bool waitingOn;
};

#endif // CONTACT_H
