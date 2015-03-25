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
#include "tank.h"
#include "cost.h"
#include "tire.h"
#include "station.h"
#include "car.h"
#include "carmanager.h"

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

    QGuiApplication *app = SailfishApp::application(argc, argv);
    QQuickView *view = SailfishApp::createView();

    QTranslator translator;
    if(translator.load((QLocale::system().name() != "C")?(QLocale::system().name()):("en_GB"), ":/i18n"))
    {
        QGuiApplication::installTranslator(&translator);
    }

    qmlRegisterType<Tank>(      "harbour.carbudget",1,0,"Tank");
    qmlRegisterType<Fueltype>(  "harbour.carbudget",1,0,"Fueltype");
    qmlRegisterType<Station>(   "harbour.carbudget",1,0,"Station");
    qmlRegisterType<Costtype>(  "harbour.carbudget",1,0,"Costtype");
    qmlRegisterType<Cost>(      "harbour.carbudget",1,0,"Cost");
    qmlRegisterType<Tire>(      "harbour.carbudget",1,0,"Tire");
    qmlRegisterType<Car>(       "harbour.carbudget",1,0,"Car");


    CarManager manager;

    view->rootContext()->setContextProperty("manager", &manager);
    view->setSource(SailfishApp::pathTo("qml/Application.qml"));
    view->showFullScreen();

    return app->exec();
}

