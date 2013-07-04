#-------------------------------------------------
#
# Project created by QtCreator 2013-07-02T14:27:22
#
#-------------------------------------------------

QT       -= gui
QT += network

TARGET = QtLibMobus
TEMPLATE = lib

DEFINES += QTLIBMOBUS_LIBRARY

SOURCES += \
    mbconnection.cpp \
    mbconnectionsmanager.cpp

HEADERS +=\
        qtlibmodbus_global.h \
    mbconnection.h \
    mbconnectionsmanager.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE1A27E8C
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = QtLibMobus.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

#libmodbus files
SOURCES += ./libmodbus/*.c
HEADERS += ./libmodbus/*.h

INCLUDEPATH += ./libmodbus
