INCLUDEPATH += $$PWD/interface

{
PATH_TO_TARGET_OUTPUT = ""
CONFIG(release, debug|release) {
  PATH_TO_TARGET_OUTPUT = $$OUT_PWD/../umdh-datamodel/release/
}
CONFIG(debug, debug|release) {
  PATH_TO_TARGET_OUTPUT = $$OUT_PWD/../umdh-datamodel/debug/
}

LIBS += -L$$PATH_TO_TARGET_OUTPUT -lumdh-datamodel
}
