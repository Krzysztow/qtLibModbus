#-------------------------------------------------
#
# Project created by QtCreator 2013-07-02T14:27:22
#
#-------------------------------------------------

QT += network#just for QHostAddress

QTMB_PREFIX = ./qtLibModbus
QTMB_CONFIG = mbconfig.pri

SOURCES += \
    $${QTMB_PREFIX}/mbconnection.cpp \
    $${QTMB_PREFIX}/mbthreadedconnmanager.cpp

HEADERS += \
    $${QTMB_PREFIX}/mbconnection.h \
    $${QTMB_PREFIX}/mbthreadedconnmanager.h

INCLUDEPATH += $${QTMB_PREFIX}

hasConfig = false
exists($${QTMB_CONFIG}) {
    message("QTMB: $${QTMB_CONFIG}")
    include($${QTMB_CONFIG})
    hasConfig = true
} else {
    exists(../$${QTMB_CONFIG}) {
        message("QTMB: " ../$${QTMB_CONFIG})
        include(../$${QTMB_CONFIG})
        hasConfig = true
    }
}

#when libmodbus is installed in default dir, headers are located in modbus folder, libs in standard location
contains(hasConfig, true) {
    message( "QTMB: Had overwritting config file." )
} else {
    message("QTMB: no config overwrites. Assuming defaults.")
    INCLUDEPATH += modbus
    LIBS += -lmodbus
}
