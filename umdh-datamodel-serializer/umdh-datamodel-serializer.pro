QT -= gui

TARGET = umdh-datamodel-serializer
TEMPLATE = lib
DEFINES += UMDH_DATAMODEL_SERIALIZER_LIBRARY

CONFIG -= qt
CONFIG += c++14

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
    impl/cforwardparser.cpp \
    impl/cloadedmoduleserializer.cpp \
    impl/clogentryserializer.cpp \
    impl/creportserializer.cpp \
    impl/cserializerfactory.cpp \
    impl/cserializerfactorycreator.cpp \
    impl/csettingsserializer.cpp \
    impl/cstacktraceframeserializer.cpp \
    impl/ctotalincreaseserializer.cpp \
    impl/serializerfactorycreatorprovider.cpp \
    impl/utils.cpp

HEADERS += \
    impl/cforwardparser.h \
    impl/cloadedmoduleserializer.h \
    impl/clogentryserializer.h \
    impl/creportserializer.h \
    impl/cserializerfactory.h \
    impl/cserializerfactorycreator.h \
    impl/csettingsserializer.h \
    impl/cstacktraceframeserializer.h \
    impl/ctotalincreaseserializer.h \
    impl/utils.h \
    interface/serializerfactorycreatorprovider.h \
    interface/umdh.datamodel.serializer.global.h

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


