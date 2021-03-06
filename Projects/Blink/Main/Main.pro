#-------------------------------------------------
#
# Project created by QtCreator 2016-02-23T19:07:47
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = main
TEMPLATE = app

INCLUDEPATH += ../../../libs/QRadCore \
               ../../../libs/QRadLicUp \
               ../../../libs/QRadLic \
               ../../../libs/QRadOrm \
               ../libs/common \
               ../libs/orm 


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS += -L../../../libs/QRadCore \
        -L../../../libs/QRadCore/release \
        -L../../../libs/QRadLicUp/release \
        -L../../../libs/QRadOrm/release \
        -L../../../libs/QRadLicUp \
        -L../../../libs/QRadCommonGui \
        -L../../../libs/QRadCommonGui/release \
        -L../../../libs/QRadOrm \
        -L../libs/common/ \
        -L../libs/common/release \
        -L../libs/orm/ \
        -L../libs/orm/release \
        -L../libs/itaufrench \
        -L../libs/itaufrench/release \
        -L../libs/acbrticket \
        -L../libs/acbrticket/release \
        -L../../../libs/3rdpart/exaro \
        -lqradcore \
        -lqradlicup \
        -lblinkcommon \
        -lblinkorm \
        -lqradorm \
        -litaufrench \
        -lacbrticket \
        -L../../../bin -lReport \
        -lqradcommongui


RESOURCES += \
    ../icons/resources.qrc

win32:RC_ICONS += ..\icons\marca-m.ico


target.path = $${PREFIX}/bin
INSTALLS += target
