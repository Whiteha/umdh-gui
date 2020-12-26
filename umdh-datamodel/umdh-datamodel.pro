QT -= gui

TARGET = umdh-datamodel
TEMPLATE = lib
DEFINES += UMDH_DATAMODEL_LIBRARY

CONFIG -= qt
CONFIG += c++14 dll

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#QMAKE_CXXFLAGS += /Zi
#QMAKE_LFLAGS += /DEBUG

SOURCES += \
        impl/cloadedmodule.cpp \
        impl/clogentry.cpp \
        impl/cobjectfactory.cpp \
        impl/creport.cpp \
        impl/csettings.cpp \
        impl/cstacktraceframe.cpp \
        impl/ctotalincrease.cpp \
        impl/objectfactorycreator.cpp

HEADERS += \
    impl/cloadedmodule.h \
    impl/clogentry.h \
    impl/cobjectfactory.h \
    impl/creport.h \
    impl/csettings.h \
    impl/cstacktraceframe.h \
    impl/ctotalincrease.h \
    interface/objectfactorycreator.h \
    interface/umdh.datamodel.global.h \

include($$PWD/../umdh-gui/umdh-gui.pri)


PATH_TO_TARGET_OUTPUT = ""
CONFIG(release, debug|release) {
  message( $$TARGET" - release" )
  PATH_TO_TARGET_OUTPUT = $$OUT_PWD/release/$$TARGET".dll"
}
CONFIG(debug, debug|release) {
  message( $$TARGET" - debug" )
  PATH_TO_TARGET_OUTPUT = $$OUT_PWD/debug/$$TARGET".dll"
}

PATH_TO_DEPLOY_TARGET = \"$$PWD/../bin\"

QMAKE_POST_LINK = copy /Y $$shell_path($$PATH_TO_TARGET_OUTPUT) $$shell_path($$PATH_TO_DEPLOY_TARGET)

