#-------------------------------------------------
#
# Project created by QtCreator 2017-05-29T14:15:00
#
#-------------------------------------------------

QT       += core sql widgets

#QT       -= gui

INCLUDEPATH += ../../libs/acbrticket/ \
               ../../libs/orm \
               ../../../../libs/QRadCore \
               ../../../../libs/QRadOrm 

TARGET = tstacbr
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
DEFINES += _NO_WIDGETS_

SOURCES += main.cpp

LIBS += -L../../libs/acbrticket/ \
        -L../../libs/orm \
        -L../../../../libs/QRadCore \
        -L../../../../libs/QRadOrm \
        -L../../../../libs/3rdpart/exaro \
        -lqradorm \
        -lqradcore \
        -lblinkorm \
        -lacbrticket \
        -lReport

target.path = $${PREFIX}/bin
INSTALLS += target
