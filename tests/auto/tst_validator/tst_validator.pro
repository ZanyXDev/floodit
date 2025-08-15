QT += testlib
TEMPLATE = app

TARGET = io.github.zanyxdev.floodit.tst_validator

CONFIG += console qt
CONFIG += testcase
CONFIG += no_testcase_installs
CONFIG -= app_bundle


INCLUDEPATH += ../../../app/
include(../../../app/validator.pri)

HEADERS += \
    tst_validator.h
SOURCES +=  \
    main.cpp \
    tst_validator.cpp

# Force C++17 if available
contains(QT_CONFIG, c++1z): CONFIG += c++1z
# Enable CCache
load(ccache)
