#include <QMutexLocker>

#include "locomotive.h"
#include "ctrain_handler.h"

Locomotive::Locomotive() :
    mutex(QMutex::Recursive),
    stopped(false),
    _numero(-1),
    _vitesse(0),
    _enFonction(false)
{

}

Locomotive::Locomotive(int numero, int vitesse) :
    mutex(QMutex::Recursive),
    stopped(false),
    _numero(numero),
    _vitesse(vitesse),
    _enFonction(false)
{

}

int Locomotive::numero() const
{
    return _numero;
}

void Locomotive::fixerNumero(int numero)
{
    _numero = numero;
}

int Locomotive::vitesse() const
{
    return _vitesse;
}

void Locomotive::fixerVitesse(int vitesse)
{
    _vitesse = vitesse;

    if (_enFonction)
        mettre_vitesse_progressive(_numero, vitesse);
}

void Locomotive::fixerPosition(int contactAvant, int contactArriere)
{
    assigner_loco(contactAvant, contactArriere, _numero, _vitesse);
}

void Locomotive::afficherMessage(const QString &message)
{
    afficher_message_loco(_numero, qPrintable(message));
}

void Locomotive::allumerPhares()
{
    if (!stopped)
        mettre_fonction_loco(_numero, ALLUME);
}

void Locomotive::eteindrePhares()
{
    if (!stopped)
        mettre_fonction_loco(_numero, ETEINT);
}

void Locomotive::demarrer()
{
    QMutexLocker locker(&mutex);
    if (!stopped) {
        mettre_vitesse_progressive(_numero, _vitesse);
        _enFonction = true;
    }
}

void Locomotive::arreter()
{
    QMutexLocker locker(&mutex);
    arreter_loco(_numero);
    _enFonction = false;
}

void Locomotive::inverserSens()
{
    QMutexLocker locker(&mutex);
    if (!stopped)
        inverser_sens_loco(_numero);
}

void Locomotive::emergencyStop()
{
    QMutexLocker locker(&mutex);
    stopped = true;
    arreter();
}
