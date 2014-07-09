include(../../common.pri)

TEMPLATE = app
CONFIG += qt link_pkgconfig

equals(QT_MAJOR_VERSION, 4): QT += declarative
equals(QT_MAJOR_VERSION, 5): QT = core qml

DEPENDPATH += ../../src
INCLUDEPATH += ../../src
LIBS += -L../../src -l$${GRILOQTLIB}
PKGCONFIG = grilo-0.2

SOURCES += simple.cpp

target.path = /usr/bin

INSTALLS += target
