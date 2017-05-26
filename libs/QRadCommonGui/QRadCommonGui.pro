#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T14:19:01
#
#-------------------------------------------------

QT += core sql network script widgets multimedia multimediawidgets

#QT       -= gui

TARGET = qradcommongui

TEMPLATE = lib

INCLUDEPATH += ../../../../libs/QRadCore \
               ../../../../libs/QRadHw \
               ../orm

SOURCES += editlogin.cpp \
           edituser.cpp \
           editphone.cpp \
           editaddress.cpp

HEADERS += editlogin.h \
           edituser.h \
           editphone.h \
           editaddress.h
		   
FORMS += editlogin.ui \
         edituser.ui \
         editphone.ui \
         editaddress.ui
 
win32:LIBS +=  -L../QRadCore\release \
               -L../QRadHw\release \
               -L../QRadOrm/release  

unix:LIBS +=  -L../QRadCore \
              -L../libs/QRadHw \
              -L../QRadOrm

LIBS += -lqradcore  \
        -lqradorm \
        -lqradhw

target.path = $${PREFIX}/bin
INSTALLS += target

