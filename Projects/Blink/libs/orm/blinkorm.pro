#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T14:19:01
#
#-------------------------------------------------

QT       += core sql

#QT       -= gui

TARGET = blinkorm

TEMPLATE = lib

INCLUDEPATH += ../../../../libs/QRadCore

SOURCES += dweller.cpp \
           Visitante.cpp \
		   visit.cpp  \
		   ap.cpp \
		   tower.cpp \
		   phone.cpp \
		   address.cpp \
		   preaut.cpp

HEADERS += dweller.h \
           Visitante.h \
		   visit.h \
		   ap.h \
		   tower.h \
		   phone.h \
		   address.h \
		   preaut.cpp
		   
win32:LIBS +=  -L../../../../libs/QRadCore\release
unix:LIBS +=  -L../../../../libs/QRadCore

LIBS += -lqradcore 
