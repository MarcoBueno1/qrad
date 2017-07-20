#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T14:19:01
#
#-------------------------------------------------

QT       += gui core sql widgets

#QT       -= gui

TARGET = qradorm

TEMPLATE = lib

INCLUDEPATH += ../QRadCore

win32:INCLUDEPATH += ../3rdpart/libpq/include

unix:INCLUDEPATH += /usr/include/postgresql\

SOURCES +=  \
           orm.cpp \
           model.cpp \
	   phone.cpp \
	   address.cpp \
           user.cpp \
           login.cpp \
           userV2.cpp \
           machine.cpp \
           emailconf.cpp \
           fieldfactory.cpp \
           cmdmysql.cpp \
           cmdpostgres.cpp \
           cmdsqlite.cpp \
           pgsqlasync.cpp \
           qradmodellog.cpp \
           maincompany.cpp \
    files.cpp \
    filedescription.cpp

HEADERS +=  \
           orm.h \
           model.h \
	   phone.h \
	   address.h \
           user.h \
           login.h \
           userV2.h \
           machine.h \
           emailconf.h \
           fieldfactory.h \
           cmdinterface.h \
           cmdmysql.h \
           cmdpostgres.h \
           cmdsqlite.h \
           pgsqlasync.h \
           qradmodellog.h \
           maincompany.h \
    files.h \
    filedescription.h

#win32:LIBS +=  -L../QRadCore\release
#unix:LIBS +=  -L../QRadCore

LIBS += -lpq

target.path = $${PREFIX}/bin
INSTALLS += target

