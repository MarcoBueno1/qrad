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
INCLUDEPATH = ../../../libs/dsmcommon \
    ../../dsm-sg \
    ../../../libs/exaro \
    ../../../libs/dsmlicense \
    ../../../ecf/DSMIOCommon/inc \
    ../../../libs/m2phi \
    ../../../libs/dsmsecure \
    ../../../libs/dsmmodel \
    ../../../libs/openssl/include \
    ../../../src/include \
    ../../../src/widgets
unix:INCLUDEPATH += ../../../ecf/DSMIOCommon/inc/lnx
win32:INCLUDEPATH += ../../../ecf/DSMIOCommon/inc/win
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
    enterbillet.cpp \
    accounthistoryreport.cpp \
    accounttypereport.cpp \
    accounthistoryreportaux.cpp \
    accountcalendar.cpp \
    accountcalendaraux.cpp
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
    enterbillet.h \
    accounthistoryreport.h \
    accounttypereport.h \
    accounthistoryreportaux.h \
    accountcalendar.h \
    accountcalendaraux.h
win32:LIBS += -L../../../installer/windows \
    -L../../../libs/dsmlicense/release -L../../../libs/dsmlicense/debug\
    -L../../../libs/dsmcommon/release -L../../../libs/dsmcommon/debug\
        -L../../../libs/dsmmodel/release -L../../../libs/dsmmodel/debug\
    -L../../../src/widgets/release -L../../../src/widgets/debug\
    -L../../../libs/dsmround/release -L../../../libs/dsmround/debug\
    -ldsmround

unix:LIBS += -L../../../libs/exaro \
    -L../../../libs/m2phi \
    -L../../../libs/dsmlicense \
    -L../../../libs/dsmcommon \
	-L../../../libs/dsmmodel \
    -L../../../src/widgets \
    -lssl
LIBS += -ldsmcommon \
		-ldsmmodel \
		-ldsmwidgets

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
    enterbillet.ui \
    accounthistoryreport.ui \
    accounttypereport.ui \
    accounthistoryreportaux.ui \
    accountcalendar.ui \
    accountcalendaraux.ui

target.path = $${PREFIX}/plugins
INSTALLS += target
