QT += testlib core concurrent qml quick gui

TEMPLATE = app

TARGET = io.github.zanyxdev.floodit.tst_imageprovider

CONFIG += testcase


INCLUDEPATH += ../../../app/
include(../../../app/tst_imageprovider.pri)

HEADERS += \
  tst_imageprovider.h
SOURCES +=  \                       
  main.cpp \
  tst_imageprovider.cpp

# Force C++17 if available
contains(QT_CONFIG, c++1z): CONFIG += c++1z
