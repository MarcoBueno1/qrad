#-------------------------------------------------
#
# Project created by QtCreator 2011-11-17T15:03:09
#
#-------------------------------------------------

QT       += core sql

QT       -= gui

TARGET = qradlicense
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../libs/QRadLic
SOURCES += main.cpp

win32:LIBS += -L../../libs/QRadLic/release \
    -leay32
unix:LIBS += -L../../libs/QRadLic \
    -lssl -lcrypto
LIBS += -lqradlic

win32:QMAKE_LFLAGS += --enable-auto-import

target.path = $${PREFIX}/bin
INSTALLS += target


