TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp
LIBS += /usr/lib/x86_64-linux-gnu/libboost_filesystem.so /usr/lib/x86_64-linux-gnu/libboost_system.so /usr/lib/x86_64-linux-gnu/libpthread.so
