# -------------------------------------------------
# Project created by QtCreator 2011-07-05T10:48:02
# -------------------------------------------------
QT += core \
    gui widgets printsupport multimedia

# DEFINES += DRAW_BOUNDINGRECT
TARGET = QtrainSim
TEMPLATE = app
CONFIG += debug
#CONFIG += MAQUETTE

MAQUETTE: DEFINES += MAQUETTE

MAQUETTE : LIBS += -lmarklin

RESOURCES += qtrainsim.qrc

win32 {
    DEFINES += ON_WINDOWS
    UI_DIR = tmp/win/ui
    MOC_DIR = tmp/win/moc
    OBJECTS_DIR = tmp/win/obj
    RCC_DIR = tmp/win/rcc
    LIBS += -lpthreadGC2
}
unix {
    DEFINES += ON_LINUX
    UI_DIR = tmp/linux/ui
    MOC_DIR = tmp/linux/moc
    OBJECTS_DIR = tmp/linux/obj
    RCC_DIR = tmp/linux/rcc
}
macx {
    DEFINES += ON_MACOS
    UI_DIR = tmp/mac/ui
    MOC_DIR = tmp/mac/moc
    OBJECTS_DIR = tmp/mac/obj
    RCC_DIR = tmp/mac/rcc
}
DESTDIR = bin
INCLUDEPATH += src
SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/voie.cpp \
    src/voiedroite.cpp \
    src/voiecourbe.cpp \
    src/voieaiguillage.cpp \
    src/voiecroisement.cpp \
    src/voievariable.cpp \
    src/voiebuttoir.cpp \
    src/voietraverseejonction.cpp \
    src/simview.cpp \
    src/commandetrain.cpp \
    src/loco.cpp \
    src/contact.cpp \
    src/segment.cpp \
    src/trainsimsettings.cpp \
    src/maquettemanager.cpp \
    src/voieaiguillageenroule.cpp \
    src/voieaiguillagetriple.cpp \
    src/ctrain_handler.cpp \
    src/cmain.c

HEADERS += src/mainwindow.h \
    src/voie.h \
    src/voiedroite.h \
    src/voiecourbe.h \
    src/voieaiguillage.h \
    src/voiecroisement.h \
    src/voievariable.h \
    src/voiebuttoir.h \
    src/voietraverseejonction.h \
    src/simview.h \
    src/connect.h \
    src/commandetrain.h \
    src/general.h \
    src/loco.h \
    src/contact.h \
    src/segment.h \
    src/trainsimsettings.h \
    src/maquettemanager.h \
    src/voieaiguillageenroule.h \
    src/voieaiguillagetriple.h \
    src/ctrain_handler.h

OTHER_FILES += infosVoies.txt
