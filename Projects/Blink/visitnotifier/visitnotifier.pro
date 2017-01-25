#-------------------------------------------------
#
# Project created by QtCreator 2017-01-25T10:31:25
#
#-------------------------------------------------

QT       += core sql network widgets

QT       -= gui

TARGET = visitnotifier
CONFIG   += console
CONFIG   -= app_bundle

INCLUDEPATH = ../../../libs/QRadCore \
              ../libs/orm/


TEMPLATE = app


SOURCES += main.cpp

win32:LIBS += -L../../../libs/QRadCore/release \
              -L../libs/orm/release
unix:LIBS += -L../../../libs/QRadCore \
             -L../libs/orm/

LIBS += -lqradcore \
        -lblinkorm

win32:QMAKE_LFLAGS += --enable-auto-import
