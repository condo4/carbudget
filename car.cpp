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

#include "car.h"
#include "tank.h"
#include "cost.h"
#include "carmanager.h"
#include "fueltype.h"
#include "station.h"
#include <QDebug>

#define CREATE_NEW_EVENT (0)

bool sortTankByDistance(const Tank *c1, const Tank *c2)
{
    return c1->distance() > c2->distance();
}

bool sortCostByDistance(const Cost *c1, const Cost *c2)
{
    return c1->distance() > c2->distance();
}

bool sortCosttypeById(const Costtype *c1, const Costtype *c2)
{
    return c1->id() < c2->id();
}

bool sortFueltypeById(const Fueltype *c1, const Fueltype *c2)
{
    return c1->id() < c2->id();
}

bool sortStationById(const Station *c1, const Station *c2)
{
    return c1->id() < c2->id();
}

void Car::db_init()
{
    QString db_name = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + _name + ".cbg";
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName(db_name);

    if(!this->db.open())
    {
        qDebug() << "ERROR: fail to open file";
    }
    qDebug() << "DB:" << db_name;
}

void Car::db_load()
{
    QSqlQuery query(this->db);

    _tanklist.clear();
    _stationlist.clear();
    _tirelist.clear();
    _costlist.clear();

    if(query.exec("SELECT event,date(date),distance,quantity,price,full,station,fueltype,note FROM TankList, Event WHERE TankList.event == Event.id;"))
    {
        qDebug() << "Start loading tank events";

        while(query.next())
        {
            qDebug() << "Importing event from " << query.value(1);
            int id = query.value(0).toInt();
            QDate date = query.value(1).toDate();
            unsigned int distance = query.value(2).toInt();
            double quantity = query.value(3).toDouble();
            double price = query.value(4).toDouble();
            bool full = query.value(5).toBool();
            unsigned int station = query.value(6).toInt();
            unsigned int fueltype = query.value(7).toInt();
            QString note = query.value(8).toString();
            Tank *tank = new Tank(date, distance, quantity, price, full, fueltype, station, id, note, this);
            _tanklist.append(tank);
        }
        emit nbtankChanged(_tanklist.count());
        emit consumptionChanged(this->consumption());
        emit maxdistanceChanged(this->maxdistance());
        emit mindistanceChanged(this->mindistance());
    }
    else
    {
        qDebug() << query.lastError();
    }
    if(query.exec("SELECT id,name FROM FueltypeList;"))
    {
        while(query.next())
        {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            Fueltype *fueltype = new Fueltype(id, name, this);
            _fueltypelist.append(fueltype);
        }
    }
    if(query.exec("SELECT id,name FROM StationList;"))
    {
        while(query.next())
        {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            Station *station = new Station(id, name, this);
            _stationlist.append(station);
        }
    }
    else
    {
        qDebug() << query.lastError();
    }
    if(query.exec("SELECT id,name FROM CosttypeList;"))
    {
        while(query.next())
        {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            Costtype *costtype = new Costtype(id, name, this);
            _costtypelist.append(costtype);
        }
    }
    else
    {
        qDebug() << query.lastError();
    }
    if(query.exec("SELECT event,date,distance,cost,desc FROM CostList, Event WHERE CostList.event == Event.id;"))
    {
        while(query.next())
        {
            int id = query.value(0).toInt();
            QDate date = query.value(1).toDate();
            unsigned int distance = query.value(2).toInt();
            unsigned int costtype = query.value(3).toInt();
            double price = query.value(4).toDouble();
            QString description = query.value(5).toString();
            Cost *cost = new Cost(date,distance,costtype,description,price,id,this);
            _costlist.append(cost);
        }
    }
    else
    {
        qDebug() << query.lastError();
    }

    if(query.exec("SELECT id,buydate,trashdate,price,name,manufacturer,model,quantity FROM TireList;"))
    {
        while(query.next())
        {
            int id = query.value(0).toInt();
            QDate buydate = query.value(1).toDate();
            QDate trashdate = query.value(2).toDate();
            double price = query.value(3).toDouble();
            QString name = query.value(4).toString();
            QString manufacturer = query.value(5).toString();
            QString model = query.value(6).toString();
            unsigned int quantity = query.value(7).toInt();
            Tire *tire = new Tire(buydate,trashdate,name,manufacturer,model,price,quantity,id,this);
            _tirelist.append(tire);
        }
    }
    else
    {
        qDebug() << query.lastError();
    }
    qSort(_tanklist.begin(),    _tanklist.end(),    sortTankByDistance);
    qSort(_costlist.begin(),    _costlist.end(),    sortCostByDistance);
    qSort(_stationlist.begin(), _stationlist.end(), sortStationById);
}

int Car::db_get_version()
{
    QSqlQuery query(this->db);

    if(query.exec("SELECT count(*) FROM sqlite_master where type='table' and name='CarBudget';"))
    {
        query.next();
        if(query.value(0).toInt() != 0)
        {
            if(query.exec("SELECT value FROM CarBudget WHERE id='version';"))
            {
                query.next();
                return query.value(0).toString().toInt();
            }
        }
    }

    return 0;
}

void Car::db_upgrade_to_2()
{
    QString sql = "ALTER TABLE TankList ADD COLUMN note TEXT;";
    QSqlQuery query(this->db);

    if(query.exec(sql))
    {
        if(query.exec("UPDATE CarBudget SET  value='2' WHERE id='version';"))
        {
            this->db.commit();
            return;
        }
    }
    this->db.rollback();
}

void Car::db_upgrade_to_3()
{
    QString sql = "ALTER TABLE TankList ADD COLUMN Fueltype TEXT;";
    QSqlQuery query(this->db);

    if(query.exec(sql))
    {
        if(query.exec("UPDATE CarBudget SET  value='3' WHERE id='version';"))
        {
            if (query.exec("CREATE TABLE FueltypeList (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);"))
            {
                if (query.exec("CREATE TABLE CosttypeList (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);"))
                {
                    if (query.exec("ALTER TABLE CostList ADD COLUMN Costtype INTEGER;"))
                    {
                        this->db.commit();
                        return;
                    }
                }
            }
        }
    }
    this->db.rollback();
}


Car::Car(CarManager *parent) : QObject(parent), _manager(parent)
{

}

Car::Car(QString name, CarManager *parent) : QObject(parent), _manager(parent), _name(name)
{
    this->db_init();
    while(this->db_get_version() < DB_VERSION)
    {
        qDebug() << "Update configuation database " << this->db_get_version() << " >> " << DB_VERSION;
        if(this->db_get_version() < 2)
        {
            db_upgrade_to_2();
        }
        if(this->db_get_version() < 3)
        {
            db_upgrade_to_3();
        }
    }
    qDebug() << "Database version " << this->db_get_version();

    this->db_load();

    this->_stationlist.append(new Station);
    qSort(_stationlist.begin(), _stationlist.end(), sortStationById);
}

unsigned int Car::nbtank() const
{
    return _tanklist.count();
}

double Car::consumption() const
{
    unsigned long int maxDistance = 0;
    unsigned long int minDistance = 999999999;
    double totalConsumption = 0;

    foreach(Tank *tank, _tanklist)
    {
        if(tank->distance() > maxDistance)
            maxDistance = tank->distance();
        if(tank->distance() < minDistance)
            minDistance = tank->distance();
        totalConsumption += tank->quantity();
    }
    if(maxDistance == 0) return 0;
    foreach(Tank *tank, _tanklist)
    {
        if(tank->distance() == minDistance)
        {
            totalConsumption -= tank->quantity();
            break;
        }
    }
    return totalConsumption / ((maxDistance - minDistance)/ 100.0);
}

unsigned int Car::maxdistance() const
{
    unsigned long int maxDistance = 0;

    foreach(Cost *cost, _costlist)
    {
        if(cost->distance() > maxDistance)
            maxDistance = cost->distance();
    }
    foreach(Tank *tank, _tanklist)
    {
        if(tank->distance() > maxDistance)
            maxDistance = tank->distance();
    }
    return maxDistance;
}

unsigned int Car::mindistance() const
{
    unsigned long int minDistance = 1000000;

    foreach(Cost *cost, _costlist)
    {
        if(cost->distance() < minDistance)
            minDistance = cost->distance();
    }
    foreach(Tank *tank, _tanklist)
    {
        if(tank->distance() < minDistance)
            minDistance = tank->distance();
    }
    return minDistance;
}

QQmlListProperty<Tank> Car::tanks()
{
    return QQmlListProperty<Tank>(this, _tanklist);
}

QQmlListProperty<Fueltype> Car::fueltypes()
{
    return QQmlListProperty<Fueltype>(this, _fueltypelist);
}

QQmlListProperty<Station> Car::stations()
{
    return QQmlListProperty<Station>(this, _stationlist);
}

QQmlListProperty<Costtype> Car::costtypes()
{
    return QQmlListProperty<Costtype>(this, _costtypelist);
}
QQmlListProperty<Cost> Car::costs()
{
    return QQmlListProperty<Cost>(this, _costlist);
}

QQmlListProperty<Tire> Car::tires()
{
    return QQmlListProperty<Tire>(this, _tirelist);
}

const Tank *Car::previousTank(unsigned int distance) const
{
    const Tank *previous = NULL;

    foreach(Tank *tank, _tanklist)
    {
        if(previous == NULL && tank->distance() < distance)
        {
            previous = tank;
        }
        else if(tank->distance() < distance && tank->distance() > previous->distance())
        {
            previous = tank;
        }
    }
    return previous;
}

void Car::setCar(QString name)
{
    _name = name;
    _tanklist.clear();
    _fueltypelist.clear();
    _stationlist.clear();
    _tirelist.clear();
    _costlist.clear();
    this->db_init();

    if(this->db_get_version() < DB_VERSION)
    {
        qDebug() << "Old database version " << this->db_get_version() << " >> " << DB_VERSION;
    }
    else
    {
        qDebug() << "Database version " << this->db_get_version();
    }

    this->db_load();

    this->_stationlist.append(new Station);
    qSort(_stationlist.begin(), _stationlist.end(), sortStationById);
}

double Car::budget_fuel()
{
    /* Return sum(fuel price) / ODO * 100 */
    unsigned long int maxDistance = 0;
    unsigned long int minDistance = 999999999;
    double totalPrice = 0;

    foreach(Tank *tank, _tanklist)
    {
        if(tank->distance() > maxDistance)
            maxDistance = tank->distance();
        if(tank->distance() < minDistance)
            minDistance = tank->distance();
        totalPrice += tank->price();
    }
    foreach(Tank *tank, _tanklist)
    {
        if(tank->distance() == minDistance)
        {
            totalPrice -= tank->price();
            break;
        }
    }
    if(maxDistance == 0) return 0;
    return totalPrice / ((maxDistance - minDistance)/ 100.0);
}

double Car::budget_cost()
{
    /* Return sum(cost) / ODO * 100 */
    double totalPrice = 0;

    foreach(Cost *cost, _costlist)
    {
        totalPrice += cost->cost();
    }
    return totalPrice / ((maxdistance() - mindistance())/ 100.0);
}

double Car::budget()
{
    return budget_fuel() + budget_cost();
}
void Car::addNewTank(QDate date, unsigned int distance, double quantity, double price, bool full, unsigned int fueltype, unsigned int station, QString note)
{
    Tank *tank = new Tank(date, distance, quantity, price, full, fueltype, station, CREATE_NEW_EVENT,  note, this);
    _tanklist.append(tank);
    qSort(_tanklist.begin(), _tanklist.end(), sortTankByDistance);
    tank->save();
    emit nbtankChanged(_tanklist.count());
    emit consumptionChanged(this->consumption());
    emit maxdistanceChanged(this->maxdistance());
    emit tanksChanged();
}

void Car::delTank(Tank *tank)
{
    qDebug() << "Remove tank " << tank->id();
    _tanklist.removeAll(tank);
    qSort(_tanklist.begin(), _tanklist.end(), sortTankByDistance);
    tank->remove();
    emit nbtankChanged(_tanklist.count());
    emit consumptionChanged(this->consumption());
    emit maxdistanceChanged(this->maxdistance());
    emit tanksChanged();
    tank->deleteLater();
}


void Car::addNewFueltype(QString name)
{
    Fueltype *fueltype = new Fueltype(-1, name, this);
    _fueltypelist.append(fueltype);
    qSort(_fueltypelist.begin(), _fueltypelist.end(), sortFueltypeById);
    fueltype->save();
    emit fueltypesChanged();
}

void Car::delFueltype(Fueltype *fueltype)
{
    qDebug() << "Remove Fuel Type " << fueltype->id();
    _fueltypelist.removeAll(fueltype);
    qSort(_fueltypelist.begin(), _fueltypelist.end(), sortFueltypeById);
    QSqlQuery query(db);
    QString sql = QString("UPDATE TankList SET Fueltype = 0 WHERE fueltyp=%1;").arg(fueltype->id());

    if(query.exec(sql))
    {
        QString sql2 = QString("DELETE FROM FueltypeList WHERE id=%1;").arg(fueltype->id());
        qDebug() << sql2;
        if(query.exec(sql2))
        {
            qDebug() << "DELETE Fueltype in database with id " << fueltype->id();
            db.commit();
        }
        else
        {
            qDebug() << "Error during DELETE Fueltype in database";
            qDebug() << query.lastError();
        }
    }
    else
    {
        qDebug() << "Error during DELETE Fueltype in database";
        qDebug() << query.lastError();
    }
    foreach(Tank *tank, _tanklist)
    {
        if(tank->fueltype() == fueltype->id())
        {
            tank->setFueltype(0);
        }
    }
    emit fueltypesChanged();
    fueltype->deleteLater();
}

Fueltype* Car::findFueltype(QString name)
{
    foreach (Fueltype *fueltype, _fueltypelist)
    {
        if (fueltype->name()==name)
            return fueltype;
    }
    return NULL;
}

QString Car::getFueltypeName(unsigned int id)
{
    foreach (Fueltype *fueltype, _fueltypelist)
    {
        if (fueltype->id()==id)
            return fueltype->name();
    }
    return "";
}

void Car::addNewStation(QString name)
{
    Station *station = new Station(-1, name, this);
    _stationlist.append(station);
    qSort(_stationlist.begin(), _stationlist.end(), sortStationById);
    station->save();
    emit stationsChanged();
}

void Car::delStation(Station *station)
{
    qDebug() << "Remove Station " << station->id();
    _stationlist.removeAll(station);
    qSort(_stationlist.begin(), _stationlist.end(), sortStationById);
    QSqlQuery query(db);
    QString sql = QString("UPDATE TankList SET station = 0 WHERE station=%1;").arg(station->id());

    if(query.exec(sql))
    {
        QString sql2 = QString("DELETE FROM StationList WHERE id=%1;").arg(station->id());
        qDebug() << sql2;
        if(query.exec(sql2))
        {
            qDebug() << "DELETE Station in database with id " << station->id();
            db.commit();
        }
        else
        {
            qDebug() << "Error during DELETE Station in database";
            qDebug() << query.lastError();
        }
    }
    else
    {
        qDebug() << "Error during DELETE Station in database";
        qDebug() << query.lastError();
    }
    foreach(Tank *tank, _tanklist)
    {
        if(tank->station() == station->id())
        {
            tank->setStation(0);
        }
    }
    emit stationsChanged();
    station->deleteLater();
}

Station* Car::findStation(QString name)
{
    foreach (Station *station, _stationlist)
    {
        if (station->name()==name)
            return station;
    }
    return NULL;
}

QString Car::getStationName(unsigned int id)
{
    foreach (Station *station, _stationlist)
    {
        if (station->id()==id)
            return station->name();
    }
    return "";
}

void Car::addNewCosttype(QString name)
{
    Costtype *costtype = new Costtype(-1, name, this);
    _costtypelist.append(costtype);
    qSort(_costtypelist.begin(), _costtypelist.end(), sortCosttypeById);
    costtype->save();
    emit costtypesChanged();
}

void Car::delCosttype(Costtype *costtype)
{
    qDebug() << "Remove Cost Type " << costtype->id();
    _costtypelist.removeAll(costtype);
    qSort(_costtypelist.begin(), _costtypelist.end(), sortCosttypeById);
    QSqlQuery query(db);
    QString sql = QString("UPDATE CostList SET costtype = 0 WHERE costtype=%1;").arg(costtype->id());

    if(query.exec(sql))
    {
        QString sql2 = QString("DELETE FROM CosttypeList WHERE id=%1;").arg(costtype->id());
        qDebug() << sql2;
        if(query.exec(sql2))
        {
            qDebug() << "DELETE Costtype in database with id " << costtype->id();
            db.commit();
        }
        else
        {
            qDebug() << "Error during DELETE Costtype in database";
            qDebug() << query.lastError();
        }
    }
    else
    {
        qDebug() << "Error during DELETE Costtype in database";
        qDebug() << query.lastError();
    }
    foreach(Cost *cost, _costlist)
    {
        if(cost->costtype() == costtype->id())
        {
            cost->setCosttype(0);
        }
    }
    emit costsChanged();
    costtype->deleteLater();
}

Costtype* Car::findCosttype(QString name)
{
    foreach (Costtype *costtype, _costtypelist)
    {
        if (costtype->name()==name)
            return costtype;
    }
    return NULL;
}

QString Car::getCosttypeName(unsigned int id)
{
    foreach (Costtype *costtype, _costtypelist)
    {
        if (costtype->id()==id)
            return costtype->name();
    }
    return "";
}

void Car::addNewCost(QDate date, unsigned int distance, unsigned int costtype, QString description, double price)
{
    Cost *cost = new Cost(date,distance,costtype,description,price,CREATE_NEW_EVENT,this);
    _costlist.append(cost);
    qSort(_costlist.begin(), _costlist.end(), sortCostByDistance);
    cost->save();
    emit costsChanged();
}

void Car::delCost(Cost *cost)
{
    qDebug() << "Remove Cost " << cost->id();
    _costlist.removeAll(cost);
    qSort(_costlist.begin(), _costlist.end(), sortCostByDistance);
    cost->remove();
    emit costsChanged();
    cost->deleteLater();
}

Tire *Car::addNewTire(QDate buydate, QString name, QString manufacturer, QString model, double price, unsigned int quantity)
{
    Tire *tire = new Tire(buydate,buydate,name,manufacturer,model,price,quantity,-1,this);
    _tirelist.append(tire);
    tire->save();
    emit tiresChanged();
    return tire;
}

void Car::delTire(Tire *tire)
{
    qDebug() << "Remove Tire " << tire->id() << " : " << _tirelist.removeAll(tire);
    QSqlQuery query(db);
    QString sql = QString("DELETE FROM TireList WHERE id=%1;").arg(tire->id());
    if(query.exec(sql))
    {
        qDebug() << "DELETE Tire in database with id " << tire->id();
        db.commit();
    }
    else
    {
        qDebug() << "Error during DELETE Tire in database";
        qDebug() << query.lastError();
    }

    emit tiresChanged();
    tire->deleteLater();
}

void Car::mountTire(QDate mountdate, unsigned int distance, Tire *tire)
{
    qDebug() << "Mount tire";
    QSqlQuery query(db);

    if(!tire->mountable())
    {
        qDebug() << "Can't mount this tire";
        return;
    }

    int id;
    QString sql = QString("INSERT INTO Event (id,date,distance) VALUES(NULL,'%1',%2)").arg(mountdate.toString("yyyy-MM-dd 00:00:00.00")).arg(distance);
    if(query.exec(sql))
    {
        id = query.lastInsertId().toInt();
        qDebug() << "Create Event(Tank) in database with id " << id;

        QString sql2 = QString("INSERT INTO TireUsage (event_mount,event_umount,tire) VALUES(%1,0,%2)").arg(id).arg(tire->id());
        if(query.exec(sql2))
        {
            id = query.lastInsertId().toInt();
            qDebug() << "Create TireUsage in database with id " << id;
            db.commit();
        }
        else id = -1;
    }
    else id = -1;

    if(id == -1)
    {
        qDebug() << "Can't mount this tire set (db error)";
        qDebug() << query.lastError();
    }
    else emit tireMountedChanged();
}

void Car::umountTire(QDate umountdate, unsigned int distance, Tire *tire, bool trashit)
{
    qDebug() << "Umount tire";
    if(!tire->mounted())
    {
        qDebug() << "Can't umount this tire";
        return;
    }

    QSqlQuery query(db);
    int id;
    QString sql = QString("INSERT INTO Event (id,date,distance) VALUES(NULL,'%1',%2)").arg(umountdate.toString("yyyy-MM-dd 00:00:00.00")).arg(distance);
    if(query.exec(sql))
    {
        id = query.lastInsertId().toInt();
        qDebug() << "Create Event(Tank) in database with id " << id;

        QString sql2 = QString("UPDATE TireUsage SET event_umount=%1 WHERE tire=%2 AND event_umount=0").arg(id).arg(tire->id());
        if(query.exec(sql2))
        {
            qDebug() << "Update TireUsage in database";
            db.commit();
        }
        else id = -1;
    }
    else id = -1;

    if(id == -1)
    {
        qDebug() << "Can't umount this tire set (db error)";
        qDebug() << query.lastError();
    }
    else
    {
        if(trashit)
        {
            QString sql2 = QString("UPDATE TireList SET trashdate='%1' WHERE id=%2").arg(umountdate.toString("yyyy-MM-dd 00:00:00.00")).arg(tire->id());
            if(query.exec(sql2))
            {
                qDebug() << "Update TireList in database to trash";
                tire->setTrashdate(QDateTime(umountdate));
                db.commit();
                emit tiresChanged();
            }
            else
            {
                qDebug() << "Can't trash this tire set (db error)";
                qDebug() << query.lastError();
            }
        }
        emit tireMountedChanged();
    }
}

int Car::tireMounted() const
{
    QSqlQuery query(db);

    /* Check if not mounted */
    QString sql = QString("SELECT SUM(TireList.quantity) FROM TireUsage, TireList WHERE TireUsage.tire == TireList.id AND event_umount == 0");
    if(query.exec(sql))
    {
        query.next();
        return query.value(0).toInt();
    }
    qDebug() << "ERROR";
    return 0;
}

QString Car::currency()
{
    if(_currency.length() < 1)
    {
        QSqlQuery query(this->db);

        if(query.exec("SELECT value FROM CarBudget WHERE id='currency';"))
        {
            query.next();
            _currency = query.value(0).toString();
            qDebug() << "Find currency in database: " << _currency;
        }
        if(_currency.length() < 1)
        {
            qDebug() << "Default currency not set in database, set to €";
            query.exec("INSERT INTO CarBudget (id, value) VALUES ('currency','€');");
            _currency = "€";
        }
    }

    return _currency;
}

void Car::setCurrency(QString currency)
{
    QSqlQuery query(this->db);

    if(query.exec("SELECT count(*) FROM CarBudget WHERE id='currency';"))
    {
        query.next();
        if(query.value(0).toString().toInt() < 1)
            query.exec(QString("INSERT INTO CarBudget (id, value) VALUES ('currency','%1');").arg(currency));
        else
            query.exec(QString("UPDATE CarBudget SET  value='%1' WHERE id='currency';").arg(currency));

        qDebug() << "Change currency in database: " << _currency;
    }
    _currency = currency;
    emit currencyChanged();
}


void Car::simulation()
{
    Tire *winter1, *winter2, *summer1;

    unsigned int km = 0;
    this->addNewStation("Saint Pal Mairie");
    this->addNewStation("Super U Craponne");
    this->addNewStation("Auchan Villard");

    winter1 = this->addNewTire(QDate(2010,11,15),"Pneu hiver","Michelin","Alpin A4",160,4);
    this->mountTire(QDate(2012,11,15), km, winter1);
/*
    this->addNewTank(QDate(2010,11,15),km += 850,52,70,true,0,0, "t1");
    this->addNewCost(QDate(2011, 1, 1),km += 100,"Revision 5000",50);
    this->addNewTank(QDate(2011, 1,29),km += 1101,60,70,true,1,"t6");
     this->umountTire(QDate(2011, 4, 5), km += 100, winter1);
    summer1 = this->addNewTire(QDate(2011,4,5),"Pneu été","Michelin","EnergySaver",110,4);
    this->mountTire(QDate(2011,4,5), km, summer1);

    this->addNewTank(QDate(2011, 4, 8),km += 1051,60,70,true,1,"Diesel","");
    this->addNewCost(QDate(2011, 4,18),km += 10, "Vidange 15000",220);
    this->addNewTank(QDate(2011, 4,28),km += 1028,60,70,true,3,"Diesel", "");
    this->addNewTank(QDate(2011, 5, 9),km += 1021,60,70,true,3,"Diesel", "");
    this->addNewTank(QDate(2011, 5,18),km += 1022,60,70,true,3,"Diesel", "");
    this->addNewTank(QDate(2011, 5,20),km += 1023,60,70,true,2,"Diesel", "");
    this->addNewTank(QDate(2011, 5,28),km += 1024,60,70,true,1,"Diesel", "");
    this->addNewTank(QDate(2011, 6,18),km += 1025,60,70,true,3,"Diesel", "");
    this->addNewTank(QDate(2011, 6,28),km += 1026,60,70,true,2,"Diesel", "");
    this->addNewTank(QDate(2011, 7,18),km += 1027,60,70,true,1,"Diesel", "A new note");
    this->addNewTank(QDate(2011, 7,28),km += 1028,60,70,true,2,"Diesel", "");
    this->addNewTank(QDate(2011, 8,18),km += 1029,60,70,true,1,"Diesel", "");
    this->addNewTank(QDate(2011, 8,28),km += 1018,60,70,true,3,"Diesel", "");
    this->addNewTank(QDate(2011, 9,18),km += 1011,60,70,true,1,"Diesel", "");
    this->addNewTank(QDate(2011, 9,28),km += 1012,60,70,true,2,"Diesel", "");
    this->addNewTank(QDate(2011,10, 1),km += 1013,60,70,true,1,"Diesel", "");
    this->addNewCost(QDate(2011,10, 8),km += 15,"Vidange 30000",220);
    this->addNewTank(QDate(2011,10,29),km += 1101,60,70,true,1,"Diesel", "");
    this->umountTire(QDate(2011,10,30), km += 100, summer1);
    this->mountTire(QDate(2011,10,30), km, winter1);

    this->addNewTank(QDate(2011,11,15),km += 1099,60,70,true,2,"Diesel", "");
    this->addNewTank(QDate(2011,11,29),km += 1080,60,70,true,3,"Diesel", "");
    this->addNewTank(QDate(2011,12,15),km += 1010,60,70,true,1,"Diesel", "");
    this->addNewTank(QDate(2011,12,29),km += 1071,60,70,true,2,"Diesel", "");
    this->addNewTank(QDate(2012, 1,15),km += 1031,60,70,true,2,"Diesel", "");
    this->addNewTank(QDate(2012, 1,29),km += 1121,60,70,true,3,"Diesel", "");
    this->addNewTank(QDate(2012, 2,15),km += 1134,60,70,true,3,"Diesel", "");
    this->addNewTank(QDate(2012, 2,27),km += 1021,60,70,true,1,"Diesel", "");
    this->addNewTank(QDate(2012, 3,15),km += 1051,60,70,true,1,"Diesel", "");
    this->addNewTank(QDate(2012, 4, 8),km += 1051,60,70,true,1,"Diesel", "");
    this->addNewTank(QDate(2012, 4,28),km += 1028,60,70,true,3,"Diesel", "");
    this->addNewTank(QDate(2012, 5, 9),km += 1021,60,70,true,3,"Diesel", "");
    this->addNewTank(QDate(2012, 5,18),km += 1022,60,70,true,3,"Diesel", "");
    this->addNewTank(QDate(2012, 5,20),km += 1023,60,70,true,2,"Diesel", "");
    this->addNewTank(QDate(2012, 5,28),km += 1024,60,70,true,1,"Diesel", "");
    this->umountTire(QDate(2012, 5,30), km += 100,winter1,true);
    this->mountTire(QDate(2012, 5,30), km,  summer1);

    this->addNewTank(QDate(2012, 6,18),km += 1025,60,70,true,3,"Diesel", "");
    this->addNewTank(QDate(2012, 6,28),km += 1026,60,70,true,2,"Diesel", "");
    this->addNewTank(QDate(2012, 7,18),km += 1027,60,70,true,1,"Diesel", "");
    this->addNewTank(QDate(2012, 7,28),km += 1028,60,70,true,2,"Diesel", "");
    this->addNewTank(QDate(2012, 8,18),km += 1029,60,70,true,1,"Diesel", "");
    this->addNewTank(QDate(2012, 8,28),km += 1018,60,70,true,3,"Diesel", "");
    this->addNewTank(QDate(2012, 9,18),km += 1011,60,70,true,1,"Diesel", "");
    this->addNewTank(QDate(2012, 9,28),km += 1012,60,70,true,2,"Diesel", "");
    this->addNewTank(QDate(2012,10, 1),km += 1013,60,70,true,1,"Diesel", "");
    winter1 = this->addNewTire(QDate(2012,10,9),"Pneu hiver AV","Michelin","Winter 2",160,2);
    winter2 = this->addNewTire(QDate(2014,10,9),"Pneu hiver AR","Michelin","Winter 2",160,2);
    this->umountTire(QDate(2012,10,10), km += 100, summer1);
    this->mountTire(QDate(2012,10,10), km, winter1);
    this->mountTire(QDate(2012,10,10), km, winter2);

    this->addNewTank(QDate(2012,10,22),km += 1013,60,70,true,1,"Diesel", "");
    this->addNewTank(QDate(2012,11,15),km += 1099,60,70,true,2,"Diesel", "");
    this->addNewTank(QDate(2012,11,29),km += 1080,60,70,true,3,"Diesel", "");
    this->addNewTank(QDate(2012,12,15),km += 1010,60,70,true,1,"Diesel", "");
    this->addNewTank(QDate(2012,12,29),km += 1071,60,70,true,2,"Diesel", "");
    this->addNewCost(QDate(2012,12,30),km += 15,"Vidange 60000",222);
    this->addNewTank(QDate(2013, 1,15),km += 1031,60,70,true,2,"Diesel", "");
    this->addNewTank(QDate(2013, 1,29),km += 1121,60,70,true,3,"Diesel", "");
    this->addNewTank(QDate(2013, 2,15),km += 1134,60,70,true,3,"Diesel","");
    this->addNewTank(QDate(2013, 2,27),km += 1021,60,70,true,1,"Diesel", "");
    this->addNewTank(QDate(2013, 3,15),km += 1051,60,70,true,1,"Diesel", "");
    this->addNewTank(QDate(2013, 4, 8),km += 1051,60,70,true,1,"Diesel", "");
    this->addNewTank(QDate(2013, 4,28),km += 1028,60,70,true,3,"Diesel", "");
    this->umountTire(QDate(2012,10,10), km, winter1);
    this->umountTire(QDate(2012,10,10), km, winter2);
    this->mountTire(QDate(2012,10,10), km += 100, summer1);

    this->addNewTank(QDate(2013, 5, 9),km += 1021,60,70,true,3,"Diesel", "");
    this->addNewTank(QDate(2013, 5,18),km += 1022,60,70,true,3,"Diesel", "");
    this->addNewTank(QDate(2013, 5,20),km += 1023,60,70,true,2,"Diesel", "");
    this->addNewTank(QDate(2013, 5,28),km += 1024,60,70,true,1,"Diesel", "");
    this->addNewTank(QDate(2013, 6,18),km += 1025,60,70,true,3,"Diesel", "");
    this->addNewTank(QDate(2013, 6,28),km += 1026,60,70,true,2,"Diesel", "");
    this->addNewTank(QDate(2013, 7,18),km += 1027,60,70,true,1,"Diesel", "");
    this->addNewTank(QDate(2013, 7,28),km += 1028,60,70,true,2,"Diesel", "");
    this->addNewTank(QDate(2013, 8,18),km += 1029,60,70,true,1,"Diesel", "");
    this->addNewTank(QDate(2013, 8,28),km += 1018,60,70,true,3,"Diesel", "");
    this->addNewTank(QDate(2013, 9,18),km += 1011,60,70,true,1,"Diesel", "");
    this->addNewTank(QDate(2013, 9,28),km += 1012,60,70,true,2,"Diesel", "");
    this->addNewTank(QDate(2013,10,22),km += 1013,60,70,true,1,"Diesel", "");
    this->addNewTank(QDate(2013,11,15),km += 1099,60,70,true,2,"Diesel", "");
    this->addNewTank(QDate(2013,11,29),km += 1080,60,70,true,3,"Diesel", "Latest simulation entry");
    this->umountTire(QDate(2012,10,10), km += 100, summer1,true);
    this->mountTire(QDate(2012,10,10), km, winter1);
    this->mountTire(QDate(2012,10,10), km, winter2);
    */
}

