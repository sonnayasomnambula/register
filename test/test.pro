QT += gui widgets

CONFIG += c++17 console
CONFIG -= app_bundle

GOOGLETEST_DIR = src/google
include(gtest_dependency.pri)

SOURCES += \
    ../app/src/numberedit.cpp \
    src/keyboard.cpp \
    src/tst_numberedit.cpp

HEADERS += \
    ../app/src/numberedit.h \
    src/keyboard.h

INCLUDEPATH += \
    src \
    ../app/src
