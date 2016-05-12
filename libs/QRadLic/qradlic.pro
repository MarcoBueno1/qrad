# -------------------------------------------------
# Project created by QtCreator 2010-01-08T13:10:21
# -------------------------------------------------
QT += network \
    sql
TARGET = qradlic
TEMPLATE = lib
CONFIG += staticlib
SOURCES += ccrypt.cpp \
    clicense.cpp
HEADERS += ccrypt.h \
    clicense.h \
    dsmerrors.h
win32:LIBS += -leay32
unix:LIBS += -lssl

