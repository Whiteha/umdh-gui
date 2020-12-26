TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = umdh-datamodel \
          umdh-datamodel-serializer \
          umdh-logic \
          umdh-gui \

umdh-gui.depends = umdh-datamodel
umdh-gui.depends = umdh-datamodel-serializer
umdh-gui.depends = umdh-logic
