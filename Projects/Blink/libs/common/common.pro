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
              ../../../../libs/QRadCommonGui \
               ../../../../libs/QRadOrm \
               ../orm \
               ../itaufrench \
               ../acbrticket

SOURCES +=  \
           editsupplier.cpp \
           managersupplier.cpp \
           managerprofile.cpp \
           editprofile.cpp \
           editlogin.cpp \
           managermetreage.cpp \
           editmetreage.cpp \
#           parsepayments.cpp \
           bankticketparserfactory.cpp \
    showbankreturn.cpp

HEADERS +=  \
           editsupplier.h \
           managersupplier.h \
           managerprofile.h \
           editprofile.h  \
           editlogin.h \
           managermetreage.h \
           editmetreage.h \
#           parsepayments.h \
           bankticketparserfactory.h \
    showbankreturn.h
		   
FORMS +=  \
         editsupplier.ui \
         managersupplier.ui \
         managerprofile.ui \
         editprofile.ui \
         editlogin.ui \
         managermetreage.ui \
         editmetreage.ui \
    showbankreturn.ui
 
win32:LIBS +=  -L../../../../libs/QRadCore\release \
               -L../../../../libs/QRadHw\release \
               -L../../../../libs/QRadCommonGui/release \
               -L../../../../libs/QRadOrm\release \
               -L../orm/release \
               -L../itaufrench/release \
               -L../acbrticket/release \

unix:LIBS +=  -L../../../../libs/QRadCore \
              -L../../../../libs/QRadOrm \
              -L../../../../libs/QRadCommonGui \
              -L../../../../libs/QRadHw \
              -L../itaufrench \
              -L../acbrticket \
              -L../orm


LIBS += -lqradcore  \
        -litaufrench \
        -lacbrticket \
        -lblinkorm \
        -lqradorm \
        -lqradhw \
        -lqradcommongui

target.path = $${PREFIX}/bin
INSTALLS += target

RESOURCES +=

