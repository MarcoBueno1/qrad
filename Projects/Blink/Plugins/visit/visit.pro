QT += core sql network script widgets multimedia multimediawidgets

TARGET = visitplugin
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH = ../../../../libs/QRadCore \
              ../../libs/orm/

DESTDIR = .
SOURCES = \ 
             visitplugin.cpp \ 
             managervisit.cpp \ 
             editvisit.cpp \ 
             managerVisitante.cpp \ 
             editVisitante.cpp \
    camera.cpp \
    editdweller.cpp \
	editphone.cpp \
	editaddress.cpp

HEADERS = \ 
             visitplugin.h \ 
             managervisit.h \ 
             editvisit.h \ 
             managerVisitante.h \ 
             editVisitante.h \ 
    camera.h \
    editdweller.h \
    editphone.h \
	editaddress.h



FORMS = \ 
             managervisit.ui \ 
             editvisit.ui\ 
             managerVisitante.ui \ 
             editVisitante.ui \ 
    camera.ui \
    editdweller.ui \
	editphone.ui \
	editaddress.ui



win32:LIBS += -L../../../../libs/QRadCore/release \
              -L../../libs/orm/release
unix:LIBS += -L../../../../libs/QRadCore \
             -L../../libs/orm/

LIBS += -lqradcore \
        -lblinkorm

win32:QMAKE_LFLAGS += --enable-auto-import 

OTHER_FILES += visit.json 

DISTFILES +=
