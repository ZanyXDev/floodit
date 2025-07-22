QT += testlib
TEMPLATE = app

TARGET = io.github.zanyxdev.floodit.tst_palette

CONFIG += console qt
CONFIG += testcase
CONFIG += no_testcase_installs
CONFIG -= app_bundle


INCLUDEPATH += ../../../app/
include(../../../app/app.pri)

HEADERS += \
    tst_palette.h
SOURCES +=  \
    main.cpp \
    tst_palette.cpp

# Force C++17 if available
contains(QT_CONFIG, c++1z): CONFIG += c++1z

