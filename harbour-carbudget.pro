# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = harbour-carbudget

CONFIG += sailfishapp
QT += sql xml

QML_IMPORT_PATH += qml

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

lupdate_only{
    SOURCES += qml/*.qml \
    SOURCES += qml/pages/*.qml \
    SOURCES += qml/jbQuick/Charts/*.qml \
    SOURCES += qml/jbQuick/Charts/*.js
}


CONFIG += sailfishapp_i18n

TRANSLATIONS = translations/de_DE.ts \
               translations/fi_FI.ts \
               translations/fr_FR.ts \
               translations/it_IT.ts \
               translations/ru_RU.ts \
               translations/sv_SE.ts


DEFINES += APP_VERSION=\"\\\"$${VERSION}\\\"\"

OTHER_FILES += qml/cover/CoverPage.qml \
    rpm/CarBudget.yaml \
    harbour-carbudget.desktop \
    qml/*.qml \
    qml/jbQuick/Charts/*


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


RESOURCES += \
    Resources.qrc
