# -------------------------------------------------
# Project created by QtCreator 2009-09-26T19:04:59
# -------------------------------------------------
QT -= gui
QT += network
TARGET = qradhw 
TEMPLATE = lib
CONFIG += staticlib
SOURCES += m2phi.cpp
INCLUDEPATH += ../QRadCore
win32 {
    SOURCES += rawio.cpp
}
HEADERS += m2phi.h \
    hdreg.h

win32 {
    HEADERS += rawio.h
}
DESTDIR += .
