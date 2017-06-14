#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T14:19:01
#
#-------------------------------------------------

QT       += gui core sql widgets

#QT       -= gui

TARGET = qradorm

TEMPLATE = lib

INCLUDEPATH += ../QRadCore

SOURCES +=  \
           orm.cpp \
           model.cpp \
	   phone.cpp \
	   address.cpp \
           user.cpp \
           login.cpp \
           userV2.cpp \
           machine.cpp \
           emailconf.cpp 
            

HEADERS +=  \
           orm.h \
           model.h \
	   phone.h \
	   address.h \
           user.h \
           login.h \
           userV2.h \
           machine.h \
           emailconf.h
		   
#win32:LIBS +=  -L../QRadCore\release
#unix:LIBS +=  -L../QRadCore

#LIBS += -lqradcore

target.path = $${PREFIX}/bin
INSTALLS += target

