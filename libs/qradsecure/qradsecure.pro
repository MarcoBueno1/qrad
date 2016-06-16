# -------------------------------------------------
# Project created by QtCreator 2010-01-28T20:16:09
# -------------------------------------------------
QT += xml
QT -= gui
TARGET = qradsecure
TEMPLATE = lib
CONFIG += staticlib
SOURCES += dsmsecure.cpp \
    ssltypes.cpp \
    dsmsecurefile.cpp \
    dsmsecuresettings.cpp
HEADERS += dsmsecure.h \
    ssltypes.h \
    securekey.h \
    dsmsecurefile.h \
    dsmsecuresettings.h
INCLUDEPATH +=  ../QRadCore
unix:INCLUDEPATH += ./lnx
win32:INCLUDEPATH += ./win
unix:LIBS+= -ldl
INCLUDEPATH += ../openssl/include
win32:DESTDIR = ../dsmsecure
unix: DESTDIR = .
