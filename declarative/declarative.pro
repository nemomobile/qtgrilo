include(../common.pri)

TEMPLATE = lib
CONFIG += qt plugin link_pkgconfig
DEPENDPATH += ../src
INCLUDEPATH += ../src

equals(QT_MAJOR_VERSION, 4): QT += declarative
equals(QT_MAJOR_VERSION, 5): QT = core qml

LIBS += -L../src -l$${GRILOQTLIB}
PKGCONFIG = grilo-0.2

EXAMPLE = ../example/declarative/*

OTHER_FILES += $${EXAMPLE}

TARGET = $${GRILOQTLIB}-qml-plugin
PLUGIN_IMPORT_PATH = org/nemomobile/grilo

QMAKE_SUBSTITUTES = qmldir.in

SOURCES += griloplugin.cpp

HEADERS += griloplugin.h

equals(QT_MAJOR_VERSION, 4): target.path = $$[QT_INSTALL_IMPORTS]/$$PLUGIN_IMPORT_PATH
equals(QT_MAJOR_VERSION, 5): target.path = $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH

qml.files = qmldir
qml.path = $$target.path
INSTALLS += target qml
