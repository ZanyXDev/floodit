!versionAtLeast(QT_VERSION, 5.15.0):error("Requires Qt version 5.15.0 or greater.")

TEMPLATE +=app
TARGET = FloodIt

QT       += core qml quick quickcontrols2 multimedia svg concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG(release,debug|release):CONFIG += bump_version
CONFIG += c++17
CONFIG += resources_big
CONFIG(release,debug|release):CONFIG += qtquickcompiler # Qt Quick compiler
CONFIG(debug,debug|release):CONFIG += qml_debug  # Add qml_debug
CONFIG += qmltypes

DEFINES += VERSION_STR=\\\"$$cat(version.txt)\\\"
DEFINES += PACKAGE_NAME_STR=\\\"$$cat(package_name.txt)\\\"
DEFINES += ACTIVITY_NAME_STR=\\\"$$cat(activity_name.txt)\\\"

DEFINES += QT_DEPRECATED_WARNINGS

# QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII
#don't use precompiled headers https://www.kdab.com/beware-of-qt-module-wide-includes/

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
        src/boardgenerator.h \
        src/boardmodel.h \
        src/cell_item.h \
        src/datamanager.h \
        src/hal.h  \
        src/imageprovider.h \
        src/palette.h

SOURCES += \
            src/boardgenerator.cpp \
            src/boardmodel.cpp \
            src/datamanager.cpp \
            src/imageprovider.cpp \
            src/main.cpp \
            src/hal.cpp  \
            src/palette.cpp

RESOURCES += \
        images.qrc \
        qml.qrc \
        fonts.qrc
#js.qrc \
#        sounds.qrc \
#	i18n.qrc

#Translate
TRANSLATIONS = res/i18n/$${TARGET}_en.ts \
               res/i18n/$${TARGET}_ru.ts

lupdate_only {
    SOURCES +=  \
        res/qml/*.qml \
        res/qml/common/*.qml \
        res/js/*.js
}

#Мне удалось решить  проблему автоматической регистрации QML_ELEMENT,
#добавив мою исходную папку в файл .pro:
#Это немного нелогично, так как мне никогда раньше не приходилось
#добавлять это для сборки проекта Qt, но, эй, это работает.
INCLUDEPATH += src

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH = $$PWD/res/qml

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $$PWD/res/qml
QML2_IMPORT_PATH = $$PWD/res/qml

QML_IMPORT_NAME = io.github.zanyxdev.floodit
QML_IMPORT_MAJOR_VERSION = 1


!build_pass:!debug_and_release*: CONFIG -= add_source_task

contains(CONFIG, add_source_task) {
    message("Adding source.zip for distribution. See more https://raymii.org/s/blog/Existing_GPL_software_for_sale.html")
    system(rm -f $$OUT_PWD/source.zip && cd $$PWD && zip -r $$OUT_PWD/source.zip . -i \*.cpp \*.h \*.qml \*.qrc \*.txt \*.pro)
    #system( create sourceqrc)
    RESOURCES += source.qrc
}

!build_pass:!debug*: CONFIG = memory_leaks_task

contains(CONFIG, memory_leaks_task) {
    message("Enable memory leaks task")
    # use for find segfault memory leaks
    QMAKE_CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer
    LIBS += -fsanitize=address
    #end sanitize
}

bump_version{
    message("Bump or update App version")
    system($$PWD/tools/ci/bump_ver.sh)
}

DISTFILES += \
    README.md \
    activity_name.txt \   
    package_name.txt \
    res/fonts/COPYRIGHT.txt \
    res/fonts/LICENSE \
    res/fonts/LICENSE.txt \
    version.txt

message( Config: $$CONFIG )
message( Libs: $$LIBS )
message( Defines: $$DEFINES )
message( Resources: $$RESOURCES )
message( Object dir: $$OBJECTS_DIR )
unix: message( CXX flags: $$QMAKE_CXXFLAGS )
message( QT Plugins: $$QTPLUGIN )

