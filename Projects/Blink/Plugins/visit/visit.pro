QT       += core gui sql xml  multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql

TARGET = visitplugin
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH = ../../../../libs/QRadCore \
              ../../../../libs/QRadCommonGui \
              ../../../../libs/QRadOrm \
              ../../../../libs/QRadHw \
              ../../libs/orm \
              ../../libs/common

DESTDIR = .
SOURCES = camera.cpp \
             visitplugin.cpp \
             managervisit.cpp \ 
             editvisit.cpp \ 
             managerVisitante.cpp \ 
             editVisitante.cpp \
             managerdweller.cpp \ 
             editdweller.cpp \
             editpreaut.cpp \
             managerpreaut.cpp \
             editemailconf.cpp \
             editmaincompany.cpp \
    editvehicle.cpp \
    editBrand.cpp \
    managerBrand.cpp \
    editveicname.cpp \
    managerveicname.cpp \
    editreason.cpp \
    managerreason.cpp \
    editlogin.cpp

HEADERS = \ 
             visitplugin.h \ 
             managervisit.h \ 
             editvisit.h \ 
             managerVisitante.h \ 
             editVisitante.h \ 
             camera.h \
             managerdweller.h \ 
             editdweller.h \
             editpreaut.h \
             managerpreaut.h \
             editemailconf.h \
             editmaincompany.h \
    editvehicle.h \
    editBrand.h \
    managerBrand.h \
    editveicname.h \
    managerveicname.h \
    editreason.h \
    managerreason.h \
    editlogin.h



FORMS = \ 
             managervisit.ui \ 
             editvisit.ui\ 
             managerVisitante.ui \ 
             editVisitante.ui \ 
             camera.ui \
             editdweller.ui \
             managerdweller.ui \ 
             editpreaut.ui \
             managerpreaut.ui \
             editemailconf.ui \
             editmaincompany.ui \
    editvehicle.ui \
    editBrand.ui \
    managerBrand.ui \
    editveicname.ui \
    managerveicname.ui \
    editreason.ui \
    managerreason.ui \
    editlogin.ui



win32:LIBS += -L../../../../libs/QRadCore/release \
              -L../../../../libs/QRadCommonGui/release \
              -L../../libs/orm/release \
              -L../../libs/common/release
unix:LIBS += -L../../../../libs/QRadCore \
             -L../../../../libs/QRadCommonGui \
             -L../../libs/orm/ \
             -L../../libs/common/

LIBS += -lqradcore \
        -lblinkorm \
        -lblinkcommon \
        -lqradcommongui

win32:QMAKE_LFLAGS += --enable-auto-import 

OTHER_FILES += visit.json 

target.path = $${PREFIX}/bin
INSTALLS += target
