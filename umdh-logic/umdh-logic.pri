INCLUDEPATH += $$PWD/interface

{
PATH_TO_TARGET_OUTPUT = ""
CONFIG(release, debug|release) {
  PATH_TO_TARGET_OUTPUT = $$OUT_PWD/../umdh-logic/release/
}
CONFIG(debug, debug|release) {
  PATH_TO_TARGET_OUTPUT = $$OUT_PWD/../umdh-logic/debug/
}

LIBS += -L$$PATH_TO_TARGET_OUTPUT -lumdh-logic
}
