#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T14:19:01
#
#-------------------------------------------------

QT       += core sql

#QT       -= gui

TARGET = blinkorm

TEMPLATE = lib

INCLUDEPATH += ../../../../libs/QRadCore \
               ../../../../libs/QRadOrm

SOURCES += dweller.cpp \
           Visitante.cpp \
	   visit.cpp  \
	   ap.cpp \
	   tower.cpp \
	   preaut.cpp \
           profile.cpp \
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
    paymentmodel.cpp \
    bankmodel.cpp \
    accounttoreceivehistorymodel.cpp \
    accounttopayhistorymodel.cpp \
    supplier.cpp
            

HEADERS += dweller.h \
           Visitante.h \
	   visit.h \
	   ap.h \
	   tower.h \
	   preaut.h \
           profile.h \
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
    paymentmodel.h \
    bankmodel.h \
    accounttoreceivehistorymodel.h \
    accounttopayhistorymodel.h \
    supplier.h
		   
win32:LIBS +=  -L../../../../libs/QRadCore\release \
               -L../../../../libs/QRadOrm\release
unix:LIBS +=  -L../../../../libs/QRadCore \
              -L../../../../libs/QRadOrm

LIBS += -lqradcore \
        -lqradorm

target.path = $${PREFIX}/bin
INSTALLS += target

