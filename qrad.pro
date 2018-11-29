# Variables initialization

isEmpty(TESTS) {
	TESTS=0
}

DEBUG=0

SOURCE_TREE=$$quote($${PWD})
BUILD_TREE=$$quote($${OUT_PWD})

isEmpty(PREFIX) {
        PREFIX=$${BUILD_TREE}

}

LIBS=-L$${BUILD_TREE}/bin
win32:QMAKE_LFLAGS += --enable-auto-import

# Include section

include(qmake/check.pri)

include(qmake/cachefile.pri)

CACHERESET() {
	CACHEVAR(PREFIX)
	CACHEVAR(SOURCE_TREE)
	CACHEVAR(BUILD_TREE)
	CACHEVAR(DEBUG)
	CACHEVAR(TESTS)
	CACHEVAR(LIBS)
}

include(qmake/debian.pri)
include(qmake/doc.pri)

# Main project setup

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = QRad \
          libs/QRadOrm \
          libs/QRadHw \
          libs/QRadLic \
          libs/QRadCore \
          libs/QRadLicUp \
          libs/pdfwrapper \
          libs/QRadCommonGui \
          plugins/QRadReport \
          Tools\QRadEdit \
          Tools\QRadPluginTest


message(Done! Now run \"make\" to build QRad)
