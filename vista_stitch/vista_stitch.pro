QT += core
QT -= gui

CONFIG += c++11

TARGET = vista_stitch
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

contains(QMAKE_TARGET.arch,x86_64) {
    ARCH = x64
}else{
    ARCH = x86
}
win32:{
    OS = win
    LIBS += $$PWD/crashrpt_v.1.4.3/CrashRpt1403.lib
}
unix: OS = unix

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin/$${OS}_$${ARCH}/debug/
} else {
    DESTDIR = $$PWD/../bin/$${OS}_$${ARCH}/release/
}

include(spdlog/spdlog.pri)
include(nvida/nvida.pri)
include(ffmpeg-4.0.2/ffmpeg.pri)
include(codec/codec.pri)
include(stitch/stitch.pri)


SOURCES += main.cpp \
    sys_info.cpp \
    configure.cpp \
    console_read.cpp \
    cmd_flags.cpp \
    panorama.cpp \
    dump_tool.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS QT_MESSAGELOGCONTEXT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    sys_info.h \
    configure.h \
    console_read.h \
    cmd_flags.h \
    panorama.h \
    json.hpp \
    dump_tool.h

INCLUDEPATH += $$PWD

