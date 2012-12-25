RESOURCES = connoisseur.qrc

SOURCES += main.cpp \
    systemconnection.cpp \
    systemobject.cpp \
    objectflowmodel.cpp \
    contenttype.cpp \
    connoisseurutils.cpp \
    objectflow.cpp \
    serverchooserquickview.cpp

HEADERS += \
    systemconnection.h \
    systemobject.h \
    objectflowmodel.h \
    contenttype.h \
    connoisseurutils.h \
    objectflow.h \
    serverchooserquickview.h

QMAKE_CXXFLAGS += -Wall

CONFIG += c++11

QT += network quick

macx {
    QMAKE_CXX = "/opt/local/bin/g++-mp-4.7"
}
