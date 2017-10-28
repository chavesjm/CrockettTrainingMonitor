#-------------------------------------------------
#
# Project created by QtCreator 2017-09-08T22:24:41
#
#-------------------------------------------------

QT       += core gui serialport opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MPU9250Test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        objectgl.cpp \
    datawidget.cpp

HEADERS  += mainwindow.h \
            objectgl.h \
    datawidget.h

FORMS    += mainwindow.ui \
    datawidget.ui
