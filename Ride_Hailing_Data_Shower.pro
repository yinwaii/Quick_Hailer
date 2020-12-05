QT       += core gui sql charts location quickwidgets

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
    Managers/mapmanager.cpp \
    main.cpp \
    Utilities/database.cpp \
    Utilities/globaldata.cpp \
    Windows/loadwindow.cpp \
    Windows/mainwindow.cpp \
    Tabs/datatab.cpp \
    Tabs/maptab.cpp \
    Tabs/predictiontab.cpp \
    Tabs/querytab.cpp \
    Managers/axismanager.cpp \
    Managers/chartmanager.cpp \
    Managers/seriesmanager.cpp

HEADERS += \
    Managers/mapmanager.h \
    Utilities/database.h \
    Utilities/globaldata.h \
    Windows/loadwindow.h \
    Windows/mainwindow.h \
    Tabs/datatab.h \
    Tabs/maptab.h \
    Tabs/predictiontab.h \
    Tabs/querytab.h \
    Managers/axismanager.h \
    Managers/chartmanager.h \
    Managers/seriesmanager.h

FORMS += \
    Windows/loadwindow.ui \
    Windows/mainwindow.ui \
    Tabs/datatab.ui \
    Tabs/maptab.ui \
    Tabs/predictiontab.ui \
    Tabs/querytab.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Maps/map.qml

RESOURCES += \
    resources.qrc
