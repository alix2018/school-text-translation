#-------------------------------------------------
#
# Project created by QtCreator 2017-04-26T15:51:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network xml xmlpatterns

TARGET = BDM
TEMPLATE = app

INCLUDEPATH += /usr/include/opencv
LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_text

SOURCES += main.cpp\
        mainwindow.cpp \
        BingTranslator.cpp

HEADERS  += mainwindow.h \
    readtranslateandwrite.h \
    BingTranslator.h

FORMS    += mainwindow.ui
