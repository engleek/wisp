# -------------------------------------------------
# Project created by QtCreator 2009-05-05T01:08:03
# -------------------------------------------------
QT += network
TARGET = wisp
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    comms.cpp \
    interface/identrequest.cpp
HEADERS += mainwindow.h \
    comms.h \
    interface/identrequest.h
win32:LIBS += libgloox
else:LIBS += -L/usr/local/lib \
    -lgloox
RESOURCES += wisp.qrc
FORMS += 
