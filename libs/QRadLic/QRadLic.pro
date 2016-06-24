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
win32:LIBS += -L../3rdpart/libpq/win/lib \
              -L../../3rdpart/libpq/win/lib \
              -leay32
unix:LIBS += -lssl

