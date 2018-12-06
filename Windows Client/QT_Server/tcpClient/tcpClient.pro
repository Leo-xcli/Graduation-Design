#-------------------------------------------------
#
# Project created by QtCreator 2017-03-20T16:42:26
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = tcpClient
TEMPLATE = app


SOURCES += main.cpp\
        client.cpp \
    qcustomplot.cpp \
    my_thread.cpp

HEADERS  += client.h \
    qcustomplot.h \
    my_thread.h

FORMS    += client.ui
