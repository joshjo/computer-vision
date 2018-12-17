#-------------------------------------------------
#
# Project created by QtCreator 2018-12-11T08:53:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CalibracionQt
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

#INCLUDEPATH += /usr/local/include/opencv4
#LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_videoio -lopencv_flann
CONFIG += link_pkgconfig
PKGCONFIG += opencv

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    calibracion.cpp \
    data.cpp

#OPEN MP
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

HEADERS += \
        mainwindow.h \
    calibracion.h \
    processCircles.hpp \
    data.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
