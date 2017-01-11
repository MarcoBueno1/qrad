QT += core sql network script widgets multimedia multimediawidgets

TARGET = qradreportplugin
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH = ../../libs/QRadCore

DESTDIR = .
SOURCES = \ 
             qradreport.cpp  

HEADERS = \ 
             qradreport.h  

win32:LIBS += -L../../libs/QRadCore/release 
unix:LIBS += -L../../libs/QRadCore

LIBS += -lqradcore 

win32:QMAKE_LFLAGS += --enable-auto-import 

OTHER_FILES += qradreport.json

target.path = $${PREFIX}/bin
INSTALLS += target
