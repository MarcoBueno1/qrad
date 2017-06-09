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
               ../../../../libs/QRadOrm \
               ../orm \
               ../itaufrench

SOURCES +=  \
           editsupplier.cpp \
           managersupplier.cpp \
           managerprofile.cpp \
           editprofile.cpp \
           editlogin.cpp \
           managermetreage.cpp \
           editmetreage.cpp \
    parsepayments.cpp

HEADERS +=  \
           editsupplier.h \
           managersupplier.h \
           managerprofile.h \
           editprofile.h  \
           editlogin.h \
           managermetreage.h \
           editmetreage.h \
    parsepayments.h
		   
FORMS +=  \
         editsupplier.ui \
         managersupplier.ui \
         managerprofile.ui \
         editprofile.ui \
         editlogin.ui \
         managermetreage.ui \
         editmetreage.ui
 
win32:LIBS +=  -L../../../../libs/QRadCore\release \
               -L../../../../libs/QRadHw\release \
               -L../../../../libs/QRadOrm\release \
               -L../orm/release \
               -L../itaufrench/release

unix:LIBS +=  -L../../../../libs/QRadCore \
              -L../../../../libs/QRadOrm \
              -L../../../../libs/QRadHw \
              -L../itaufrench \
              -L../orm


LIBS += -lqradcore  \
        -litaufrench \
        -lblinkorm \
        -lqradorm \
        -lqradhw

target.path = $${PREFIX}/bin
INSTALLS += target

