#-------------------------------------------------
#
# Project created by QtCreator 2016-03-16T07:56:03
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testcompleter
TEMPLATE = app

INCLUDEPATH += ../libs/

SOURCES += main.cpp\
        mainwindow.cpp 

HEADERS  += mainwindow.h 

win32:LIBS += -L../libs/release
unix:LIBS += -L../libs
LIBS +=  -lqradcore
FORMS    += mainwindow.ui
