# -------------------------------------------------
# Project created by QtCreator 2010-02-03T10:37:17
# -------------------------------------------------
QT += xml widgets
TARGET = qradedit
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    highlighter.cpp
HEADERS += mainwindow.h \
    highlighter.h
FORMS += mainwindow.ui
QMAKE_LFLAGS += -Wl,-rpath=./ \
    -Wl,-rpath=$${SOURCE_TREE}/bin
INCLUDEPATH += ../../libs/QRadCore

unix:INCLUDEPATH += ../../libs/QRadCore/system/inc/lnx
win32:INCLUDEPATH += ../../libs/QRadCore/system/inc/win

win32:LIBS +=  -L../../libs/QRadCore\release
unix:LIBS +=  -L../../libs/QRadCore

LIBS += -lqradcore 

win32:QMAKE_LFLAGS += --enable-auto-import

unix:LIBS += -lssl -ldl
RESOURCES += QRadresource.qrc
