contains(QMAKE_TARGET.arch,x86_64) {
    ARCH = x64
}else{
    ARCH = x86
}

INCLUDEPATH += $$PWD/include


win32:{
    LIBS += $$PWD/lib/$${ARCH}/windows/TrstPano.lib \
            $$PWD/merge_dll/MergeImg.lib
}

HEADERS += \
    $$PWD/stitch_nvidia.h \
    $$PWD/stitch_cuda.h
SOURCES += \
    $$PWD/stitch_nvidia.cpp \
    $$PWD/stitch_cuda.cpp

