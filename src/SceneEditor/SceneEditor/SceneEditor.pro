
TEMPLATE = app
TARGET = SceneEditor
QT += core network widgets gui
CONFIG += c++11
INCLUDEPATH += ./zlib ./qwt ./quazip ./Scene ./Tools ./MainUI .
PRECOMPILED_HEADER = PreHeader.h
DEPENDPATH += .
QT           += xml
include(SceneEditor.pri)
