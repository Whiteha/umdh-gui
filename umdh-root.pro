TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = umdh-datamodel \
          umdh-datamodel-serializer \
          umdh-logic \
          umdh-gui \

umdh-gui.depends = umdh-datamodel
umdh-gui.depends = umdh-datamodel-serializer
umdh-gui.depends = umdh-logic

umdh-gui.commands = \"C:\\Qt\\5.15.2\\msvc2019_64\\bin\\windeployqt.exe D:\\umdh-gui\\bin\\umdh-gui.exe\"
