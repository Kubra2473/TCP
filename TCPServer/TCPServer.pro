#-------------------------------------------------
#
# Project created by QtCreator 2016-04-22T16:31:11 沙振宇
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TCPServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tcpsever.cpp

HEADERS  += mainwindow.h \
    tcpsever.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../Desktop/cryptopp870/release/ -lcryptopp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../Desktop/cryptopp870/debug/ -lcryptopp
else:unix: LIBS += -L$$PWD/../../../Desktop/cryptopp870/ -lcryptopp

INCLUDEPATH += $$PWD/../../../Desktop/cryptopp870
DEPENDPATH += $$PWD/../../../Desktop/cryptopp870
