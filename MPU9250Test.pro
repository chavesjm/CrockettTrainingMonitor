#-------------------------------------------------
#
# Project created by QtCreator 2017-09-08T22:24:41
#
#-------------------------------------------------

QT       += core gui serialport opengl printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MPU9250Test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        objectgl.cpp \
    datawidget.cpp \
    dataconnector.cpp \
    qcustomplot/qcustomplot.cpp


HEADERS  += mainwindow.h \
            objectgl.h \
    datawidget.h \
    dataconnector.h \
    qcustomplot/qcustomplot.h

FORMS    += mainwindow.ui \
    datawidget.ui
