QT += testlib positioning
#QT -= gui
QT += gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle
CONFIG  += qt exception c++14

TEMPLATE = app

SOURCES +=  tst_photoplanner.cpp

unix: CONFIG += link_pkgconfig

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../PhotoPlannerMath/release/ -lPhotoPlannerMath
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../PhotoPlannerMath/debug/ -lPhotoPlannerMath
else:unix: LIBS += -L$$OUT_PWD/../PhotoPlannerMath/ -lPhotoPlannerMath

INCLUDEPATH += $$PWD/../PhotoPlannerMath
DEPENDPATH += $$PWD/../PhotoPlannerMath

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../PhotoPlannerMath/release/libPhotoPlannerMath.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../PhotoPlannerMath/debug/libPhotoPlannerMath.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../PhotoPlannerMath/release/PhotoPlannerMath.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../PhotoPlannerMath/debug/PhotoPlannerMath.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../PhotoPlannerMath/libPhotoPlannerMath.a
