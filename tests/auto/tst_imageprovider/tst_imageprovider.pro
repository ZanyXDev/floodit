QT += testlib concurrent
TEMPLATE = app

TARGET = io.github.zanyxdev.floodit.tst_imageprovider

CONFIG += console qt
CONFIG += testcase
CONFIG += no_testcase_installs
CONFIG -= app_bundle


INCLUDEPATH += ../../../app/
include(../../../app/app.pri)

HEADERS += \
                        tst_imageprovider.h
SOURCES +=  \                       
                        main.cpp \
		    tst_imageprovider.cpp
