#-------------------------------------------------
#
# Project created by QtCreator 2014-12-23T15:32:00
#
#-------------------------------------------------

QT       += core gui
QT       += serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = stepmotor_controller
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    networks.cpp

HEADERS  += mainwindow.h \
    networks.h

FORMS    += mainwindow.ui

