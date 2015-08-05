#-------------------------------------------------
#
# Project created by QtCreator 2015-07-01T12:55:36
#
#-------------------------------------------------

QT       += core network testlib

QT       -= gui

TARGET = TCP_minimal_example
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp\
        client.cpp \
        server.cpp \
    controller.cpp

HEADERS += client.h \
        server.h \
    controller.h
