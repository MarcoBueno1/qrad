#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T14:19:01
#
#-------------------------------------------------

QT       += core sql

#QT       -= gui

TARGET = itaufrench

TEMPLATE = lib

INCLUDEPATH += ../../../../libs/QRadCore 

SOURCES += itauinterface.cpp \
    itauparser.cpp \
    itauticket.cpp
            

HEADERS += itauinterface.h \
    pdf2txt.h \
    itauparser.h \
    itauticket.h
		   
win32:LIBS +=  -L../../../../libs/QRadCore\release 
unix:LIBS +=  -L../../../../libs/QRadCore 

LIBS += -lqradcore 

target.path = $${PREFIX}/bin
INSTALLS += target

