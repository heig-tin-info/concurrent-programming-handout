message("Building solution project 2")
include(../../QtrainSim/QtrainSim.pri)

CONFIG += c++17

LIBS += -lpcosynchro

HEADERS +=  \
    src/badsharedsection.h \
    src/parcours.h \
    src/sharedsectioninterface.h \
    src/sharedsectionsettings.h \
    src/locomotive.h \
    src/launchable.h \
    src/locomotivebehavior.h \
    src/sharedsection.h

SOURCES +=  \
    src/locomotive.cpp \
    src/cppmain.cpp \
    src/locomotivebehavior.cpp
