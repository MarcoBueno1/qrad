#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T14:19:01
#
#-------------------------------------------------

QT += core sql network script widgets multimedia multimediawidgets

#QT       -= gui

TARGET = blinkcommon

TEMPLATE = lib

INCLUDEPATH += ../../../../libs/QRadCore \
               ../orm

SOURCES += editlogin.cpp 

HEADERS += editlogin.h 
		   
FORMS += editlogin.ui
 
win32:LIBS +=  -L../../../../libs/QRadCore\release \
               -L../orm/release 
unix:LIBS +=  -L../../../../libs/QRadCore \
              -L../orm

LIBS += -lqradcore  \
        -lblinkorm

target.path = $${PREFIX}/bin
INSTALLS += target

