PRJ_TOP = ../..
ALL_TOP = $${PRJ_TOP}

CONFIG += silent

# Directories and paths

INCLUDEPATH += \
    ../shared \
    $${ALL_TOP}/ \
    $${ALL_TOP}/lib

OBJECTS_DIR = $${PRJ_TOP}/obj/$$TARGET

DESTDIR = $${PRJ_TOP}/bin

UI_DIR = $$OBJECTS_DIR
MOC_DIR = $$OBJECTS_DIR
RCC_DIR = $$OBJECTS_DIR

# Make much smaller libraries (and packages) by removing debugging informations
QMAKE_CFLAGS_RELEASE -= -g
QMAKE_CXXFLAGS_RELEASE -= -g

QT += network xml widgets script

unix:!mac{
# VERSION DEFINITION
isEmpty(VERSION){
  exists( ../debian/version ){
    VERSION = $$system(cat ../debian/version)
  }
  isEmpty(VERSION){
    VERSION = $$str_member($$system(git describe --tags --match=\'v*.*\'), 1, -1)
  }
}
DEFINES += VERSION=$$VERSION

isEmpty(BRANCH){
  exists( ../debian/branch ){
    BRANCH = $$system(cat ../debian/branch)
  }
  isEmpty(BRANCH){
    BRANCH = $$system(git rev-parse --abbrev-ref HEAD)
  }
}
DEFINES += BRANCH=$$BRANCH

}

#android {
#  include(android.pri)
#}



