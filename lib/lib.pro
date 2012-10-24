TEMPLATE = lib

CONFIG += link_pkgconfig

PKGCONFIG = grilo-0.2

TARGET = qtgrilo

SOURCES += griloregistry.cpp grilomedia.cpp grilodatasource.cpp
HEADERS += $$PRIVATE_HEADERS $$PUBLIC_HEADERS

PRIVATE_HEADERS = griloregistry_p.h grilomedia_p.h grilodatasource_p.h
PUBLIC_HEADERS = griloregistry.h grilomedia.h grilodatasource.h

#SOURCES += grilomodel.cpp griloregistry.cpp grilodatasource.cpp \
#           grilobrowse.cpp grilosearch.cpp griloquery.cpp grilomultisearch.cpp

#HEADERS += grilomodel.h griloregistry.h grilodatasource.h \
#           grilobrowse.h grilosearch.h griloquery.h grilomultisearch.h

target.path = /usr/lib/

INSTALLS += target
