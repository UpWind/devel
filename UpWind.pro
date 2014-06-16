QT += serialport
CONFIG += serialport
QT += widgets



TEMPLATE = subdirs

SUBDIRS = \
        upwind/src/Settings \
        upwind/src/UWCore \
        upwind/src/UWPlugins \
        UpWindNMEASources/PluginSystem/PluginBuilder \
        UpWindNMEASources/UpWindNMEA/PluginController

CONFIG += serialport
