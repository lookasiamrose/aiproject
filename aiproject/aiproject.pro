###################################################################################################
#                                                                                                 #
#   OpenNN: Open Neural Networks Library                                                          #
#   www.artelnics.com/opennn                                                                      #
#                                                                                                 #
#   B L A N K   P R O J E C T                                                                     #
#                                                                                                 #
#   Roberto Lopez                                                                                 #
#   Artelnics - Making intelligent use of data                                                    #
#   robertolopez@artelnics.com                                                                    #
#                                                                                                 #
###################################################################################################

QT += core gui
QT += widgets
QT += webenginewidgets
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
#CONFIG += console
CONFIG += c++11

mac{
    CONFIG-=app_bundle
}

TARGET = aiproject

DESTDIR = "$$PWD/bin"

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h \
    model.h \
    graphicsmatrixitem.h \
    graphics_view_zoom.h

SOURCES += main.cpp \
    mainwindow.cpp \
    model.cpp \
    graphicsmatrixitem.cpp \
    graphics_view_zoom.cpp

win32-g++{
QMAKE_LFLAGS += -static-libgcc
QMAKE_LFLAGS += -static-libstdc++
QMAKE_LFLAGS += -static
}

# OpenNN library


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../opennn/release/ -lopennn
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../opennn/debug/ -lopennn
else:unix: LIBS += -L$$OUT_PWD/../opennn/ -lopennn

INCLUDEPATH += $$PWD/../opennn
DEPENDPATH += $$PWD/../opennn

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../opennn/release/libopennn.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../opennn/debug/libopennn.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../opennn/release/opennn.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../opennn/debug/opennn.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../opennn/libopennn.a

# Tiny XML 2 library

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../tinyxml2/release/ -ltinyxml2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../tinyxml2/debug/ -ltinyxml2
else:unix: LIBS += -L$$OUT_PWD/../tinyxml2/ -ltinyxml2

INCLUDEPATH += $$PWD/../tinyxml2
DEPENDPATH += $$PWD/../tinyxml2

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../tinyxml2/release/libtinyxml2.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../tinyxml2/debug/libtinyxml2.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../tinyxml2/release/tinyxml2.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../tinyxml2/debug/tinyxml2.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../tinyxml2/libtinyxml2.a

# OpenMP library
win32:!win32-g++{
QMAKE_CXXFLAGS += -openmp
QMAKE_LFLAGS   += -openmp
}

!win32{
QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp
}

mac{
INCLUDEPATH += /usr/local/Cellar/libiomp/20150701/include/libiomp
LIBS += -L/usr/local/Cellar/libiomp/20150701/lib -liomp5
}

# MPI libraries
#include(../mpi.pri)

# CUDA libraries
#include(../cuda.pri)
