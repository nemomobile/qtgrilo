TEMPLATE = lib
CONFIG += qt plugin link_pkgconfig
QT += declarative

PKGCONFIG = grilo-0.2

EXAMPLE = ../example/*

OTHER_FILES += $${EXAMPLE}

TARGET = qmlgriloplugin
PLUGIN_IMPORT_PATH = org/nemomobile/grilo

SOURCES += griloplugin.cpp grilomodel.cpp griloregistry.cpp grilomedia.cpp grilodatasource.cpp \
           grilobrowse.cpp grilosearch.cpp griloquery.cpp grilomultisearch.cpp

HEADERS += griloplugin.h grilomodel.h griloregistry.h grilomedia.h grilodatasource.h \
           grilobrowse.h grilosearch.h griloquery.h grilomultisearch.h

target.path = $$[QT_INSTALL_IMPORTS]/$$PLUGIN_IMPORT_PATH
qml.files = qmldir
qml.path = $$[QT_INSTALL_IMPORTS]/$$$$PLUGIN_IMPORT_PATH

INSTALLS += target qml
