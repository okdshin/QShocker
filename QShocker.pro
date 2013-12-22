#-------------------------------------------------
#
# Project created by QtCreator 2013-12-22T19:05:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QShocker
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11
LIBS += -lboost_system -lopencv_highgui -lopencv_core

SOURCES += main.cpp\
        mainwindow.cpp \
    sound_player_thread.cpp

HEADERS  += mainwindow.h \
    Shocker/shocker.h \
    sound_player_thread.h

FORMS    += mainwindow.ui
