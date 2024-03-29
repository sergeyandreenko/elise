QT			+=	sql widgets

TARGET		=	BDPlugin

TEMPLATE	=	lib

CONFIG		+=	plugin

DEFINES		+=	UNICODE

VERSON		= 0.0.0.2

SOURCES += \
	src/dbplugin.cpp \
	src/elsiedb.cpp

HEADERS += \
	../../api/version.h \
	../../api/e_dbpluginapi.h \
	../../api/e_pluginapi.h \
	src/dbplugin.h \
	src/elisedb.h \
    ../../api/e_database.h

OTHER_FILES += \
	metadata.json

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/tools/echoplugin/plugin
#sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS plugin.pro
#sources.path = $$[QT_INSTALL_EXAMPLES]/tools/echoplugin/plugin
#INSTALLS += target sources
#CONFIG (debug, debug|release) {
#	target.path = ../Elise-build-debug/plugins/
#}else{
#	#target.path = ../Elise-build-release/plugins/
#}

CONFIG (debug, debug|release) {
#	OBJECTS_DIR = ../Elise-build/debug/plugins/newplugin
	DESTDIR = ../../../Elise-debug/Plugins
	#DEFINES		+=	_DEBUG
}else{
#	OBJECTS_DIR = ../Elise-build/release/plugins/newplugin
	DESTDIR = ../../../Elise-release/Plugins
}


















