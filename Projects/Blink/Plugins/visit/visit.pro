QT       += core gui sql xml  multimedia multimediawidgets script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql

TARGET = visitplugin
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH = ../../../../libs/QRadCore \
              ../../../../libs/QRadCommonGui \
              ../../../../libs/QRadOrm \
              ../../../../libs/QRadHw \
              ../../../../libs/3rdpart/exaro \
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
             editmaincompany.cpp \
    editvehicle.cpp \
    editBrand.cpp \
    managerBrand.cpp \
    editveicname.cpp \
    managerveicname.cpp \
    editreason.cpp \
    managerreason.cpp \
    manageruser.cpp \
    managercommon_area.cpp \
    editcommon_area.cpp \
#    managerreserve.cpp \
#    editreserve.cpp \
    choosecommonarea.cpp \
    calendarreserve.cpp \
    dayreserve.cpp \
    newdayreserve.cpp \
    choosereportreserve.cpp

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
             editmaincompany.h \
    editvehicle.h \
    editBrand.h \
    managerBrand.h \
    editveicname.h \
    managerveicname.h \
    editreason.h \
    managerreason.h \
    manageruser.h \
    managercommon_area.h \
    editcommon_area.h \
#    managerreserve.h \
#    editreserve.h \
    choosecommonarea.h \
    calendarreserve.h \
    dayreserve.h \
    newdayreserve.h \
    choosereportreserve.h



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
             editmaincompany.ui \
    editvehicle.ui \
    editBrand.ui \
    managerBrand.ui \
    editveicname.ui \
    managerveicname.ui \
    editreason.ui \
    managerreason.ui \
    manageruser.ui \
    managercommon_area.ui \
    editcommon_area.ui \
#    managerreserve.ui \
#    editreserve.ui \
    choosecommonarea.ui \
    calendarreserve.ui \
    dayreserve.ui \
    newdayreserve.ui \
    choosereportreserve.ui



win32:LIBS += -L../../../../libs/QRadCore/release \
              -L../../../../libs/QRadCommonGui/release \
              -L../../../../libs/QRadOrm/release \
              -L../../libs/orm/release \
              -L../../libs/common/release
unix:LIBS += -L../../../../libs/QRadCore \
             -L../../../../libs/QRadCommonGui \
              -L../../../../libs/QRadOrm\
             -L../../libs/orm/ \
             -L../../libs/common/

LIBS += -lqradcore \
        -lblinkorm \
        -lqradorm \
        -lblinkcommon \
        -lqradcommongui

win32:QMAKE_LFLAGS += --enable-auto-import 

OTHER_FILES += visit.json 

target.path = $${PREFIX}/bin
INSTALLS += target

RESOURCES += \
    reports.qrc
