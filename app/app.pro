QT       += core gui widgets

CONFIG += c++17

SOURCES += \
    src/numberedit.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/selectiondialog.cpp

HEADERS += \
    src/numberedit.h \
    src/mainwindow.h \
    src/selectiondialog.h

FORMS += \
    src/mainwindow.ui \
    src/selectiondialog.ui

INCLUDEPATH += \
    src
