

QT += widgets sql

TARGET		=	Elise

TEMPLATE	=	app

VERSION		=	0.0.0.1
DEFINES		+=	UNICODE

SOURCES += \
	src/elise.cpp \
	src/tests.cpp \
	src/services.cpp \
	src/modules.cpp \
	src/modules/tray.cpp \
	src/modules/logon.cpp \
	src/modules/options.cpp


HEADERS += \
    src/commonheaders.h \
    src/tests.h \
    src/services.h \
    src/modules.h \
    src/modules/tray.h \
    src/modules/logon.h \
	../api/e_pluginapi.h \
	../api/e_dbpluginapi.h \
	../api/e_database.h \
	../api/e_options.h \
	src/modules/options.h

RESOURCES += \
	res/resources.qrc

CONFIG (debug, debug|release) {
#	OBJECTS_DIR = ../Elise-build/debug
	DESTDIR = ../../Elise-build-debug

}else{
#	OBJECTS_DIR = ../Elise-build/release
	DESTDIR = ../../Elise-build-release
}

#target.path = F:/Sources/Elise-build-release/

#INSTALLS += target











