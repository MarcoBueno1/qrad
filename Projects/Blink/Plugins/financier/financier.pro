# -------------------------------------------------
# Project created by QtCreator 2009-11-30T10:56:44
# -------------------------------------------------
QT += core \
    gui \
    sql \
    network \
    script \
    widgets
TARGET = financier
TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH = ../../../../libs/QRadCore \
              ../../../../libs/QRadOrm \
              ../../libs/orm \
              ../../../../libs/QRadCommonGui \
              ../../libs/common \
              ../../../../libs/QRadLic \
              ../../libs/acbrticket/ \
              ../../../../libs/3rdpart/exaro

              . 
DESTDIR = .
SOURCES += financierplugin.cpp \
    accounttypemanager.cpp \
    accounttyperegister.cpp \
    accounttopaymanager.cpp \
    accounttoreceivemanager.cpp \
    accounttopayregister.cpp \
    accounttoreceiveregister.cpp \
    bankmanager.cpp \
    bankregister.cpp \
    paidaccount.cpp \
    accounthistoryreport.cpp \
    accounttypereport.cpp \
    accounthistoryreportaux.cpp \
    accountcalendar.cpp \
    accountcalendaraux.cpp \ 
    financierdelegates.cpp \
    editbankaccount.cpp \
    managerbankaccount.cpp \
           managercnab.cpp \
           editcnab.cpp \
           edittipocobranca.cpp \
           managertipocobranca.cpp \
           editlayoutbol.cpp \
           managerlayoutbol.cpp \
           managertipopessoa.cpp \
           edittipopessoa.cpp \
           managerrespemis.cpp \
           editrespemis.cpp \
           managerticketconfig.cpp \
           editticketconfig.cpp \
           managerticket.cpp \
           editticket.cpp \
           ticketcontroller.cpp \
           managershipper.cpp

HEADERS += financierplugin.h \
    accounttypemanager.h \
    accounttyperegister.h \
    accounttopaymanager.h \
    accounttoreceivemanager.h \
    accounttopayregister.h \
    accounttoreceiveregister.h \
    bankmanager.h \
    bankregister.h \
    paidaccount.h \
    accounthistoryreport.h \
    accounttypereport.h \
    accounthistoryreportaux.h \
    accountcalendar.h \
    accountcalendaraux.h \
    financierdelegates.h \
    editbankaccount.h \
    managerbankaccount.h \
           managercnab.h \
           editcnab.h \
           edittipocobranca.h \
           managertipocobranca.h \
           editlayoutbol.h \
           managerlayoutbol.h \
           edittipopessoa.h \
           managertipopessoa.h \
           editrespemis.h \
           managerrespemis.h \
           editticketconfig.h \
           managerticketconfig.h \
           managerticket.h \
           editticket.h \
    ticketcontroller.h \
    managershipper.h
 

win32:LIBS += -L../../../../libs/QRadCore/release \
        -L../../../../libs/QRadOrm/release \
        -L../../libs/orm/release \
        -L../../../../libs/QRadCommonGui/release \
        -L../../libs/common/release \
        -L../../../../libs/QRadLic/release \
        -L../../../../libs/QRadLicUp/release \
        -L../../libs/acbrticket/release

LIBS += -L../../../../libs/QRadCore \
        -L../../../../libs/QRadOrm \
        -L../../../../libs/QRadCommonGui \
        -L../../libs/orm \
        -L../../libs/common \
        -L../../../../libs/QRadLic \
        -L../../../../libs/QRadLicUp \
        -L../../libs/acbrticket/ \
        -lqradcore \
        -lqradorm \
        -lqradlicup \
        -lblinkcommon \
        -lblinkorm \
        -lacbrticket\
        -lqradcommongui



win32:QMAKE_LFLAGS += --enable-auto-import

OTHER_FILES += financier.json

FORMS += accounttypemanager.ui \
    accounttyperegister.ui \
    accounttopaymanager.ui \
    accounttoreceivemanager.ui \
    accounttopayregister.ui \
    accounttoreceiveregister.ui \
    bankmanager.ui \
    bankregister.ui \
    paidaccount.ui \
    accounthistoryreport.ui \
    accounttypereport.ui \
    accounthistoryreportaux.ui \
    accountcalendar.ui \
    accountcalendaraux.ui \
    editbankaccount.ui \
    managerbankaccount.ui \
           managercnab.ui \
           editcnab.ui \
           managertipocobranca.ui \
           edittipocobranca.ui \
           editlayoutbol.ui \
           managerlayoutbol.ui \
           managertipopessoa.ui \
           edittipopessoa.ui \
           managerrespemis.ui \
           editrespemis.ui \
           managerticketconfig.ui \
           editticketconfig.ui \
           managerticket.ui \
           editticket.ui \
           managershipper.ui

target.path = $${PREFIX}/bin 
INSTALLS += target

RESOURCES += \
    ../../icons/resources.qrc \
    reports.qrc
