# -------------------------------------------------
# Project created by QtCreator 2010-03-15T10:05:47
# -------------------------------------------------
QT += widgets \
sql


TARGET = qradplugintest
TEMPLATE = app
SOURCES += main.cpp \
    dialog.cpp \
    pluginmodel.cpp \
    propertytablemodel.cpp
HEADERS += dialog.h \
    ../../libs/QRadCore/qradplugininterface.h \
    pluginmodel.h \
    propertytablemodel.h
FORMS += dialog.ui
INCLUDEPATH += ../../libs/QRadCore/ 


unix:QMAKE_LFLAGS += \
	-Wl,-rpath=$${SOURCE_TREE}/bin \ 
	-Wl,-rpath=$${SOURCE_TREE}/libs/3rdpart/exaro

win32:QMAKE_LFLAGS += \
	-Wl,-rpath=$${SOURCE_TREE}/bin \
	-Wl,-rpath=$${SOURCE_TREE}/libs/3rdpart/exaro


win32:LIBS += \
        -L$${SOURCE_TREE}/bin


unix:LIBS += \
        -L$${SOURCE_TREE}/bin

LIBS +=  -L../../libs/QRadCore/release -lqradcore \
         -L../../libs/3rdpart/exaro \
         -lReport


win32:QMAKE_LFLAGS += --enable-auto-import

#RESOURCES += \
#    ../../Projects/Blink/icons/resources.qrc

target.path = $${PREFIX}/bin
INSTALLS += target
