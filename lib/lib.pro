TEMPLATE = lib

CONFIG += link_pkgconfig

PKGCONFIG = grilo-0.2

TARGET = qtgrilo

SOURCES += griloregistry.cpp
HEADERS += griloregistry.h griloregistry_p.h

#SOURCES += grilomodel.cpp griloregistry.cpp grilomedia.cpp grilodatasource.cpp \
#           grilobrowse.cpp grilosearch.cpp griloquery.cpp grilomultisearch.cpp

#HEADERS += grilomodel.h griloregistry.h grilomedia.h grilodatasource.h \
#           grilobrowse.h grilosearch.h griloquery.h grilomultisearch.h

target.path = /usr/lib/

INSTALLS += target
