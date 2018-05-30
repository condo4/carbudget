# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = harbour-carbudget

CONFIG += sailfishapp sailfishapp_i18n
QT += sql xml
DEFINES += APP_VERSION=\"\\\"$${VERSION}\\\"\"
QML_IMPORT_PATH += qml

TRANSLATIONS = translations/de_DE.ts \
               translations/fi_FI.ts \
               translations/fr_FR.ts \
               translations/it_IT.ts \
               translations/ru_RU.ts \
               translations/sv_SE.ts

lupdate_only{
    SOURCES += qml/*.qml \
    SOURCES += qml/pages/*.qml \
    SOURCES += qml/jbQuick/Charts/*.qml \
    SOURCES += qml/jbQuick/Charts/*.js
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

SAILFISHAPP_ICONS = 86x86 108x108 128x128

OTHER_FILES += rpm/CarBudget.yaml \
    harbour-carbudget.desktop

RESOURCES += \
    Resources.qrc

QT += qml-private core-private
SOURCES += qmlLibs/qquickfolderlistmodel.cpp qmlLibs/fileinfothread.cpp
HEADERS += qmlLibs/qquickfolderlistmodel.h qmlLibs/fileproperty_p.h qmlLibs/fileinfothread_p.h
