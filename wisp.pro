# #####################################################################
# Automatically generated by qmake (2.01a) ven. 15. mai 14:18:29 2009
# #####################################################################
TEMPLATE = app
TARGET = 
DEPENDPATH += . \
    interface
INCLUDEPATH += . \
    interface
QT += webkit

# Input
HEADERS += comms.h \
    interface/contact.h \
    interface/mainwindow.h \
    interface/chatbox.h \
    interface/connectbox.h
SOURCES += comms.cpp \
    interface/contact.cpp \
    main.cpp \
    interface/mainwindow.cpp \
    interface/chatbox.cpp \
    interface/connectbox.cpp
win32:LIBS += libgloox
else:LIBS += -L/usr/local/lib -lgloox
RESOURCES += wisp.qrc
