QT       += core gui widgets

CONFIG += c++17

SOURCES += \
    src/basedialog.cpp \
    src/numberedit.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/selectiondialog.cpp

HEADERS += \
    src/basedialog.h \
    src/numberedit.h \
    src/mainwindow.h \
    src/selectiondialog.h

FORMS += \
    src/basedialog.ui

INCLUDEPATH += \
    src

RESOURCES += \
    resources.qrc
