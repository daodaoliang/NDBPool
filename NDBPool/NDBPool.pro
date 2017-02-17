#-------------------------------------------------
#
# Project created by daodaoliang 2016-07-11T11:08:28
#
#-------------------------------------------------

QT       += sql
QT       -= gui
TARGET = NDBPool
TEMPLATE = lib
DEFINES += NDBPOOL_LIBRARY

include(./NDBPool_src.pri)

INCLUDEPATH += ./inc/ \

RC_FILE += ./NDBPool_resource.rc

win32{
    CONFIG += debug_and_release
    CONFIG(release, debug|release) {
            target_path = ./build_/dist
        } else {
            target_path = ./build_/debug
        }
        DESTDIR = ../bin
        MOC_DIR = $$target_path/moc
        RCC_DIR = $$target_path/rcc
        OBJECTS_DIR = $$target_path/obj
}
CONFIG += c++11
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])
message(the NDBPool will create in folder: $$target_path)
