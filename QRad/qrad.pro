#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T14:19:01
#
#-------------------------------------------------

QT       += core sql widgets

#QT       -= gui

TARGET = qrad
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    skeleton.cpp \
    sqlparser.cpp \
    uiskeleton.cpp \
    cppsskeleton.cpp \
    hskeleton.cpp \
    pathmaker.cpp \
    managerskeleton.cpp \
    parser.cpp \
    type.cpp \
    field.cpp \
    table.cpp \
    projectskeleton.cpp \
    qradhead.cpp \
    foreingkey.cpp \
    qradgui.cpp

HEADERS += \
    skeleton.h \
    sqlparser.h \
    uiskeleton.h \
    cppsskeleton.h \
    hskeleton.h \
    pathmaker.h \
    managerskeleton.h \
    parser.h \
    type.h \
    field.h \
    table.h \
    projectskeleton.h \
    qradhead.h \
    foreingkey.h \
    qradgui.h

FORMS += \
    qradgui.ui

DISTFILES += \
    UMLModel.qmodel
