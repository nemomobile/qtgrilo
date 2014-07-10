include(../../common.pri)

TEMPLATE = app
TARGET = simple
CONFIG += qt link_pkgconfig

equals(QT_MAJOR_VERSION, 4): QT += declarative
equals(QT_MAJOR_VERSION, 5): QT += widgets qml

DEPENDPATH += ../../src
INCLUDEPATH += ../../src
LIBS += -L../../src -l$${GRILOQTLIB}
PKGCONFIG = grilo-0.2

SOURCES += \
    simple-model.cpp \
    simple.cpp

HEADERS += simple-model.h

target.path = /usr/bin

INSTALLS += target
