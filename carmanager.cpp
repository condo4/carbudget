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

    // Initialise program data directory and make sure it exists.
    // Data directory: /home/nemo/.local/share/harbour-carbudget/harbour-carbudget
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dataDir(dataPath);
    if(!dataDir.exists())
    {
        qDebug() << "Creating data directory" << dataPath;
        if(!dataDir.mkpath(dataPath))
        {
            qDebug() << "ERROR: failed to create data directory" << dataPath;
        }
    }

    // Move (rename) *.cbg files from home directory to data directory.
    // This does not overwrite existing database files.
    // Home directory; /home/nemo
    QString homePath = QDir::homePath();
    QDir homeDir(homePath);
    homeDir.setFilter(QDir::Files);
    homeDir.setNameFilters(QStringList() << "*.cbg");
    bool fileMoved;
    foreach(QString file, homeDir.entryList())
    {
        fileMoved = QFile::rename(homePath + QDir::separator() + file,
                                  dataPath + QDir::separator() + file);
        qDebug() << "Moving"  << (homePath + QDir::separator() + file)
                 << "to"      << (dataPath + QDir::separator() + file)
                 << (fileMoved ? "" : "failed");
    }

    // Find and add *.cbg files to car list
    _cars.clear();
    dataDir.setFilter(QDir::Files);
    dataDir.setNameFilters(QStringList() << "*.cbg");
    foreach(QString file, dataDir.entryList())
    {
        _cars.append(file.replace(".cbg",""));
        qDebug() << "Adding car:" << file.replace(".cbg","");
    }
    emit carsChanged();
}

CarManager::CarManager(QObject *parent) :
    QObject(parent)
{
    QSettings settings;
    refresh();

    if(_cars.contains(settings.value("SelectedCar","").toString())) {
        _car = new Car(settings.value("SelectedCar").toString());
    }
    else if(_cars.count() == 1) {
        _car = new Car(QString(_cars.first()));
    }
    else {
        _car = NULL;
    }
    emit carsChanged();
    emit carChanged();
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
        emit carChanged();
    }
    QFile::remove( QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QDir::separator() + name + ".cbg");
    //refresh();
    _cars.removeOne(name);
    emit carsChanged();
}

bool CarManager::backupCar(QString name)
{
    // Initialise program data directory and make sure it exists.
    // Data directory: /home/nemo/.local/share/harbour-carbudget/harbour-carbudget
    QString source = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QDir::separator() + name + ".cbg";
    QString destination = QDir::homePath() + QDir::separator() + name + "_" + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".cbg.";
    qDebug() << source;
    qDebug() << destination;
    if(!QFile::exists(source))
        return false;
    if(QFile::exists(destination))
        return false;
    return QFile::copy(source, destination);
}

void CarManager::createCar(QString name)
{
    QString db_name = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QDir::separator() + name + ".cbg";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(db_name);

    if(!db.open())
    {
        qDebug() << "ERROR: fail to open file";
    }
    qDebug() << "DB:" << db_name;
    createTables(db);
    db.close();
    refresh();
}

bool CarManager::createTables(QSqlDatabase db)
{
    bool success = true;
    QSqlQuery query(db);
    QStringList sqlQueries;

    sqlQueries.append(QString("CREATE TABLE CarBudget (id VARCHAR(20) PRIMARY KEY, value VARCHAR(20));"));
    sqlQueries.append(QString("INSERT  INTO CarBudget (id, value) VALUES ('version','%1'),('make',''),('model',''),('year',''),('licensePlate','');").arg(DB_VERSION));
    sqlQueries.append(QString("CREATE TABLE CostTypeList (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);"));
    sqlQueries.append(QString("CREATE TABLE FuelTypeList (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);"));
    sqlQueries.append(QString("CREATE TABLE StationList (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);"));
    sqlQueries.append(QString("CREATE TABLE TireList (id INTEGER PRIMARY KEY AUTOINCREMENT, buyDate DATE, trashDate DATE DEFAULT NULL, price DOUBLE, quantity INT, name TEXT, manufacturer TEXT, model TEXT);"));
    sqlQueries.append(QString("CREATE TABLE Event (id INTEGER PRIMARY KEY AUTOINCREMENT, date DATE, distance UNSIGNED BIG INT);"));
    sqlQueries.append(QString("CREATE TABLE TankList (event INTEGER, quantity DOUBLE, price DOUBLE, full TINYINT, station INTEGER, fuelType INTEGER, note TEXT);"));
    sqlQueries.append(QString("CREATE TABLE CostList (event INTEGER, costType INTEGER, cost DOUBLE, desc TEXT);"));
    sqlQueries.append(QString("CREATE TABLE TireUsage (event_mount INTEGER, event_umount INTEGER, tire INTEGER);"));
    sqlQueries.append(QString("CREATE TABLE PeriodicList (id INTEGER PRIMARY KEY AUTOINCREMENT, first DATE, last DATE, cost DOUBLE, desc TEXT, period INTEGER);"));

    db.transaction();
    foreach (const QString &q, sqlQueries) {
        if(!query.exec(q)){
            qDebug() << query.lastError();
            success = false;
        }
    }

    if(success) {
        qDebug() << "Database version" << DB_VERSION << "created.";
        db.commit();
    }
    else {
        qDebug() << "There were errors during dar database creation.";
        db.rollback();
    }

    return success;
}

void CarManager::importFromMyCar(QString filename, QString name)
{
    createCar(name);
    selectCar(name);
    QDomDocument doc;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
    {
        qDebug() << "ERROR: fail to open myCar Backup file";
        return;
    }
    // First load all fuel types
    qDebug() << "Start importing fuel types";
    QDomNodeList fuelTypes= doc.elementsByTagName("FuelSubtype");
    for (int i = 0; i < fuelTypes.size(); i++) {
        QDomNode n = fuelTypes.item(i);
        QDomElement type = n.firstChildElement("code");
        if (type.isNull())
            continue;
        _car->addNewFuelType(type.text());
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
        QDomElement n_note = n.firstChildElement("note");
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
            unsigned int t_fuelType=0;
            QString t_note;
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
                FuelType *fuelType = _car->findFuelType(n_fuel_subtype.text());
                if (fuelType)
                    t_fuelType=fuelType->id();
            }
            if (!n_station.isNull())
            {
                Station *station = _car->findStation(n_station.text());
                if (station)
                    t_station=station->id();
            }
            if (!n_note.isNull()) t_note = n_note.text();
            _car->addNewTank(t_date,t_distance,t_quantity,t_price,t_refuel_type,t_fuelType,t_station,t_note);
        }
    }
    // Now import cost types from bill types;
    qDebug() << "Now import bill types as cost types";
    QDomNodeList costTypes = doc.elementsByTagName("bill_type");
    for (int i = 0; i < costTypes.size(); i++) {
        QDomNode n = costTypes.item(i);
        QDomElement type = n.firstChildElement("name");
        if (type.isNull())
            continue;
        _car->addNewCostType(type.text());
    }
    // We don't distinguish between service and bills, therefore add service types as cost types
    qDebug() << "Now import service categories as cost types";
    costTypes = doc.elementsByTagName("service_category");
    for (int i = 0; i < costTypes.size(); i++) {
        QDomNode n = costTypes.item(i);
        QDomElement type = n.firstChildElement("name");
        if (type.isNull())
            continue;
        _car->addNewCostType(type.text());
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
        unsigned int t_odo = 0;
        double t_cost = 0;
        QDate t_date;
        QString t_note;
        if (!n_billtype.isNull())
        {
            CostType *costType = _car->findCostType(n_billtype.text());
            if (costType) t_billtype = costType->id();
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
        // bills in myCar do not support distance but are needed in carbudet
        // We simply set odo to distnace of last tank before bill date
        // This is a bit ugly and should be improved :-)
        t_odo = _car->getDistance(t_date);
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
            CostType *costType = _car->findCostType(n_billtype.text());
            if (costType) t_billtype = costType->id();
        }
        if (!n_odo.isNull())
        {
            t_odo =  (int) n_odo.text().toDouble();
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


void CarManager::importFromFuelpad(QString filename, QString name)
{
    qDebug() << "Importing from Fuelpad";
    createCar(name);
    selectCar(name);
    filename.remove(0,7);
    qDebug() << "INFO: attempting to process fuelpad DB" << filename;
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE","fuelpaddb");
    db.setDatabaseName(filename);
    if(!db.open())
    {
        qDebug() << "ERROR: fail to open Fuelpad database";
        return;
    }
    // First insert Fuelpad costTypes
    _car->addNewCostType(QString("Service"));
    _car->addNewCostType(QString("Oil"));
    _car->addNewCostType(QString("Tires"));
    _car->addNewCostType(QString("Insurance"));
    _car->addNewCostType(QString("Other"));
    // Get ids of costTypes for faster import
    // We should probably think about addNewXXX to return the id, could make life easier
    int t_serviceid=_car->findCostType("Service")->id();
    int t_oilid=_car->findCostType("Oil")->id();
    int t_tiresid=_car->findCostType("Tires")->id();
    int t_insuranceid=_car->findCostType("Insurance")->id();
    int t_otherid=_car->findCostType("Other")->id();
    QSqlQuery query(db);
    QDate t_date;
    unsigned long int t_km;
    double t_fill;
    double t_price;
    double t_service;
    double t_oil;
    double t_tires;
    double t_insurance;
    double t_other;
    QString t_notes;
    int t_id=0;
    if (!query.exec(QString("Select id from car WHERE register='%1';").arg(name)))
    {
        qDebug() << query.lastError();
        db.close();
        return;
    }
    if (query.next())
        t_id = query.value(0).toInt();
    qDebug() << "Car id:" <<  t_id;
    if(query.exec(QString("SELECT day,km,fill,price,service,oil,tires,insurance,other,notes FROM record WHERE carid=%1;").arg(t_id)))
    {
        while(query.next())
        {
            t_date = query.value(0).toDate();
            t_km = (int) query.value(1).toDouble();
            t_fill = query.value(2).toDouble();
            t_price = query.value(3).toDouble();
            t_service = query.value(4).toDouble();
            t_oil = query.value(5).toDouble();
            t_tires = query.value(6).toDouble();
            t_insurance = query.value(7).toDouble();
            t_other = query.value(8).toDouble();
            t_notes = query.value(9).toString();
            if (t_fill!=0)
                _car->addNewTank(t_date,t_km,t_fill,t_price,true,0,0,t_notes);
            if (t_service!=0)
                _car->addNewCost(t_date,t_km,t_serviceid,t_notes,t_service);
            if (t_oil!=0)
                _car->addNewCost(t_date,t_km,t_oilid,t_notes,t_oil);
            if (t_tires!=0)
                _car->addNewCost(t_date,t_km,t_tiresid,t_notes,t_tires);
            if (t_insurance!=0)
                _car->addNewCost(t_date,t_km,t_insuranceid,t_notes,t_insurance);
            if (t_other!=0)
                _car->addNewCost(t_date,t_km,t_otherid,t_notes,t_other);
            qDebug() << "Oil" << t_oil << "Insurance" << t_insurance;
        }
    }
    else
    {
        qDebug() << query.lastError();
    }
    db.close();
}

bool CarManager::is_debug() const
{
#ifdef QT_NO_DEBUG
    return false;
#else
    return true;
#endif
}

QString CarManager::getEnv(QString name)
{
    qDebug() << "Find environment value for" << name << ":" << getenv(name.toStdString().c_str());
    return getenv(name.toStdString().c_str());
}

QStringList CarManager::checkFuelpadDBforCars( QString name)
{
    QStringList fuelpadcars;
    QSqlDatabase db;
    name.remove(0,7);
    qDebug() << "INFO: attempting to process fuelpad DB" << name;
    db = QSqlDatabase::addDatabase("QSQLITE","fuelpaddb");
    db.setDatabaseName(name);
    if(!db.open())
    {
        qDebug() << "ERROR: fail to open Fuelpad database";
        db.close();
        return fuelpadcars;
    }
    QSqlQuery query(db);
    if(query.exec("SELECT register FROM car;"))
    {
        while(query.next())
        {
            QString name = query.value(0).toString();
            fuelpadcars.append(name);
        }
    }
    else
    {
        qDebug() << query.lastError();
    }
    db.close();
    return fuelpadcars;
}
