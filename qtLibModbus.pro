#-------------------------------------------------
#
# Project created by QtCreator 2013-07-02T14:27:22
#
#-------------------------------------------------

QT -= gui
QT += network

TARGET = QtLibModbus
TEMPLATE = lib

DEFINES += QTLIBMODBUS_LIBRARY

SOURCES += \
    mbconnection.cpp \
    mbconnectionsmanager.cpp \
    libmodbus/modbus-tcp.c \
    libmodbus/modbus-rtu.c \
    libmodbus/modbus-data.c \
    libmodbus/modbus.c

HEADERS +=\
    qtlibmodbus_global.h \
    mbconnection.h \
    mbconnectionsmanager.h

#libmodbus files
INCLUDEPATH += ./libmodbus
HEADERS += \
    libmodbus/modbus-version.h \
    libmodbus/modbus-tcp-private.h \
    libmodbus/modbus-tcp.h \
    libmodbus/modbus-rtu-private.h \
    libmodbus/modbus-rtu.h \
    libmodbus/modbus-private.h \
    libmodbus/modbus.h \
    libmodbus/config.h
