TEMPLATE = lib
CONFIG += qt plugin link_pkgconfig

equals(QT_MAJOR_VERSION, 4): QT += declarative
equals(QT_MAJOR_VERSION, 5): QT = core qml

PKGCONFIG = grilo-0.2

EXAMPLE = ../example/*

OTHER_FILES += $${EXAMPLE}

TARGET = qmlgriloplugin
PLUGIN_IMPORT_PATH = org/nemomobile/grilo

SOURCES += griloplugin.cpp grilomodel.cpp griloregistry.cpp grilomedia.cpp grilodatasource.cpp \
           grilobrowse.cpp grilosearch.cpp griloquery.cpp grilomultisearch.cpp

HEADERS += griloplugin.h grilomodel.h griloregistry.h grilomedia.h grilodatasource.h \
           grilobrowse.h grilosearch.h griloquery.h grilomultisearch.h

equals(QT_MAJOR_VERSION, 4): target.path = $$[QT_INSTALL_IMPORTS]/$$PLUGIN_IMPORT_PATH
equals(QT_MAJOR_VERSION, 5): target.path = $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH

qml.files = qmldir
qml.path = $$target.path

INSTALLS += target qml

equals(QT_MAJOR_VERSION, 5): DEFINES += QT_VERSION_5
