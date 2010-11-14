#-------------------------------------------------
#
# Project created by QtCreator 2010-11-07T09:13:40
#
#-------------------------------------------------

#QT       += core

QT       -= gui

TARGET = voxello
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    voxobj.cpp \
    kv6format.cpp \
    ogldraw.cpp \
    compress_tools.cpp

HEADERS += \
    voxobj.h \
    kv6format.h \
    ogldraw.h \
    compress_tools.h


LIBS += -lSDL -lGL -lGLU

OTHER_FILES += \
    TODO.txt
