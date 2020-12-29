isEmpty(GOOGLETEST_DIR):error("please set GOOGLETEST_DIR as environment variable or qmake variable")

requires(exists($$GOOGLETEST_DIR))

!exists($$GOOGLETEST_DIR):message("No googletest src dir found - set GOOGLETEST_DIR to enable.")

DEFINES += \
    GTEST_LANG_CXX11

INCLUDEPATH *= \
    $$GOOGLETEST_DIR \
    $$GOOGLETEST_DIR/gmock \
    $$GOOGLETEST_DIR/gtest

SOURCES += \
    $$GOOGLETEST_DIR/gmock-gtest-all.cc \
    $$GOOGLETEST_DIR/gmock_main.cc

HEADERS += \
    $$GOOGLETEST_DIR/gmock/gmock.h \
    $$GOOGLETEST_DIR/gtest/gtest.h
