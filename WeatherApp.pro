QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    src/clientapi.cpp \
    src/forecast.cpp \
    src/location.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/settingsdialog.cpp

HEADERS += \
    include/clientapi.hpp \
    include/forecast.hpp \
    include/location.hpp \
    include/mainwindow.hpp \
    include/settingsdialog.hpp \
    include/weatherdata.hpp

QT += network

RESOURCES += resources/resources.qrc

DISTFILES += \
    resources/icons/sun.png \
    resources/icons/cloud.png \
    resources/icons/rain.png \
    resources/icons/snow.png \
    resources/icons/thunder.png \
    resources/icons/default.png \
    resources/icons/settings.png \
    resources/icons/refresh.png
