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
              ../../libs/orm \
              ../../../../libs/QRadLic \
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
    financierdelegates.cpp

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
    financierdelegates.h

LIBS += -L../../../../libs/QRadCore \
        -L../../libs/orm \
        -L../../../../libs/QRadLic \
        -l \
        -lqradcore
        -lqradlicup


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
    accountcalendaraux.ui

target.path = $${PREFIX}/plugins
INSTALLS += target
