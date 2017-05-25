#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T14:19:01
#
#-------------------------------------------------

QT       += core sql

#QT       -= gui

TARGET = blinkorm

TEMPLATE = lib

INCLUDEPATH += ../../../../libs/QRadCore

SOURCES += dweller.cpp \
           Visitante.cpp \
	   visit.cpp  \
	   ap.cpp \
	   tower.cpp \
	   phone.cpp \
	   address.cpp \
	   preaut.cpp \
           profile.cpp \
           user.cpp \
           login.cpp \
           emailconf.cpp \
           maincompany.cpp \
           machine.cpp \ 
    vehicle.cpp \
    Brand.cpp \
    veicname.cpp \
    reason.cpp \
    accounttypemodel.cpp \
    accounttopaymodel.cpp \
    accounttoreceivemodel.cpp \
    debtmodel.cpp \
    paymentwaymodel.cpp \
    bankmodel.cpp \
    accounttoreceivehistorymodel.cpp \
    accounttopayhistorymodel.cpp \
    userV2.cpp
            

HEADERS += dweller.h \
           Visitante.h \
	   visit.h \
	   ap.h \
	   tower.h \
	   phone.h \
	   address.h \
	   preaut.h \
           profile.h \
           user.h \
           login.h \
           emailconf.h \
           maincompany.h \
           machine.h \ 
    vehicle.h \
    Brand.h \
    veicname.h \
    reason.h \
    accounttypemodel.h \
    accounttopaymodel.h \
    accounttoreceivemodel.h \
    debtmodel.h \
    paymentwaymodel.h \
    bankmodel.h \
    accounttoreceivehistorymodel.h \
    accounttopayhistorymodel.h \
    userV2.h
		   
win32:LIBS +=  -L../../../../libs/QRadCore\release
unix:LIBS +=  -L../../../../libs/QRadCore

LIBS += -lqradcore 

target.path = $${PREFIX}/bin
INSTALLS += target

