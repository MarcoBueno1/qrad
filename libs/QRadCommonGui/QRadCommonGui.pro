#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T14:19:01
#
#-------------------------------------------------

QT += core sql network script widgets multimedia multimediawidgets

#QT       -= gui

TARGET = qradcommongui

TEMPLATE = lib

INCLUDEPATH += ../QRadCore \
               ../QRadHw \
               ../QRadOrm

SOURCES +=  \
           edituser.cpp \
           editphone.cpp \
           editaddress.cpp

HEADERS +=  \
           edituser.h \
           editphone.h \
           editaddress.h
		   
FORMS +=  \
         edituser.ui \
         editphone.ui \
         editaddress.ui
 
win32:LIBS +=  -L../QRadCore\release \
               -L../QRadHw\release \
               -L../QRadOrm/release  

unix:LIBS +=  -L../QRadCore \
              -L../QRadHw \
              -L../QRadOrm

LIBS += -lqradcore  \
        -lqradorm \
        -lqradhw

target.path = $${PREFIX}/bin
INSTALLS += target

