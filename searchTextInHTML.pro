QT += quick network
CONFIG += c++14

SOURCES += main.cpp \
    searchengine.cpp \
    downloader.cpp \
    resultsproxy.cpp \
    scannerinline.cpp \
    scanner.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    searchengine.h \
    downloader.h \
    resultsproxy.h \
    scannerinline.h \
    scanner.h
