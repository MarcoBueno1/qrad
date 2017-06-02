#-------------------------------------------------
#
# Project created by QtCreator 2017-05-29T14:15:00
#
#-------------------------------------------------

QT       += core sql

#QT       -= gui

INCLUDEPATH += ../../libs/itaufrench

TARGET = tstitaufrench
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

LIBS += -L../../libs/itaufrench \
        -litaufrench 

