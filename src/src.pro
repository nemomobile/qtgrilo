include(../common.pri)

TEMPLATE = lib
CONFIG += qt link_pkgconfig

equals(QT_MAJOR_VERSION, 5): QT = core

PKGCONFIG = grilo-0.2
TARGET = $${GRILOQTLIB}

unix:system(cp pre-grilo-qt.prf.in $${TARGET}.prf.in)
QMAKE_SUBSTITUTES = $${TARGET}.prf.in

DEFINES += GRILO_QT_LIBRARY

# Generate pkg-config support by default
# Note that we HAVE TO also create prl config as QMake implementation
# mixes both of them together.
CONFIG += create_pc create_prl no_install_prl

SOURCES += \
    grilomodel.cpp \
    griloregistry.cpp \
    grilomedia.cpp \
    grilodatasource.cpp \
    grilobrowse.cpp \
    grilosearch.cpp \
    griloquery.cpp \
    grilomultisearch.cpp

HEADERS += \
    griloqt.h \
    grilomodel.h \
    griloregistry.h \
    grilomedia.h \
    grilodatasource.h \
    grilobrowse.h \
    grilosearch.h \
    griloquery.h \
    grilomultisearch.h

INSTALL_HEADERS = \
    GriloQt \
    GriloModel \
    GriloRegistry \
    GriloMedia \
    GriloDataSource \
    GriloBrowse \
    GriloQuery \
    GriloMultiSearch \
    griloqt.h \
    grilomodel.h \
    griloregistry.h \
    grilomedia.h \
    grilodatasource.h \
    grilobrowse.h \
    grilosearch.h \
    griloquery.h \
    grilomultisearch.h

target.path = $$[QT_INSTALL_LIBS]
headers.files = $$INSTALL_HEADERS
headers.path = $$[QT_INSTALL_HEADERS]/GriloQt
prf.files = $${TARGET}.prf
prf.path = $$[QMAKE_MKSPECS]/features
INSTALLS += target headers prf

QMAKE_PKGCONFIG_REQUIRES = grilo-0.2
QMAKE_PKGCONFIG_LIBDIR = $$target.path
QMAKE_PKGCONFIG_INCDIR = $$headers.path
QMAKE_PKGCONFIG_DESTDIR = pkgconfig

equals(QT_MAJOR_VERSION, 4): QMAKE_PKGCONFIG_REQUIRES += Qt4Core
equals(QT_MAJOR_VERSION, 5): QMAKE_PKGCONFIG_REQUIRES += Qt5Core
