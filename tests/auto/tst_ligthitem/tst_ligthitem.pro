CONFIG += warn_on qmltestcase

TARGET = ts_ligthitem

TEMPLATE = app

DISTFILES += \
    tst_lightitem.qml

SOURCES += \
    main.cpp

# Force C++17 if available
contains(QT_CONFIG, c++1z): CONFIG += c++1z
# Enable CCache
load(ccache)
