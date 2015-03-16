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
 * Authors: Fabien Proriol, Thomas Michel
 */


#include "carmanager.h"
#include <QSettings>
#include <QtXml/QDomDocument>
#include <QFile>

void CarManager::refresh()
{
    _cars.clear();
    QDir home(QDir::homePath());
    home.mkpath(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));

    home.setFilter(QDir::Files);
    home.setNameFilters(QStringList()<<"*.cbg");
    QStringList homeFileList = home.entryList();
    foreach(QString file, homeFileList)
    {
        QFile::rename(QDir::homePath() + QDir::separator() + file,
                      QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + file);
    }

    dir.setFilter(QDir::Files);
    dir.setNameFilters(QStringList()<<"*.cbg");
    QStringList fileList = dir.entryList();
    foreach(QString file, fileList)
    {
        _cars.append(file.replace(".cbg",""));
    }
    emit carsChanged();
}

CarManager::CarManager(QObject *parent) :
    QObject(parent)
{
    QSettings settings;
    refresh();

    if(settings.contains("SelectedCar"))
    {
        if(settings.value("SelectedCar").toString() != "NOT_SET")
            _car = new Car(settings.value("SelectedCar").toString());
        else
            _car = NULL;
    }
    else
    {
        _car = NULL;
    }
    emit carsChanged();
    emit carChanged();;
}

QStringList CarManager::cars()
{
    return _cars;
}

Car *CarManager::car()
{
    return _car;
}

void CarManager::selectCar(QString name)
{
    QSettings settings;
    settings.setValue("SelectedCar",name);
    if(_car != NULL) delete _car;
    _car = new Car(name);
    emit carChanged();
}

void CarManager::delCar(QString name)
{
    QSettings settings;
    if(_car && _car->getName() == name)
    {
        settings.setValue("SelectedCar","NOT_SET");
        delete _car;
        _car = NULL;
    }
    QFile::remove( QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + name + ".cbg");
    refresh();
}

void CarManager::createCar(QString name)
{
    bool error = false;
    QString db_name = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + name + ".cbg";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(db_name);

    if(!db.open())
    {
        qDebug() << "ERROR: fail to open file";
    }
    qDebug() << "DB:" << db_name;

    QSqlQuery query(db);
    if(!query.exec("CREATE TABLE CarBudget (id VARCHAR(20) PRIMARY KEY, value VARCHAR(20));"))
    {
        qDebug() << query.lastError();
        error = true;
    }
    if(!query.exec(QString("INSERT INTO CarBudget (id, value) VALUES ('version','%1');").arg(DB_VERSION)))
    {
        qDebug() << query.lastError();
        error = true;
    }

    if(!query.exec("CREATE TABLE FueltypeList (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);"))
    {
        qDebug() << query.lastError();
        error = true;
    }
    if(!query.exec("CREATE TABLE StationList (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);"))
    {
        qDebug() << query.lastError();
        error = true;
    }
    if(!query.exec("CREATE TABLE TireList (id INTEGER PRIMARY KEY AUTOINCREMENT, buydate DATE, trashdate DATE DEFAULT NULL, price DOUBLE, quantity INT, name TEXT, manufacturer TEXT, model TEXT);"))
    {
        qDebug() << query.lastError();
        error = true;
    }

    if(!query.exec("CREATE TABLE Event (id INTEGER PRIMARY KEY AUTOINCREMENT, date DATE, distance UNSIGNED BIG INT);"))
    {
        qDebug() << query.lastError();
        error = true;
    }
    if(!query.exec("CREATE TABLE TankList (event INTEGER, quantity DOUBLE, price DOUBLE, full TINYINT, station INTEGER, fueltype TEXT, note TEXT);"))
    {
        qDebug() << query.lastError();
        error = true;
    }
    if(!query.exec("CREATE TABLE CostList (event INTEGER, cost DOUBLE, desc TEXT);"))
    {
        qDebug() << query.lastError();
        error = true;
    }
    if(!query.exec("CREATE TABLE TireUsage (event_mount INTEGER, event_umount INTEGER, tire INTEGER);"))
    {
        qDebug() << query.lastError();
        error = true;
    }


    if(!query.exec("CREATE TABLE PeriodicList (id INTEGER PRIMARY KEY AUTOINCREMENT, first DATE, last DATE, cost DOUBLE, desc TEXT, period INTEGER);"))
    {
        qDebug() << query.lastError();
        error = true;
    }
    if(!error) db.commit();
    db.close();
    refresh();
}

void CarManager::importFromMyCar(QString name)
{
    createCar(name);
    selectCar(name);
    QDomDocument doc;
    QFile file("/home/nemo/mycar_data.xml");
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
    {
        qDebug() << "ERROR: fail to open myCar Backup file";
        return;
    }
    QDomNodeList fueltypes= doc.elementsByTagName("FuelSubtype");
    for (int i = 0; i < fueltypes.size(); i++) {
        QDomNode n = fueltypes.item(i);
        QDomElement type = n.firstChildElement("code");
        if (type.isNull())
            continue;
        qDebug() << "Adding fuelltyp " << type.text();
        _car->addNewFueltype(type.text());
    }
}
