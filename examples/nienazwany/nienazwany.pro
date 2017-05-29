#-------------------------------------------------
#
# Project created by QtCreator 2017-05-22T13:02:15
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nienazwany
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

DESTDIR = "$$PWD/bin"

win32-g++{
QMAKE_LFLAGS += -static-libgcc
QMAKE_LFLAGS += -static-libstdc++
QMAKE_LFLAGS += -static
}

# OpenNN library

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../opennn/release/ -lopennn
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../opennn/debug/ -lopennn
else:unix: LIBS += -L$$OUT_PWD/../../opennn/ -lopennn

INCLUDEPATH += $$PWD/../../opennn
DEPENDPATH += $$PWD/../../opennn

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../opennn/release/libopennn.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../opennn/debug/libopennn.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../opennn/release/opennn.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../opennn/debug/opennn.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../opennn/libopennn.a

# Tiny XML 2 library

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../tinyxml2/release/ -ltinyxml2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../tinyxml2/debug/ -ltinyxml2
else:unix: LIBS += -L$$OUT_PWD/../../tinyxml2/ -ltinyxml2

INCLUDEPATH += $$PWD/../../tinyxml2
DEPENDPATH += $$PWD/../../tinyxml2

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../tinyxml2/release/libtinyxml2.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../tinyxml2/debug/libtinyxml2.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../tinyxml2/release/tinyxml2.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../tinyxml2/debug/tinyxml2.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../tinyxml2/libtinyxml2.a

# OpenMP library
!win32{
QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp
}

mac{
INCLUDEPATH += /usr/local/Cellar/libiomp/20150701/include/libiomp
LIBS += -L/usr/local/Cellar/libiomp/20150701/lib -liomp5
}

# MPI include
#include(../../mpi.pri)

# CUDA include
#include(../../cuda.pri)

