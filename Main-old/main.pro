#-------------------------------------------------
#
# Project created by QtCreator 2016-02-23T19:07:47
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = main
TEMPLATE = app

INCLUDEPATH += ../QRad/generated \
               ../libs

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS += -L../libs \
        -lqradcore
