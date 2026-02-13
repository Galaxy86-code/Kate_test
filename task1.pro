#-------------------------------------------------
#
# Project created by QtCreator 2026-02-04T14:12:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = task1
TEMPLATE = app

QT += widgets
QT += svg

CONFIG += qwt


SOURCES += main.cpp\
        mainwindow.cpp \
    form1.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    form1.h \
    qcustomplot.h
FORMS    += mainwindow.ui \
    form1.ui

INCLUDEPATH += C:\Qwt-6.1.4\include
#LIBS += -LC:\Qwt-6.1.4\lib -lqwtd

INCLUDEPATH += $${QWT_PATH}/src
LIBS += -L$$PWD -llibqwt_static

