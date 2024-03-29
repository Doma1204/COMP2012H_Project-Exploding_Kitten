#-------------------------------------------------
#
# Project created by QtCreator 2018-11-12T20:06:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = COMP2012H-Exploding-Kitten
TEMPLATE = app

INCLUDEPATH += ui
INCLUDEPATH += src
INCLUDEPATH += server

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11



SOURCES += \
        ui/mainwindow.cpp \
    src/gamelogic.cpp \
    server/server.cpp \
    server/client.cpp\
    main.cpp \
    server/serverworker.cpp \
    ui/gamewindow.cpp \
    ui/notifywindow.cpp

HEADERS += \
        ui/mainwindow.h \
    src/gamelogic.h \
    server/server.h \
    server/client.h \
    server/serverworker.h \
    ui/gamewindow.h \
    ui/notifywindow.h

FORMS += \
        ui/mainwindow.ui \
    ui/game_window.ui \
    ui/notifywindow.ui

 QT           += network widgets

requires(qtConfig(combobox))

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
