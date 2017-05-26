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
               ../../../../libs/QRadHw \
               ../orm

SOURCES += editlogin.cpp \
           editsupplier.cpp \
           managersupplier.cpp

HEADERS += editlogin.h \
           editsupplier.h \
           managersupplier.h
		   
FORMS += editlogin.ui \
         editsupplier.ui \
         managersupplier.ui
 
win32:LIBS +=  -L../../../../libs/QRadCore\release \
               -L../../../../libs/QRadHw\release \
               -L../orm/release 
unix:LIBS +=  -L../../../../libs/QRadCore \
              -L../../../../libs/QRadHw \
              -L../orm

LIBS += -lqradcore  \
        -lblinkorm \
        -lqradhw

target.path = $${PREFIX}/bin
INSTALLS += target

