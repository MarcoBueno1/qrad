QT       += core gui sql xml  multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql

TARGET = visitplugin
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH = ../../../../libs/QRadCore \
              ../../libs/orm/

DESTDIR = .
SOURCES = camera.cpp \
             visitplugin.cpp \
             managervisit.cpp \ 
             editvisit.cpp \ 
             managerVisitante.cpp \ 
             editVisitante.cpp \
             managerdweller.cpp \ 
             editdweller.cpp \
	     editphone.cpp \
	     editaddress.cpp \
             editpreaut.cpp \
             managerpreaut.cpp \
             editprofile.cpp \
             managerprofile.cpp  \
             edituser.cpp \
             manageruser.cpp \ 
             editemailconf.cpp \
             editmaincompany.cpp \
    editvehicle.cpp \
    editBrand.cpp \
    managerBrand.cpp \
    editveicname.cpp \
    managerveicname.cpp \
    editreason.cpp \
    managerreason.cpp

HEADERS = \ 
             visitplugin.h \ 
             managervisit.h \ 
             editvisit.h \ 
             managerVisitante.h \ 
             editVisitante.h \ 
             camera.h \
             managerdweller.h \ 
             editdweller.h \
             editphone.h \
	     editaddress.h \
             editpreaut.h \
             managerpreaut.h \
             editprofile.h \ 
             managerprofile.h \
             edituser.h \
             manageruser.h \
             editemailconf.h \
             editmaincompany.h \
    editvehicle.h \
    editBrand.h \
    managerBrand.h \
    editveicname.h \
    managerveicname.h \
    editreason.h \
    managerreason.h



FORMS = \ 
             managervisit.ui \ 
             editvisit.ui\ 
             managerVisitante.ui \ 
             editVisitante.ui \ 
             camera.ui \
             editdweller.ui \
             managerdweller.ui \ 
	     editphone.ui \
	     editaddress.ui \
             editpreaut.ui \
             managerpreaut.ui \
             editprofile.ui \
             managerprofile.ui \
             edituser.ui \
             manageruser.ui \ 
             editemailconf.ui \
             editmaincompany.ui \
    editvehicle.ui \
    editBrand.ui \
    managerBrand.ui \
    editveicname.ui \
    managerveicname.ui \
    editreason.ui \
    managerreason.ui



win32:LIBS += -L../../../../libs/QRadCore/release \
              -L../../libs/orm/release
unix:LIBS += -L../../../../libs/QRadCore \
             -L../../libs/orm/

LIBS += -lqradcore \
        -lblinkorm

win32:QMAKE_LFLAGS += --enable-auto-import 

OTHER_FILES += visit.json 

target.path = $${PREFIX}/bin
INSTALLS += target
