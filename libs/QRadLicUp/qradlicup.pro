# -------------------------------------------------
# Project created by QtCreator 2009-11-30T10:56:44
# -------------------------------------------------
QT += core \
    gui \
    sql 

TARGET = qradlicup
TEMPLATE = lib
INCLUDEPATH = ../../qradcore \
     ../../qradlic 

DESTDIR = .
SOURCES += qradlicup.cpp 
	
HEADERS += qradlicup.h 

win32:LIBS += -L../../qradcore/release \
    -L../../qradlic/release

unix:LIBS += -L../../qradcore/release \
    -L../../qradlic/release
	
LIBS += -lqradcore \
    -lqradlic

win32:QMAKE_LFLAGS += --enable-auto-import
