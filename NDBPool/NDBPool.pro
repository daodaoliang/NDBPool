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
CONFIG += c++11

# 版本号
win32{
    RC_FILE += ./NDBPool_resource.rc
}
unix{
    VERSION = 1.0.7
}

# 源码
include(./NDBPool_src.pri)
INCLUDEPATH += ./inc/ \

# 输出定义
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

unix{
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

# 打印信息
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])
message(the NDBPool will create in folder: $$target_path)
