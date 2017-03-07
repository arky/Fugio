#-------------------------------------------------
#
# Project created by QtCreator 2015-06-12T21:16:08
#
#-------------------------------------------------

include( ../../FugioGlobal.pri )

QT       += widgets

TARGET = $$qtLibraryTarget(fugio-oculusrift)
TEMPLATE = lib
CONFIG += plugin c++11

DESTDIR = $$DESTDIR/plugins

DEFINES += OCULUSRIFT_LIBRARY

SOURCES += oculusriftplugin.cpp \
	oculusriftnode.cpp \
	deviceoculusrift.cpp

HEADERS += oculusriftplugin.h\
	../../include/fugio/oculusrift/uuid.h \
	../../include/fugio/nodecontrolbase.h \
	../../include/fugio/pincontrolbase.h \
	oculusriftnode.h \
	deviceoculusrift.h

#------------------------------------------------------------------------------
# OSX plugin bundle

macx {
	DEFINES += TARGET_OS_MAC
	CONFIG -= x86
	CONFIG += lib_bundle

	BUNDLEDIR    = $$DESTDIR/$$TARGET".bundle"
	INSTALLDIR   = $$INSTALLBASE/packages/com.bigfug.fugio
	INSTALLDEST  = $$INSTALLDIR/data/plugins
	INCLUDEDEST  = $$INSTALLDIR/data/include/fugio
	FRAMEWORKDIR = $$BUNDLEDIR/Contents/Frameworks

	DESTDIR = $$BUNDLEDIR/Contents/MacOS
	DESTLIB = $$DESTDIR/"lib"$$TARGET".dylib"

	CONFIG(release,debug|release) {
		QMAKE_POST_LINK += echo

		LIBCHANGEDEST = $$DESTLIB

		QMAKE_POST_LINK += $$qtLibChange( QtWidgets )
		QMAKE_POST_LINK += $$qtLibChange( QtGui )
		QMAKE_POST_LINK += $$qtLibChange( QtCore )

		QMAKE_POST_LINK += && defaults write $$absolute_path( "Contents/Info", $$BUNDLEDIR ) CFBundleExecutable "lib"$$TARGET".dylib"

		QMAKE_POST_LINK += && macdeployqt $$BUNDLEDIR -always-overwrite -no-plugins

		QMAKE_POST_LINK += && mkdir -pv $$INSTALLDIR/meta
		QMAKE_POST_LINK += && mkdir -pv $$INSTALLDEST
		QMAKE_POST_LINK += && mkdir -pv $$INCLUDEDEST

		QMAKE_POST_LINK += && rm -rf $$INSTALLDEST/$$TARGET".bundle"

		QMAKE_POST_LINK += && cp -a $$BUNDLEDIR $$INSTALLDEST
	}
}

#------------------------------------------------------------------------------
# Windows

windows {
	INSTALLDIR = $$INSTALLBASE/packages/com.bigfug.fugio

	CONFIG(release,debug|release) {
		QMAKE_POST_LINK += echo

		QMAKE_POST_LINK += & mkdir $$shell_path( $$INSTALLDIR/data/plugins )

		QMAKE_POST_LINK += & copy /V /Y $$shell_path( $$DESTDIR/$$TARGET".dll" ) $$shell_path( $$INSTALLDIR/data/plugins )
	}
}

#------------------------------------------------------------------------------
# Oculus Rift

win32 {
	exists( $$(LIBS)/OculusSDK-1.10.1/LibOVR ) {
		INCLUDEPATH += $$(LIBS)/OculusSDK-1.10.1/LibOVR/Include

		CONFIG(release,debug|release) {
			LIBS += -L$$(LIBS)/OculusSDK-1.10.1/LibOVR/Lib/Windows/Win32/Release/VS2015
		} else {
			LIBS += -L$$(LIBS)/OculusSDK-1.10.1/LibOVR/Lib/Windows/Win32/Debug/VS2015
		}

		LIBS += -lLibOVR -lopengl32

		DEFINES += OCULUS_PLUGIN_SUPPORTED
	}
}

win64 {
	exists( $$(LIBS)/OculusSDK-1.10.1/LibOVR ) {
		INCLUDEPATH += $$(LIBS)/OculusSDK-1.10.1/LibOVR/Include

		LIBS += -L$$(LIBS)/OculusSDK-1.10.1/LibOVR/Lib/Windows/x64/Release/VS2015 -lLibOVR

		LIBS += -lopengl32

		DEFINES += OCULUS_PLUGIN_SUPPORTED
	}
}

!contains( DEFINES, OCULUS_PLUGIN_SUPPORTED ) {
	warning( "Oculus Rift not supported" )
}

#------------------------------------------------------------------------------
# GLEW

win32 {
	INCLUDEPATH += $$(LIBS)/glew-2.0.0/include

	LIBS += -L$$(LIBS)/glew-2.0.0/lib/Release/Win32 -lglew32s

	DEFINES += GLEW_STATIC

	LIBS += -lopengl32
}

macx {
	INCLUDEPATH += /usr/local/include

	LIBS += -L/usr/local/lib -lGLEW
}

#------------------------------------------------------------------------------
# API

INCLUDEPATH += $$PWD/../../include

