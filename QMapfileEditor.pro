#-------------------------------------------------
#
# Project created by QtCreator 2014-09-14T17:22:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += debug_and_release
TARGET = QMapfileEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        mapsettings.cpp \
        mapfileparser.cpp

HEADERS  += mainwindow.h \
    mapsettings.h \
    mapfileparser.h

FORMS    += mainwindow.ui \
    mapsettings.ui \
    layersSetings.ui

INCLUDEPATH += "/usr/include/mapserver" \
  "/usr/include/gdal"

LIBS += -lmapserver

RESOURCES += \
    resources.qrc
