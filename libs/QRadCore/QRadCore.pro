# #####################################################################
# Automatically generated by qmake (2.01a) seg abr 12 14:59:02 2010
# #####################################################################
QT += gui \
    sql \
    network \
    xml \
    widgets \
    core \
    script \
    printsupport \
    network \
    axcontainer

TEMPLATE = lib
TARGET = qradcore
DEPENDPATH += .

win32:INCLUDEPATH += . \
               ../3rdpart/libpq/include \
               ../3rdpart/exaro

win32:INCLUDEPATH += ./system/inc/win
unix:INCLUDEPATH += ./system/inc/lnx \
                    /usr/include/postgresql\
                   ../3rdpart/exaro

INCLUDEPATH += ../QRadOrm \
               ../QRadLic

HEADERS +=  qradplugincontainer.h \
              qradcoreplugin.h \
              qradpluginaction.h \
              qradround.h \
              qradlineeditfakeenter.h \
              qradtableviewsearch.h \
              qradmoney.h \
              column2delegate.h \
              qradsplashscreen.h \
              qradplugininterface.h \
              scombobox.h \
              qradshared.h \
              qraddebug.h \
              qradconfig.h \
              qradlog.h \
    qradmenu.h \
    stableview.h \
    qradlistview.h \
    qradfile.h \
    qradsecure.h \
    qradsecurefile.h \
    qradsecuresettings.h \
    securekey.h \
    ssltypes.h \ 
    qradlineeditcompleter.h \
    qradlineeditenter.h \
    coid.h \
#    profilemanager.h \
#    profileregister.h \
    qraddoublespinboxenter.h \
    docvalidate.h \
    qraddateeditfakeenter.h \
    qradtimeeditenter.h \
    smtp.h \
    m2smtp.h \
    emailaddress.h \
    mimeattachment.h \
    mimefile.h \
    mimehtml.h \
    mimeinlinefile.h \
    mimemessage.h \
    mimepart.h \
    mimetext.h \
    smtpclient.h \
    SmtpMime \
    quotedprintable.h \
    mimemultipart.h \
    mimecontentformatter.h \
    smtpexports.h \
    dsmcalendar.h \
    dsmdateeditenter.h \
    dsmgroupboxenter.h \
    dsmcomboboxenter.h \
    dsmspinboxenter.h \
    dsmtextedit.h \
    dsmradiobuttonenter.h \
    dsmmoney.h \
    qradreportmanager.h \
    reportfactory.h \
    exaroreport.h \
    qradreportinterface.h \
    qradlicensecontrol.h \
    bankticket.h \
    bankticketinterface.h \
    qradprogresswindow.h \
    qrademail.h \
    emailgui.h \
    connectservererror.h \
    excelexporthelper.h \
    qraddelegate.h \
    qradcalendar.h





win32:HEADERS  += system/inc/win/qradsystem.h
unix::HEADERS  += system/inc/lnx/qradsystem.h

SOURCES +=  qradplugininterface.cpp \
             qradpluginaction.cpp \
             qradplugincontainer.cpp \
             qradcoreplugin.cpp \
             scombobox.cpp \
             qradround.cpp \
             qradlineeditfakeenter.cpp \
             qradtableviewsearch.cpp \
             qradmoney.cpp \
             column2delegate.cpp \
             qradsplashscreen.cpp \
             qradconfig.cpp \
             qradlog.cpp \
    qradmenu.cpp \
    stableview.cpp \
    qradlistview.cpp \
    qradfile.cpp \
    qradsecure.cpp \
    qradsecurefile.cpp \
    qradsecuresettings.cpp \
    ssltypes.cpp \
    qradlineeditcompleter.cpp \
    qradlineeditenter.cpp \
    coid.cpp \
#    profilemanager.cpp \
#    profileregister.cpp \
    qraddoublespinboxenter.cpp \
    docvalidate.cpp \
    qraddateeditfakeenter.cpp \
    qradtimeeditenter.cpp \
    smtp.cpp \
    m2smtp.cpp \
    emailaddress.cpp \
    mimeattachment.cpp \
    mimefile.cpp \
    mimehtml.cpp \
    mimeinlinefile.cpp \
    mimemessage.cpp \
    mimepart.cpp \
    mimetext.cpp \
    smtpclient.cpp \
    quotedprintable.cpp \
    mimemultipart.cpp \
    mimecontentformatter.cpp \
    dsmcalendar.cpp \
    dsmdateeditenter.cpp \
    dsmgroupboxenter.cpp \
    dsmcomboboxenter.cpp \
    dsmspinboxenter.cpp \
    dsmtextedit.cpp \
    dsmradiobuttonenter.cpp \
    dsmmoney.cpp \
    qradreportmanager.cpp \
    reportfactory.cpp \
    exaroreport.cpp\
    qradlicensecontrol.cpp\
    bankticket.cpp \
    qradprogresswindow.cpp \
    qrademail.cpp \
    emailgui.cpp \
    connectservererror.cpp \
    excelexporthelper.cpp \
    qraddelegate.cpp \
    qradcalendar.cpp

win32:SOURCES  += system/src/win/qradsystem.c
unix::SOURCES  += system/src/lnx/qradsystem.c

win32:LIBS += -L ../3rdpart/libpq/win/lib   \
             -leay32
unix:LIBS += -lssl

LIBS += -lpq \
        -L../3rdpart/exaro \
        -L../QRadOrm/release \
        -L../QRadOrm/ \
        -L../QRadLic/ \
        -L../QRadLic/release \
        -lReport \
        -lqradorm \
        -lqradlic

RESOURCES += \
    ../../Projects/Blink/icons/resources.qrc

win32:QMAKE_LFLAGS += --enable-auto-import

#INSTALLS += target
#INSTALLS += target

FORMS += \
    qradlicensecontrol.ui \
    qradprogresswindow.ui \
    emailgui.ui \
    connectservererror.ui
#    profileregister.ui

target.path = $${PREFIX}/bin
INSTALLS += target

