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
QT += sql

SOURCES += CarBudget.cpp \
    tank.cpp \
    car.cpp \
    station.cpp \
    cost.cpp \
    tire.cpp \
    carmanager.cpp \
    fueltype.cpp \
    carevent.cpp

lupdate_only{
    SOURCES += qml/*.qml \
    SOURCES += qml/pages/*.qml
}

OTHER_FILES += qml/cover/CoverPage.qml \
    rpm/CarBudget.yaml \
    harbour-carbudget.desktop \
    qml/Application.qml \
    qml/pages/CarView.qml \
    qml/pages/CarEntry.qml \
    qml/pages/TankView.qml \
    qml/pages/TankEntry.qml \
    qml/pages/StationView.qml \
    qml/pages/StationEntry.qml \
    qml/pages/FueltypeView.qml \
    qml/pages/FueltypeEntry.qml \
    qml/pages/CostEntry.qml \
    qml/pages/CostView.qml \
    qml/pages/TireView.qml \
    qml/pages/TireEntry.qml \
    qml/pages/TireMount.qml \
    qml/pages/About.qml \
    qml/pages/CarCreate.qml \
    i18n/FR_fr.ts \
    i18n/RU_ru.ts \
    qml/pages/BudgetView.qml \
    qml/pages/Settings.qml \
    qml/pages/TankEntryView.qml \
    qml/pages/MyCarImportMainview.qml

HEADERS += \
    tank.h \
    car.h \
    station.h \
    cost.h \
    tire.h \
    carmanager.h \
    carevent.h \
    fueltype.h

TRANSLATIONS = CarBudget_fr.ts \
               CarBudget_en.ts \
               CarBudget_it.ts \
               CarBudget_ru.ts

RESOURCES += \
    Ressources.qrc
