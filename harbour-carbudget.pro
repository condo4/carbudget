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
    SOURCES += qml/pages/*.qml
}


CONFIG += sailfishapp_i18n

TRANSLATIONS = translations/de_DE.ts \
               translations/fr_FR.ts \
               translations/it_IT.ts \
               translations/ru_RU.ts \
               translations/sv_SE.ts


DEFINES += APP_VERSION=\"\\\"$${VERSION}\\\"\"

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
    qml/pages/BudgetView.qml \
    qml/pages/Settings.qml \
    qml/pages/CosttypeEntry.qml \
    qml/pages/CosttypeView.qml \
    qml/pages/SelectImportFile.qml \
    qml/pages/MycarImport.qml \
    qml/pages/FuelpadImport.qml \
    qml/pages/CostStatistics.qml \
    qml/pages/TiremountView.qml \
    qml/pages/TiremountEdit.qml \
    qml/pages/ConsumptionStatistics.qml \
    qml/pages/DirectoryPage.qml \
    qml/pages/ImportHelp.qml

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
    globals.h


RESOURCES += \
    Ressources.qrc

DISTFILES += \
    qml/pages/Statistics.qml
