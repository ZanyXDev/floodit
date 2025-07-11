QT += testlib

TEMPLATE = app

TARGET = io.github.zanyxdev.floodit.tst_myclass

CONFIG += console qt
CONFIG += testcase
CONFIG += no_testcase_installs

INCLUDEPATH += ../../../app/
include(../../../app/app.pri)

SOURCES += \
    main.cpp \
    testmyclass.cpp \

HEADERS += \
    testmyclass.h

