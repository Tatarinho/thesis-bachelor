#-------------------------------------------------
#
# Project created by QtCreator 2015-02-16T21:32:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EKG
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    background.cpp \
    plotter.cpp \
    scrollarea.cpp \
    waveform.cpp

HEADERS  += mainwindow.h \
    background.h \
    plotter.h \
    scrollarea.h \
    waveform.h

FORMS    += mainwindow.ui \
    background.ui \
    scrollarea.ui

RESOURCES += \
    imgResources.qrc

DISTFILES += \
    EKG.pro.user
