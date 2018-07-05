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

win32{
    DESTDIR = $$PWD/../bin
    MOC_DIR = $$PWD/build_/moc
    RCC_DIR = $$PWD/build_/rcc
    OBJECTS_DIR = $$PWD/build_/obj
}

LIBS += -L$$PWD/../bin/ -lNDBPool
DEPENDPATH += $$PWD/../bin

# import dll file
include($$PWD/../NDBPool/NDBPool_inc.pri)

