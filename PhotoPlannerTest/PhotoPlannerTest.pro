QT += testlib positioning
#QT -= gui
QT += gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_photoplanner.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../PhotoPlanner/release/ -lPhotoPlanner
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../PhotoPlanner/debug/ -lPhotoPlanner
else:unix: LIBS += -L$$OUT_PWD/../PhotoPlanner/ -lPhotoPlanner

INCLUDEPATH += $$PWD/../PhotoPlanner
DEPENDPATH += $$PWD/../PhotoPlanner

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../PhotoPlanner/release/libPhotoPlanner.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../PhotoPlanner/debug/libPhotoPlanner.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../PhotoPlanner/release/PhotoPlanner.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../PhotoPlanner/debug/PhotoPlanner.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../PhotoPlanner/libPhotoPlanner.a



unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += proj
