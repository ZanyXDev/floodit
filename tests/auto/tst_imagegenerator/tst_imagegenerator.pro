QT += testlib core concurrent qml quick gui

TEMPLATE = app

TARGET = io.github.zanyxdev.floodit.tst_imagegenerator

CONFIG += testcase


INCLUDEPATH += ../../../app/
include(../../../app/tst_imagegenerator.pri)

HEADERS += \
  tst_imagegenerator.h
SOURCES +=  \                       
  main.cpp \
  tst_imagegenerator.cpp

# Force C++17 if available
contains(QT_CONFIG, c++1z): CONFIG += c++1z
# Enable CCache
load(ccache)
