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
           maincompanyV2.cpp \
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
    supplier.cpp \
    bankaccount.cpp \
         cnab.cpp \
         tipocobranca.cpp \
         layoutbol.cpp \
         tipopessoa.cpp \
         respemis.cpp \
         ticketconfig.cpp \
         city.cpp \
         state.cpp \
         metreage.cpp \
         tktspecie.cpp
            

HEADERS += dweller.h \
           Visitante.h \
	   visit.h \
	   ap.h \
	   tower.h \
	   preaut.h \
           profile.h \
           emailconf.h \
           maincompany.h \
           maincompanyV2.h \
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
    supplier.h \
    bankaccount.h \
         cnab.h \
         tipocobranca.h \
         layoutbol.h \ 
         tipopessoa.h \ 
         respemis.h \ 
         ticketconfig.h \
    city.h \
    state.h \
    metreage.h \
    tktspecie.h
		   
win32:LIBS +=  -L../../../../libs/QRadCore\release \
               -L../../../../libs/QRadOrm\release
unix:LIBS +=  -L../../../../libs/QRadCore \
              -L../../../../libs/QRadOrm

LIBS += -lqradcore \
        -lqradorm

target.path = $${PREFIX}/bin
INSTALLS += target

