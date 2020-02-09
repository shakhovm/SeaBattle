#-------------------------------------------------
#
# Project created by QtCreator 2019-06-30T13:10:53
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SeaBattle
TEMPLATE = app

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
        SeaBattle/Field.cpp \
        SeaBattle/game.cpp \
        SeaBattle/player.cpp \
        SeaBattle/ship.cpp \
        extrahandler.cpp \
        image.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        SeaBattle/Field.h \
        SeaBattle/game.h \
        SeaBattle/player.h \
        SeaBattle/ship.h \
        config.h \
        extrahandler.h \
        image.h \
        mainwindow.h

FORMS += \
        mainwindow.ui

RESOURCES +=\
        Images/redCell.png\
        Images/Field.png\
        Images/Cell.png\
        Images/1500.png\
        Images/real_field.png\
        Images/red_cell.png\
        Images/blue_cell.png\
        Images/white_cell.png\
        Images/orange_cell.png


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
