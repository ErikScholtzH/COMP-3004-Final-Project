QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    breathpacemeter.cpp \
    coherenceled.cpp \
    device.cpp \
    heartmeasuringelectrodes.cpp \
    main.cpp \
    mainwindow.cpp \
    poc.cpp \
    sessionhistory.cpp \
    user.cpp

HEADERS += \
    breathpacemeter.h \
    coherenceled.h \
    device.h \
    heartmeasuringelectrodes.h \
    mainwindow.h \
    poc.h \
    sessionhistory.h \
    user.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    FinalAss_en_CA.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    MyResources.qrc