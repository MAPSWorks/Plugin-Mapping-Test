#-------------------------------------------------
#
# Project created by QtCreator 2017-10-23T11:17:24
#
#-------------------------------------------------

QT       += core gui quickwidgets script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Planner
TEMPLATE = app

include( ../../gcu.pri )

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += ../..

SOURCES += \
        main.cpp \
        photoplannerdialog.cpp \
#    ../src/shared/QMandala.cpp \
#    ../src/shared/QMandalaField.cpp \
#    ../src/shared/QMandalaItem.cpp \
#    ../lib/Mandala.cpp \
#    ../lib/MandalaCore.cpp \
#    ../lib/MatrixMath.cpp \
#    ../src/shared/FlightDataFile.cpp

HEADERS += \
        photoplannerdialog.h \
#    ../src/shared/QMandala.h \
#    ../src/shared/QMandalaField.h \
#    ../src/shared/QMandalaItem.h \
#    ../lib/Mandala.h \
#    ../lib/MandalaCore.h \
#    ../lib/MatrixMath.h \
#    ../src/shared/FlightDataFile.h

FORMS += \
        photoplannerdialog.ui