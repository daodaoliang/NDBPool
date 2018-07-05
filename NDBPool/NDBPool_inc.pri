#-------------------------------------------------
#
# Project created by daodaoliang 2016-07-11T11:08:28
#
#-------------------------------------------------

INCLUDEPATH += $$PWD\inc

win32{
    LIBS += -L$$PWD/../bin/ -lNDBPool
    DEPENDPATH += $$PWD/../bin
}

unix{
    LIBS += -L$$PWD/../bin/ -lNDBPool
    DEPENDPATH += $$PWD/../bin
}


