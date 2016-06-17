# -------------------------------------------------
# Project created by QtCreator 2009-11-30T10:56:44
# -------------------------------------------------
QT += core \
    gui \
    sql \
    widgets

TARGET = qradlicup
TEMPLATE = lib
INCLUDEPATH = ../QRadCore \
     ../QRadLic 

DESTDIR = .
SOURCES += qradlicup.cpp 
	
HEADERS += qradlicup.h 

win32:LIBS += -L../QRadCore/release \
    -L../QRadLic/release

unix:LIBS += -L../QRadCore \
    -L../QRadLic
	
LIBS += -lqradcore \
    -lqradlic

win32:QMAKE_LFLAGS += --enable-auto-import

target.path = $${PREFIX}/bin
INSTALLS += target
