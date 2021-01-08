# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = carbudget

DEFINES += QT_DEPRECATED_WARNINGS
QMAKE_CXXFLAGS += -Wall -Wextra -pedantic

QT += quick sql xml
CONFIG += c++11
DEFINES += APP_VERSION=\"\\\"$${VERSION}\\\"\"
QML_IMPORT_PATH += qml

TRANSLATIONS = translations/de_DE.ts \
               translations/fi_FI.ts \
               translations/fr_FR.ts \
               translations/it_IT.ts \
               translations/ru_RU.ts \
               translations/sv_SE.ts

lupdate_only{
    SOURCES += qml-controls/*.qml \
    SOURCES += qml-controls/pages/*.qml \
    SOURCES += qml-controls/jbQuick/Charts/*.qml \
    SOURCES += qml-controls/jbQuick/Charts/*.js
}

HEADERS += \
    tank.h \
    car.h \
    station.h \
    cost.h \
    tire.h \
    carmanager.h \
    fueltype.h \
    costtype.h \
    carevent.h \
    tiremount.h \
    filemodel.h \
    statfileinfo.h \
    globals.h \
    charttypes.h

SOURCES += CarBudget.cpp \
    tank.cpp \
    car.cpp \
    station.cpp \
    cost.cpp \
    tire.cpp \
    carmanager.cpp \
    fueltype.cpp \
    costtype.cpp \
    carevent.cpp \
    tiremount.cpp \
    filemodel.cpp \
    statfileinfo.cpp \
    globals.cpp


RESOURCES += \
    qml.qrc

    
# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

DISTFILES +=
