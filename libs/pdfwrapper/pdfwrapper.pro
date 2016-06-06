#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T14:19:01
#
#-------------------------------------------------

QT       += core sql

#QT       -= gui

TARGET = pdfwrapper 

TEMPLATE = lib

INCLUDEPATH += ../3rdpart/libharu/src/include \
               ../3rdpart/libharu/bin/include/

SOURCES += pdfwrapper.cpp 

HEADERS += pdfwrapper.h 
		   
LIBS +=  -L../3rdpart/libharu/bin/src/

LIBS += -lhpdf 

target.path = $${PREFIX}/bin
INSTALLS += target

