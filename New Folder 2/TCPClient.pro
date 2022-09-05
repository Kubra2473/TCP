#-------------------------------------------------
#
# Project created by QtCreator 2016-05-04T16:09:20 沙振宇
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TCPClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tcpclient.cpp

HEADERS  += mainwindow.h \
    tcpclient.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../Desktop/cryptopp870/release/ -lcryptopp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../Desktop/cryptopp870/debug/ -lcryptopp
else:unix: LIBS += -L$$PWD/../../../Desktop/cryptopp870/ -lcryptopp

INCLUDEPATH += $$PWD/../../../Desktop/cryptopp870
DEPENDPATH += $$PWD/../../../Desktop/cryptopp870

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../cryptopp870/release/ -lcryptopp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../cryptopp870/debug/ -lcryptopp
else:unix: LIBS += -L$$PWD/../cryptopp870/ -lcryptopp

INCLUDEPATH += $$PWD/../cryptopp870
DEPENDPATH += $$PWD/../cryptopp870
