TEMPLATE = lib
CONFIG += plugin
include( gcu.pri )
include( deploy.pri )

DESTDIR = $$DESTDIR/plugins

HEADERS += ../shared/plugin_interface.h

target.path = $$INSTALLBASE_LIB/plugins/gcu
INSTALLS += target
INSTALLS -= targetx

qtlibs.extra = ../../copy-libs.sh $$DESTDIR/lib$${TARGET}.so $$INSTALLBASE_LIB/Qt/lib

uavos-data {
INSTALLS -= target
}
