#-------------------------------------------------
#
# Project created by QtCreator 2017-01-25T10:31:25
#
#-------------------------------------------------

QT       += gui core sql network widgets

#QT       -= gui

TARGET = notifier
CONFIG   += console
CONFIG   -= app_bundle

INCLUDEPATH = ../../../libs/QRadCore \
              ../../../libs/QRadOrm \
              ../libs/orm/


TEMPLATE = app


SOURCES += main.cpp \
    task.cpp

win32:LIBS += -L../../../libs/QRadCore/release \
              -L../../../libs/QRadOrm/release \
              -L../libs/orm/release
unix:LIBS += -L../../../libs/QRadCore \
             -L../../../libs/QRadOrm \
             -L../libs/orm/ \
             -L../../../libs/3rdpart/exaro

LIBS += -lqradcore \
        -lqradorm \
        -lblinkorm \
        -lReport

win32:QMAKE_LFLAGS += --enable-auto-import

target.path = $${PREFIX}/bin
INSTALLS += target

HEADERS += \
    task.h
