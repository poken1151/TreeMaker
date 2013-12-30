#-------------------------------------------------
#
# Project created by QtCreator 2013-03-18T15:27:50
#
#-------------------------------------------------

QT       += core gui opengl widgets
#CONFIG   += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TreeMaker_T1
TEMPLATE = app


SOURCES +=  sources/window.cpp \
            sources/main.cpp \
            sources/glwidget.cpp \
            sources/camera.cpp \
    sources/branch.cpp \
    sources/leafCloud.cpp \
    sources/ray.cpp \
    sources/octnode.cpp \
    sources/aabb.cpp \
    sources/object.cpp \
    sources/ellipsoid.cpp

HEADERS  += sources/glwidget.h \
            sources/window.h \
            sources/camera.h \
    sources/vecmath.h \
    sources/branch.h \
    sources/leafCloud.h \
    sources/ray.h \
    sources/octnode.h \
    sources/aabb.h \
    sources/quaternion.h \
    sources/object.h \
    sources/ellipsoid.h

FORMS    += window.ui

