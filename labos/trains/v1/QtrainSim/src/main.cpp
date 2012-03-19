#include <QtGui/QApplication>
#include <QSettings>

#include <QDebug>

#include "ctrain_handler.h"

#include <iostream>
using namespace std;

#ifdef MAQUETTE
#include "commandetrain.h"
#endif // MAQUETTE

/**
 * Programme principal
 */
int main(int argc, char *argv[])
{

    QApplication app(argc,argv);

#ifdef MAQUETTE
    CommandeTrain::getInstance()->init_maquette();
#endif // MAQUETTE

    init_maquette();
    return app.exec();
}
