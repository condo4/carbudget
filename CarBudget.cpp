/**
 * CarBudget, Sailfish application to manage car cost
 *
 * Copyright (C) 2014 Fabien Proriol
 *
 * This file is part of CarBudget.
 *
 * CarBudget is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * CarBudget is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with CarBudget. If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Fabien Proriol
 */


#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include <QSettings>
#include <QLocale>
#include "src/tank.h"
#include "src/cost.h"
#include "src/tire.h"
#include "src/station.h"
#include "src/car.h"
#include "src/carmanager.h"
#include "src/filemodel.h"
#include "qmlLibs/qquickfolderlistmodel.h"

#include <QtCore/QTranslator>
#include <QQmlApplicationEngine>



int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/template.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //
    // To display the view, call "show()" (will show fullscreen on device).

    qDebug() << "Starting CarBudget" << APP_VERSION;

    QGuiApplication *app = SailfishApp::application(argc, argv);
    app->setOrganizationName("harbour-carbudget");
    app->setApplicationName("harbour-carbudget");
    QQuickView *view = SailfishApp::createView();

    QLocale systemLocale;

    QTranslator translator;
    if(translator.load((systemLocale.name() != "C")?(systemLocale.name()):("en_GB"), "/usr/share/harbour-carbudget/translations/"))
    {
        QGuiApplication::installTranslator(&translator);
    }

    app->setApplicationVersion(QString(APP_VERSION));

    // To circumvent Jolla Harbour limitation on QML import.
    qmlRegisterType<QQuickFolderListModel>("harbour.carbudget",1,0,"FolderListModel");

    qmlRegisterType<Tank>(      "harbour.carbudget",1,0,"Tank");
    qmlRegisterType<FuelType>(  "harbour.carbudget",1,0,"FuelType");
    qmlRegisterType<Station>(   "harbour.carbudget",1,0,"Station");
    qmlRegisterType<CostType>(  "harbour.carbudget",1,0,"CostType");
    qmlRegisterType<Cost>(      "harbour.carbudget",1,0,"Cost");
    qmlRegisterType<Tire>(      "harbour.carbudget",1,0,"Tire");
    qmlRegisterType<TireMount>( "harbour.carbudget",1,0,"TireMount");
    qmlRegisterType<Car>(       "harbour.carbudget",1,0,"Car");
    qmlRegisterType<FileModel>( "harbour.carbudget",1,0,"FileModel");

    CarManager manager;

    view->engine()->addImportPath("/usr/share/harbour-carbudget/qmlModules");
    view->rootContext()->setContextProperty("manager", &manager);
    view->rootContext()->setContextProperty("downloadPath", QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    view->rootContext()->setContextProperty("systemCurrencySymbol", systemLocale.currencySymbol());
    view->rootContext()->setContextProperty("systemDistanceUnit", QString(systemLocale.measurementSystem() == 0 ? "km" : "mi"));
    view->setSource(SailfishApp::pathTo("qml/Application.qml"));
    view->showFullScreen();

    int errorlevel = app->exec();
    qDebug() << "CarBudget exited normally.";
    return errorlevel;
}

