#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T14:19:01
#
#-------------------------------------------------

QT += core sql network script widgets multimedia multimediawidgets xml

TARGET = qradcommongui

TEMPLATE = lib

INCLUDEPATH += ../QRadCore \
               ../QRadHw \
               ../QRadOrm \
               ../QRadLic

SOURCES +=  \
           edituser.cpp \
           editphone.cpp \
           editaddress.cpp \
           editemailconf.cpp \
           editfiles.cpp \
           managerfiles.cpp \
           managerassociation.cpp

HEADERS +=  \
           edituser.h \
           editphone.h \
           editaddress.h \
           editemailconf.h \
           editfiles.h \
           managerfiles.h \
           managerassociation.h
#    persistfiletodatabase.h
		   
FORMS +=  \
         edituser.ui \
         editphone.ui \
         editaddress.ui \
         editemailconf.ui \
         editfiles.ui \
         managerfiles.ui \
        managerassociation.ui
#    persistfiletodatabase.ui
 
win32:LIBS +=  -L../QRadCore\release \
               -L../QRadHw\release \
               -L../QRadOrm/release

unix:LIBS +=   \
              -L../QRadCore \
              -L../QRadHw \
              -L../QRadOrm

LIBS += -L../bin \
        -lqradcore  \
        -lqradorm \
        -lqradhw

win32:QMAKE_LFLAGS += --enable-auto-import

target.path = $${PREFIX}/bin
INSTALLS += target

