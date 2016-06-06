QT += core sql network script widgets multimedia multimediawidgets

TARGET = report
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH = ../../../../libs/QRadCore \
               ../../../../libs/pdfwrapper \
              ../../libs/orm/

DESTDIR = .
SOURCES = \ 
             reportplugin.cpp 

HEADERS = \ 
             reportplugin.h 

win32:LIBS += -L../../../../libs/QRadCore/release \
              -L../../../../libs/pdfwrapper/release \
              -L../../libs/orm/release
unix:LIBS += -L../../../../libs/QRadCore \
             -L../../../../libs/pdfwrapper \
             -L../../libs/orm/

LIBS += -lqradcore \
        -lblinkorm \
        -lpdfwrapper

win32:QMAKE_LFLAGS += --enable-auto-import 

OTHER_FILES += report.json 

target.path = $${PREFIX}/bin
INSTALLS += target
