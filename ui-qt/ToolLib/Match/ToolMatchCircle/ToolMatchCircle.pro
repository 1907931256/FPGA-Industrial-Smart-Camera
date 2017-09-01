#-------------------------------------------------
#
# Project created by QtCreator 2017-05-15T10:19:21
#
#-------------------------------------------------

QT       += widgets

QT       -= gui

TARGET = ToolMatchCircle
TEMPLATE = lib

DEFINES += TOOLMATCHCIRCLE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DESTDIR=../../../bin
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH  += ../../../../include  \
                ../../../../include/function \
                ../../../../include/function/sszn  \
                ../../../../include/function/third_party \
                ../../../../include/lfpga_include \
                ../../../../include/lzynq_include \
                ../../../../include/task
SOURCES += ToolMatchCircle.cpp \
    CircleWidget.cpp \
    FWMatchCircleLabel.cpp \
    LibCircleInterface.cpp \
    ResolveRunData.cpp

HEADERS += ToolMatchCircle.h\
        toolmatchcircle_global.h \
    CircleWidget.h \
    FWMatchCircleLabel.h \
    LibCircleInterface.h \
    ResolveRunData.h \
    ../../BasicDefine.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    CircleWidget.ui
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable
