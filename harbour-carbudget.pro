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

DEFINES += SAILFISH APP_VERSION=\"\\\"$${VERSION}\\\"\"

HEADERS += \
    src/tank.h \
    src/car.h \
    src/station.h \
    src/cost.h \
    src/tire.h \
    src/carmanager.h \
    src/fueltype.h \
    src/costtype.h \
    src/carevent.h \
    src/tiremount.h \
    src/filemodel.h \
    src/statfileinfo.h \
    src/globals.h \
    src/charttypes.h

SOURCES += CarBudget.cpp \
    src/tank.cpp \
    src/car.cpp \
    src/station.cpp \
    src/cost.cpp \
    src/tire.cpp \
    src/carmanager.cpp \
    src/fueltype.cpp \
    src/costtype.cpp \
    src/carevent.cpp \
    src/tiremount.cpp \
    src/filemodel.cpp \
    src/statfileinfo.cpp \
    src/globals.cpp
    
DISTFILES += \
    qml/pages/CostEntry.qml \
    qml/pages/ImportHelp.qml \
    qml/pages/CarEntry.qml \
    qml/pages/CarView.qml \
    qml/pages/About.qml \
    qml/pages/BudgetView.qml \
    qml/pages/TiremountView.qml \
    qml/pages/CosttypeView.qml \
    qml/pages/SelectImportFile.qml \
    qml/pages/TireEntry.qml \
    qml/pages/Settings.qml \
    qml/pages/TankEntry.qml \
    qml/pages/FueltypeView.qml \
    qml/pages/MycarImport.qml \
    qml/pages/CosttypeEntry.qml \
    qml/pages/TankEntryView.qml \
    qml/pages/FuelpadImport.qml \
    qml/pages/CostEntryView.qml \
    qml/pages/CostView.qml \
    qml/pages/TankView.qml \
    qml/pages/TireMount.qml \
    qml/pages/StationView.qml \
    qml/pages/BackupNotification.qml \
    qml/pages/CarBudgetImport.qml \
    qml/pages/CostStatistics.qml \
    qml/pages/TiremountEdit.qml \
    qml/pages/CarBudgetImportError.qml \
    qml/pages/ConsumptionStatistics.qml \
    qml/pages/TireView.qml \
    qml/pages/StationEntry.qml \
    qml/pages/SelectTankDate.qml \
    qml/pages/Statistics.qml \
    qml/pages/CarCreate.qml \
    qml/pages/DirectoryPage.qml \
    qml/pages/FueltypeEntry.qml \
    qml/Application.qml \
    qml/cover/CoverPage.qml \
    qmlModules/jbQuick/Charts/qmldir \
    qmlModules/jbQuick/Charts/*.qml \
    qmlModules/jbQuick/Charts/*.js \
    qmlModules/jbQuick/Charts/*.md \
    rpm/CarBudget.yaml \
    rpm/CarBudget.spec \
    harbour-carbudget.desktop \
    translations/*.ts

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

TRANSLATIONS = translations/de_DE.ts \
               translations/fi_FI.ts \
               translations/fr_FR.ts \
               translations/it_IT.ts \
               translations/ru_RU.ts \
               translations/sv_SE.ts

QML_IMPORT_PATH += ./qmlModules


jbcharts.files = \
    $$files(qmlModules/jbQuick/Charts/qmldir) \
    $$files(qmlModules/jbQuick/Charts/*.qml) \
    $$files(qmlModules/jbQuick/Charts/*.js) \
    $$files(qmlModules/jbQuick/Charts/*.md)
jbcharts.path = /usr/share/$${TARGET}/qmlModules/jbQuick/Charts
INSTALLS += jbcharts

RESOURCES += \
    Resources.qrc

QT += qml-private core-private
SOURCES += qmlLibs/qquickfolderlistmodel.cpp qmlLibs/fileinfothread.cpp
HEADERS += qmlLibs/qquickfolderlistmodel.h qmlLibs/fileproperty_p.h qmlLibs/fileinfothread_p.h
