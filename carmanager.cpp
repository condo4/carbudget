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

    if(!query.exec("CREATE TABLE CosttypeList (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);"))
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
    if(!query.exec("CREATE TABLE TankList (event INTEGER, quantity DOUBLE, price DOUBLE, full TINYINT, station INTEGER, fueltype INTEGER, note TEXT);"))
    {
        qDebug() << query.lastError();
        error = true;
    }
    if(!query.exec("CREATE TABLE CostList (event INTEGER, costtype INTEGER, cost DOUBLE, desc TEXT);"))
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
    // First load all fuel types
    qDebug() << "Start importing fuel types";
    QDomNodeList fueltypes= doc.elementsByTagName("FuelSubtype");
    for (int i = 0; i < fueltypes.size(); i++) {
        QDomNode n = fueltypes.item(i);
        QDomElement type = n.firstChildElement("code");
        if (type.isNull())
            continue;
            _car->addNewFueltype(type.text());
    }
    // Now import tank events

    qDebug() << "Now import tank events";
    QDomNodeList tanks= doc.elementsByTagName("refuel");
    for (int i = 0; i < tanks.size(); i++) {
        QDomNode n = tanks.item(i);
        QDomElement n_carname = n.firstChildElement("car_name");
        QDomElement n_station = n.firstChildElement("fuel_station");
        QDomElement n_date = n.firstChildElement("refuelDate");
        QDomElement n_quantity = n.firstChildElement("quantity");
        QDomElement n_distance = n.firstChildElement("distance");
        QDomElement n_price = n.firstChildElement("cost_def_curr");
        QDomElement n_refuel_type = n.firstChildElement("refuel_type");
        QDomElement n_fuel_subtype = n.firstChildElement("fuel_subtype");
        if (n_carname.isNull())
            continue;
        if (n_carname.text() == _car->getName())
        {
            // First add stations
            if (!n_station.isNull())
            {
                if (!_car->findStation(n_station.text()))
                {
                    _car->addNewStation(n_station.text());
                }
            }
            //Now add fuelEvent
            //QDomElements should not be empty, but just to make sure...
            QDate t_date;
            unsigned int t_distance=0;
            double t_quantity=0;
            double t_price=0;
            bool t_refuel_type=true;
            unsigned int t_station=0;
            unsigned int t_fueltype=0;
            if (!n_date.isNull())
            {
               QDateTime t_datetime;
               t_datetime = QDateTime::fromString(n_date.text(),"yyyy-MM-dd hh:mm");
               t_date = t_datetime.date();
            }
            if (!n_distance.isNull()) t_distance = n_distance.text().toInt();
            if (!n_quantity.isNull()) t_quantity = n_quantity.text().toDouble();
            if (!n_price.isNull()) t_price = n_price.text().toDouble();
            if (!n_refuel_type.isNull())
                if (n_refuel_type.text().toInt()!=0) t_refuel_type=false;
            if (!n_fuel_subtype.isNull())
            {
                Fueltype *fueltype = _car->findFueltype(n_fuel_subtype.text());
                if (fueltype)
                    t_fueltype=fueltype->id();
            }
            if (!n_station.isNull())
            {
                Station *station = _car->findStation(n_station.text());
                if (station)
                    t_station=station->id();
            }
            _car->addNewTank(t_date,t_distance,t_quantity,t_price,t_refuel_type,t_fueltype,t_station,"");
        }
    }
    // Now import cost types from bill types;
    qDebug() << "Now import bill types as cost types";
    QDomNodeList costtypes = doc.elementsByTagName("bill_type");
    for (int i = 0; i < costtypes.size(); i++) {
        QDomNode n = costtypes.item(i);
        QDomElement type = n.firstChildElement("name");
        if (type.isNull())
            continue;
         _car->addNewCosttype(type.text());
    }
    // We don't distinguish between service and bills, therefore add service types as cost types
    qDebug() << "Now import service categories as cost types";
    costtypes = doc.elementsByTagName("service_category");
    for (int i = 0; i < costtypes.size(); i++) {
        QDomNode n = costtypes.item(i);
        QDomElement type = n.firstChildElement("name");
        if (type.isNull())
            continue;
        _car->addNewCosttype(type.text());
    }
    //Now it's time to import the bills
    qDebug() << "Now import bills as costs";
    QDomNodeList bills = doc.elementsByTagName("bill");
    for (int i = 0; i < bills.size(); i++) {
        QDomNode n = bills.item(i);
        QDomElement n_carname = n.firstChildElement("car_name");
        if (n_carname.isNull())
            continue;
        if(n_carname.text()!=_car->getName())
            continue;
        QDomElement n_billtype = n.firstChildElement("bill_type_name");
        QDomElement n_cost = n.firstChildElement("cost");
        QDomElement n_date = n.firstChildElement("date");
        QDomElement n_note = n.firstChildElement("note");
        unsigned int t_billtype = 0;
        // bills in myCar do not support distance, so set it to 0
        unsigned int t_odo = 0;
        double t_cost = 0;
        QDate t_date;
        QString t_note;
        if (!n_billtype.isNull())
        {
            Costtype *costtype = _car->findCosttype(n_billtype.text());
            if (costtype) t_billtype = costtype->id();
        }
        if (!n_date.isNull())
        {
           QDateTime t_datetime;
           t_datetime = QDateTime::fromString(n_date.text(),"yyyy-MM-dd hh:mm");
           t_date = t_datetime.date();
        }
        if (!n_cost.isNull())
            t_cost=n_cost.text().toDouble();
        if (!n_note.isNull())
            t_note=n_note.text();
        _car->addNewCost(t_date,t_odo,t_billtype,t_note,t_cost);
    }
    //Now it's time to import the service records
    qDebug() << "Now import service records as costs";
    bills = doc.elementsByTagName("service_record");
    for (int i = 0; i < bills.size(); i++) {
        QDomNode n = bills.item(i);
        QDomElement n_carname = n.firstChildElement("carName");
        if (n_carname.isNull())
            continue;
        if(n_carname.text()!=_car->getName())
            continue;
        QDomElement n_billtype = n.firstChildElement("service_categories");
        QDomElement n_cost = n.firstChildElement("cost");
        QDomElement n_odo = n.firstChildElement("odometer");
        QDomElement n_date = n.firstChildElement("date");
        QDomElement n_note = n.firstChildElement("note");
        QDomElement n_garage = n.firstChildElement("garage");
        unsigned int t_billtype = 0;
        unsigned int t_odo = 0;
        double t_cost = 0;
        QDate t_date;
        QString t_note;
        if (!n_billtype.isNull())
        {
            Costtype *costtype = _car->findCosttype(n_billtype.text());
            if (costtype) t_billtype = costtype->id();
        }
        if (!n_odo.isNull())
        {
            t_odo =  (int) n_odo.text().toDouble();
        qDebug() <<"Odo: " << n_odo.text() << "converted: " << t_odo;
        }
        if (!n_date.isNull())
        {
           QDateTime t_datetime;
           t_datetime = QDateTime::fromString(n_date.text(),"yyyy-MM-dd hh:mm");
           t_date = t_datetime.date();
        }
        if (!n_cost.isNull())
            t_cost=n_cost.text().toDouble();
        if (!n_note.isNull())
            t_note=n_note.text();
        if (!n_garage.isNull())
            t_note+="\n"+n_garage.text();
        _car->addNewCost(t_date,t_odo,t_billtype,t_note,t_cost);
    }


}
