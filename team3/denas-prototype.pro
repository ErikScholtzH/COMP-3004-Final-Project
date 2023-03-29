#-------------------------------------------------
#
# Project created by QtCreator 2021-03-10T20:15:39
#
#-------------------------------------------------
QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = denas-prototype
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

source_dir = src
forms_dir = ui
resources_dir = res

QMAKE_CLEAN += denas.db

SOURCES += \
        $${source_dir}/dbmanager.cpp \
        $${source_dir}/main.cpp \
        $${source_dir}/mainwindow.cpp \
        $${source_dir}/menu.cpp \
        $${source_dir}/profile.cpp \
        $${source_dir}/record.cpp \
        $${source_dir}/therapy.cpp

HEADERS += \
        $${source_dir}/dbmanager.h \
        $${source_dir}/mainwindow.h \
        $${source_dir}/menu.h \
        $${source_dir}/profile.h \
        $${source_dir}/record.h \
        $${source_dir}/therapy.h

FORMS += \
        $${forms_dir}/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    $${resources_dir}/denas.qrc

OBJECTS_DIR = .qt/obj
MOC_DIR = .qt/moc
UI_DIR = .qt/ui
RCC_DIR = .qt/resources
DESTDIR = .
