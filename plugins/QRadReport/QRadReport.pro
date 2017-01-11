QT += core sql network script widgets multimedia multimediawidgets xml

TARGET = qradreportplugin
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH = ../../libs/QRadCore \
              ../../libs/pdfwrapper

DESTDIR = .
SOURCES = \ 
             qradreport.cpp  \
             ormreport.cpp \
    reportlauncher.cpp

HEADERS = \ 
             qradreport.h \
             ormreport.h \
    reportlauncher.h

win32:LIBS += -L../../libs/QRadCore/release -L../../libs/QRadCore -L../../libs/pdfwrapper/release
unix:LIBS += -L../../libs/QRadCore -L../../libs/pdfwrapper

LIBS += -lqradcore \
        -lpdfwrapper

win32:QMAKE_LFLAGS += --enable-auto-import 

OTHER_FILES += qradreport.json

target.path = $${PREFIX}/bin
INSTALLS += target

FORMS += \
    reportlauncher.ui
