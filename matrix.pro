QT += core xml
QT -= gui

CONFIG += c++11 console

TARGET = matrix
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    Neuron.cpp \
    stdafx.cpp \
    NeuroProcess.cpp \
    MatrixClass.cpp \
    MatrixWork.cpp

HEADERS += \
    Neuron.h \
    stdafx.h \
    synaps.h \
    MatrixClass.h \
    MatrixWork.h \
    NeuroProcess.h
