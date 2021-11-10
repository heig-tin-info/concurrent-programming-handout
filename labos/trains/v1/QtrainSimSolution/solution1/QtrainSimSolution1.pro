message("Building solution project 1")
include(../../QtrainSim/QtrainSim.pri)

HEADERS +=  \
    src/locomotive.h

SOURCES +=  \
    src/locomotive.cpp \
    src/cppmain.cpp
