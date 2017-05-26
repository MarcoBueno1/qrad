#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T14:19:01
#
#-------------------------------------------------

QT       += core sql

#QT       -= gui

TARGET = qradorm

TEMPLATE = lib

INCLUDEPATH += ../QRadCore

SOURCES +=  \
	   phone.cpp \
	   address.cpp \
           user.cpp \
           login.cpp \
           userV2.cpp \
           machine.cpp 
            

HEADERS +=  \
	   phone.h \
	   address.h \
           user.h \
           login.h \
           userV2.h \
           machine.h
		   
win32:LIBS +=  -L../QRadCore\release
unix:LIBS +=  -L../QRadCore

LIBS += -lqradcore 

target.path = $${PREFIX}/bin
INSTALLS += target

