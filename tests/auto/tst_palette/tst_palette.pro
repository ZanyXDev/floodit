QT += testlib
QT -= gui

TARGET = io.github.zanyxdev.floodit.tst_palette

CONFIG += qt console warn_on depend_includepath testcase
CONFIG += no_testcase_installs
CONFIG -= app_bundle

TEMPLATE = app
INCLUDEPATH += ../../../app/
include(../../../app/app.pri)

HEADERS += \
                        tst_palette.h
SOURCES +=  \
                        tst_palette.cpp \
                        main.cpp
