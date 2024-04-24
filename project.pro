TARGET = threads.exe

QT -= core gui concurrent
TEMPLATE = app
CONFIG -= qt 
CONFIG += c++20 cmdline

# POSIX thread library needed in linux
LIBS += -lpthread

SOURCES += \
    main.cpp 

HEADERS += \
    config.hh 

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
