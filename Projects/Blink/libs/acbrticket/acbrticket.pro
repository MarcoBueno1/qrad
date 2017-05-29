#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T14:19:01
#
#-------------------------------------------------

QT       += core sql

#QT       -= gui

TARGET = acbrticket

TEMPLATE = lib

INCLUDEPATH += ../../../../libs/QRadCore \
               ../../../../libs/QRadOrm \
               ../orm

SOURCES += buildtkt.cpp
            

HEADERS += buildtkt.h
		   
win32:LIBS +=  -L../../../../libs/QRadCore\release \
               -L../../../../libs/QRadOrm\release \
               -L../orm\release
unix:LIBS +=  -L../../../../libs/QRadCore \
              -L../../../../libs/QRadOrm \
               -L../orm

LIBS += -lqradcore \
        -lqradorm \
        -lblinkorm

target.path = $${PREFIX}/bin
INSTALLS += target

