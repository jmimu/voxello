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
    octreecell.cpp \
    kv6format.cpp \
    ogldraw.cpp

HEADERS += \
    voxobj.h \
    octreecell.h \
    kv6format.h \
    ogldraw.h


LIBS += -lSDL -lGL -lGLU
