TEMPLATE = app

TARGET = io.github.zanyxdev.floodit.tests

CONFIG += console qt

QT += testlib
CONFIG += testcase
CONFIG += no_testcase_installs

INCLUDEPATH += ../../../app/
include(../../../app/app.pri)

SOURCES += \
    main.cpp \
    testmyclass.cpp \

HEADERS += \
    testmyclass.h

