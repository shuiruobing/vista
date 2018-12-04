#nvdia

INCLUDEPATH += $$PWD/../ \
               $$PWD/include/

DEPENDPATH += $$PWD/../ \
              $$PWD/include/
HEADERS += \
    $$PWD/include/cuda.h    \
    $$PWD/include/dynlink_cuda.h

SOURCES += \
    $$PWD/src/dynlink_cuda_cuda.cpp
