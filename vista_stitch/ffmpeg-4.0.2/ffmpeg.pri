
contains(QMAKE_TARGET.arch,x86_64) {
    ARCH = x64
}else{
    ARCH = x86
}

win32:{
    LIBS += $$PWD/lib/$${ARCH}/windows/avcodec.lib \
            $$PWD/lib/$${ARCH}/windows/avdevice.lib \
            $$PWD/lib/$${ARCH}/windows/avfilter.lib \
            $$PWD/lib/$${ARCH}/windows/avformat.lib \
            $$PWD/lib/$${ARCH}/windows/avutil.lib \
            $$PWD/lib/$${ARCH}/windows/postproc.lib \
            $$PWD/lib/$${ARCH}/windows/swresample.lib \
            $$PWD/lib/$${ARCH}/windows/swscale.lib
}unix:{
    LIBS += -L$$PWD/lib/$${ARCH}/unix -lavcodec \
            -L$$PWD/lib/$${ARCH}/unix -lavdevice \
            -L$$PWD/lib/$${ARCH}/unix -lavfilter \
            -L$$PWD/lib/$${ARCH}/unix -lavformat \
            -L$$PWD/lib/$${ARCH}/unix -lavutil \
            -L$$PWD/lib/$${ARCH}/unix -lpostproc \
            -L$$PWD/lib/$${ARCH}/unix -lswresample \
            -L$$PWD/lib/$${ARCH}/unix -lswscale
}

INCLUDEPATH += $$PWD/include
