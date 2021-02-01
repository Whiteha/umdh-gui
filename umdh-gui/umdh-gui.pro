QT += core gui

TARGET = umdh-gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14 ordered

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
    src/impl/main.cpp \
    src/impl/mainwindow.cpp \
    src/impl/reportwidget.cpp \
    src/impl/reportviewwidget.cpp \
    src/impl/umdhwidget.cpp

HEADERS += \
    src/impl/reportwidget.h \
    src/impl/reportviewwidget.h \
    src/impl/umdhwidget.h \
    src/interface/customuniqueptr.h \
    src/interface/guiutils.h \
    src/interface/idatamodel/iclonable.h \
    src/interface/idatamodel/idataobject.h \
    src/interface/idatamodel/iloadedmodule.h \
    src/interface/idatamodel/ilogentry.h \
    src/interface/idatamodel/iobjectfactory.h \
    src/interface/idatamodel/ireport.h \
    src/interface/idatamodel/iserializable.h \
    src/interface/idatamodel/isettings.h \
    src/interface/idatamodel/istacktraceframe.h \
    src/interface/idatamodel/itotalincrease.h \
    src/interface/idatamodelserializer/iserializer.h \
    src/interface/idatamodelserializer/iserializerfactory.h \
    src/interface/idatamodelserializer/iserializerfactorycreator.h \
    src/interface/ilogic/ifilterfactory.h \
    src/interface/ilogic/ifilterfactorycreator.h \
    src/interface/ilogic/ileakedmorethanbytesfilter.h \
    src/interface/ilogic/ileakedmorethancountfilter.h \
    src/interface/ilogic/ireportfilter.h \
    src/interface/ilogic/ireportfilterwithoutpattern.h \
    src/interface/ilogic/ireportfilterwithpattern.h \
    src/interface/ilogic/ireportmanager.h \
    src/interface/ilogic/ireportsorter.h \
    src/interface/ilogic/isorterfactory.h \
    src/interface/ilogic/isorterfactorycreator.h \
    src/interface/ilogic/iumdh.h \
    src/interface/ilogic/iumdhcreator.h \
    src/interface/metamodel.h \
    src/impl/mainwindow.h

FORMS += \
    ui/reportwidget.ui \
    ui/umdhwidget.ui \
    ui/reportviewwidget.ui \
    ui/mainwindow.ui

INCLUDEPATH += src/interface

include($$PWD/../umdh-datamodel-serializer/umdh-datamodel-serializer.pri)
include($$PWD/../umdh-datamodel/umdh-datamodel.pri)
include($$PWD/../umdh-logic/umdh-logic.pri)


PATH_TO_TARGET_OUTPUT = ""
CONFIG(release, debug|release) {
  message( $$TARGET" - release" )
  PATH_TO_TARGET_OUTPUT = $$OUT_PWD/release/$$TARGET".exe"
}
CONFIG(debug, debug|release) {
  message( $$TARGET" - debug" )
  PATH_TO_TARGET_OUTPUT = $$OUT_PWD/debug/$$TARGET".exe"
}

LIBS += -lPsapi

PATH_TO_DEPLOY_TARGET_RAW = $$PWD/../bin
PATH_TO_DEPLOY_TARGET = \"$$PATH_TO_DEPLOY_TARGET_RAW\"

QMAKE_POST_LINK = copy /Y $$shell_path($$PATH_TO_TARGET_OUTPUT) $$shell_path($$PATH_TO_DEPLOY_TARGET) && $(Qt5_Dir)\bin\windeployqt.exe $$PATH_TO_DEPLOY_TARGET_RAW\umdh-gui.exe
#message($(Qt5_Dir)\bin\windeployqt.exe $$PATH_TO_DEPLOY_TARGET_RAW\umdh-gui.exe)
