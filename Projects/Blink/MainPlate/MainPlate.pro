#-------------------------------------------------
#
# Project created by QtCreator 2016-02-23T19:07:47
#
#-------------------------------------------------

QT       += core gui sql xml  multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MainPlate
TEMPLATE = app

INCLUDEPATH += ../../../libs/QRadCore \
               ../../../libs/QRadLicUp \
               ../../../libs/QRadLic \
               ../libs/common \
               ../libs/orm \
               ../../../libs/3rdpart/ocr/openalpr


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS += -L../../../libs/QRadCore \
        -L../../../libs/QRadCore/release \
        -L../../../libs/QRadLicUp/release \
        -L../../../libs/QRadLicUp \
        -L../libs/common/ \
        -L../libs/common/release \
        -L../libs/orm/ \
        -L../libs/orm/release \
        -lqradcore \
        -lqradlicup \
        -lblinkcommon \
        -lblinkorm \


target.path = $${PREFIX}/bin
INSTALLS += target
