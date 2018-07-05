QT += core
QT += sql
QT -= gui

CONFIG += c++11

TARGET = NDBPool_Example
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    testconnection.cpp \
    threadtest.cpp

HEADERS += \
    testconnection.h \
    threadtest.h

# 输出定义
win32{
    DESTDIR = $$PWD/../bin
    MOC_DIR = $$PWD/build_/moc
    RCC_DIR = $$PWD/build_/rcc
    OBJECTS_DIR = $$PWD/build_/obj
}
unix{
    DESTDIR = $$PWD/../bin
    MOC_DIR = $$PWD/build_/moc
    RCC_DIR = $$PWD/build_/rcc
    OBJECTS_DIR = $$PWD/build_/obj
}

# import dll file
include($$PWD/../NDBPool/NDBPool_inc.pri)

