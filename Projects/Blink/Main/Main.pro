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
               ../libs/common \
               ../libs/orm


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS += -L../../../libs/QRadCore \
        -L../../../libs/QRadCore/release \
        -L../libs/common/ \
        -L../libs/common/release \
        -L../libs/orm/ \
        -L../libs/orm/release \
        -lqradcore \
        -lblinkcommon \
        -lblinkorm \


target.path = $${PREFIX}/bin
INSTALLS += target
