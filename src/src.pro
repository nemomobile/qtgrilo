TEMPLATE = lib
CONFIG += qt plugin link_pkgconfig
QT += declarative

PKGCONFIG = grilo-0.2

DESTDIR = org/nemomobile/grilo
TARGET = qmlgriloplugin

SOURCES += griloplugin.cpp grilomodel.cpp griloregistry.cpp grilomedia.cpp grilodatasource.cpp \
           grilobrowse.cpp grilosearch.cpp griloquery.cpp grilomultisearch.cpp

HEADERS += griloplugin.h grilomodel.h griloregistry.h grilomedia.h grilodatasource.h \
           grilobrowse.h grilosearch.h griloquery.h grilomultisearch.h

target.path = /usr/lib/qt4/imports/org/nemomobile/grilo/
qml.files = qmldir
qml.path = /usr/lib/qt4/imports/org/nemomobile/grilo/

INSTALLS += target qml
