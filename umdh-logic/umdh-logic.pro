QT -= gui

TARGET = umdh-logic
TEMPLATE = lib
DEFINES += UMDH_LOGIC_LIBRARY

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
    impl/cbyallocatedbytessorter.cpp \
    impl/cbyallocationcountsorter.cpp \
    impl/cdefaultfilter.cpp \
    impl/cdefaultsorter.cpp \
    impl/cfilterfactory.cpp \
    impl/cfilterfactorycreator.cpp \
    impl/cleakedmorethanbytesfilter.cpp \
    impl/cleakedmorethancountfilter.cpp \
    impl/creportmanager.cpp \
    impl/creportmanagercreator.cpp \
    impl/csorterfactory.cpp \
    impl/csorterfactorycreator.cpp \
    impl/cumdh.cpp \
    impl/cumdhcreator.cpp \
    impl/cwithoutpatternfilter.cpp \
    impl/cwithoutsystemmodulesfilter.cpp \
    impl/cwithpatternfilter.cpp \
    impl/cwithsourcesfilter.cpp \
    impl/cwithsymbolsfilter.cpp \
    impl/filterfactorycreatorprovider.cpp \
    impl/reportmanagercreatorprovider.cpp \
    impl/sorterfactorycreatorprovider.cpp \
    impl/umdhcreatorprovider.cpp


HEADERS += \
    impl/cbyallocatedbytessorter.h \
    impl/cbyallocationcountsorter.h \
    impl/cdefaultfilter.h \
    impl/cdefaultsorter.h \
    impl/cfilterfactory.h \
    impl/cfilterfactorycreator.h \
    impl/cleakedmorethanbytesfilter.h \
    impl/cleakedmorethancountfilter.h \
    impl/creportmanager.h \
    impl/creportmanagercreator.h \
    impl/csorterfactory.h \
    impl/csorterfactorycreator.h \
    impl/cumdh.h \
    impl/cumdhcreator.h \
    impl/cwithoutpatternfilter.h \
    impl/cwithoutsystemmodulesfilter.h \
    impl/cwithpatternfilter.h \
    impl/cwithsourcesfilter.h \
    impl/cwithsymbolsfilter.h \
    interface/filterfactorycreatorprovider.h \
    interface/reportmanagercreatorprovider.h \
    interface/sorterfactorycreatorprovider.h \
    interface/umdh.logic.global.h \
    interface/umdhcreatorprovider.h

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
