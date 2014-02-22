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
#include "station.h"
#include <QDebug>


bool sortTankByDistance(const Tank *c1, const Tank *c2)
{
    return c1->distance() > c2->distance();
}

bool sortCostByDistance(const Cost *c1, const Cost *c2)
{
    return c1->distance() > c2->distance();
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

    if(query.exec("SELECT event,date(date),distance,quantity,price,full,station FROM TankList, Event WHERE TankList.event == Event.id;"))
    {
        while(query.next())
        {
            int id = query.value(0).toInt();
            QDate date = query.value(1).toDate();
            unsigned int distance = query.value(2).toInt();
            double quantity = query.value(3).toDouble();
            double price = query.value(4).toDouble();
            bool full = query.value(5).toBool();
            unsigned int station = query.value(6).toInt();
            Tank *tank = new Tank(id, date, distance, quantity, price, full, station, this);
            _tanklist.append(tank);
        }
        emit nbtankChanged(_tanklist.count());
        emit consumptionChanged(this->consumption());
        emit distanceChanged(this->distance());
    }
    else
    {
        qDebug() << query.lastError();
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

    if(query.exec("SELECT event,date,distance,cost,desc FROM CostList, Event WHERE CostList.event == Event.id;"))
    {
        while(query.next())
        {
            int id = query.value(0).toInt();
            QDate date = query.value(1).toDate();
            unsigned int distance = query.value(2).toInt();
            double price = query.value(3).toDouble();
            QString description = query.value(4).toString();
            Cost *cost = new Cost(date,distance,description,price,id,this);
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

Car::Car(CarManager *parent) : QObject(parent), _manager(parent)
{

}

Car::Car(QString name, CarManager *parent) : QObject(parent), _manager(parent), _name(name)
{
    this->db_init();
    if(this->db_get_version() < DB_VERSION)
    {
        qDebug() << "Create configuation database " << this->db_get_version() << " >> " << DB_VERSION;
    }
    else
    {
        qDebug() << "Database version " << this->db_get_version();
    }

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
    unsigned int totalConsumption = 0;

    foreach(Tank *tank, _tanklist)
    {
        if(tank->distance() > maxDistance)
            maxDistance = tank->distance();
        if(tank->distance() < minDistance)
            minDistance = tank->distance();
        totalConsumption += tank->quantity();
    }
    if(maxDistance == 0) return 0;
    return totalConsumption / ((maxDistance - minDistance)/ 100.0);
}

unsigned int Car::distance() const
{
    unsigned long int maxDistance = 0;

    foreach(Tank *tank, _tanklist)
    {
        if(tank->distance() > maxDistance)
            maxDistance = tank->distance();
    }
    return maxDistance;
}

QQmlListProperty<Tank> Car::tanks()
{
    return QQmlListProperty<Tank>(this, _tanklist);
}

QQmlListProperty<Station> Car::stations()
{
    return QQmlListProperty<Station>(this, _stationlist);
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

void Car::addNewTank(QDate date, unsigned int distance, double quantity, double price, bool full, unsigned int station)
{
    Tank *tank = new Tank(date, distance, quantity, price, full, station, this);
    _tanklist.append(tank);
    qSort(_tanklist.begin(), _tanklist.end(), sortTankByDistance);
    tank->save();
    emit nbtankChanged(_tanklist.count());
    emit consumptionChanged(this->consumption());
    emit distanceChanged(this->distance());
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
    emit distanceChanged(this->distance());
    emit tanksChanged();
    tank->deleteLater();
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

void Car::addNewCost(QDate date, unsigned int distance, QString description, double price)
{
    Cost *cost = new Cost(date,distance,description,price,-1,this);
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
    QSqlQuery query(db);
    QString sql = QString("DELETE FROM CostList WHERE id=%1;").arg(cost->id());
    if(query.exec(sql))
    {
        qDebug() << "DELETE Cost in database with id " << cost->id();
        db.commit();
    }
    else
    {
        qDebug() << "Error during DELETE Cost in database";
        qDebug() << query.lastError();
    }

    emit costsChanged();
    cost->deleteLater();
}

void Car::addNewTire(QDate buydate, QDate trashdate, QString name, QString manufacturer, QString model, double price, unsigned int quantity)
{
    Tire *tire = new Tire(buydate,trashdate,name,manufacturer,model,price,quantity,-1,this);
    _tirelist.append(tire);
    tire->save();
    emit tiresChanged();
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


void Car::simulation()
{
    unsigned int km = 0;
    this->addNewStation("Saint Pal Mairie");
    this->addNewStation("Super U Craponne");
    this->addNewStation("Auchan Villard");

    this->addNewTank(QDate(2012, 8,15),km += 850,52,70,true,0);
    this->addNewTank(QDate(2012, 8,29),km += 981,55,74,true,1);
    this->addNewTank(QDate(2012, 9,15),km += 1042,47,63,true,2);
    this->addNewTank(QDate(2012, 9,29),km += 1021,48,60,true,3);
    this->addNewTank(QDate(2012,10,15),km += 1051,60,70,true,1);
    this->addNewCost(QDate(2013,12, 1),km += 100,"Revision 5000",50);
    this->addNewTank(QDate(2012,10,29),km += 1101,60,70,true,1);
    this->addNewTank(QDate(2012,11,15),km += 1099,60,70,true,2);
    this->addNewTank(QDate(2012,11,29),km += 1080,60,70,true,3);
    this->addNewTank(QDate(2012,12,15),km += 1010,60,70,true,1);
    this->addNewTank(QDate(2012,12,29),km += 1071,60,70,true,2);
    this->addNewTank(QDate(2013, 1,15),km += 1031,60,70,true,2);
    this->addNewTank(QDate(2013, 1,29),km += 1121,60,70,true,3);
    this->addNewTank(QDate(2013, 2,15),km += 1134,60,70,true,3);
    this->addNewTank(QDate(2013, 2,27),km += 1021,60,70,true,1);
    this->addNewTank(QDate(2013, 3,15),km += 1051,60,70,true,1);
    this->addNewCost(QDate(2013, 3,18),km += 10, "Vidange 15000",220);
    this->addNewTank(QDate(2013, 3,28),km += 1028,60,70,true,3);
    this->addNewTank(QDate(2013, 4,18),km += 1021,60,70,true,3);
    this->addNewTank(QDate(2013, 4,28),km += 1022,60,70,true,3);
    this->addNewTank(QDate(2013, 5,18),km += 1023,60,70,true,2);
    this->addNewTank(QDate(2013, 5,28),km += 1024,60,70,true,1);
    this->addNewTank(QDate(2013, 6,18),km += 1025,60,70,true,3);
    this->addNewTank(QDate(2013, 6,28),km += 1026,60,70,true,2);
    this->addNewTank(QDate(2013, 7,18),km += 1027,60,70,true,1);
    this->addNewTank(QDate(2013, 7,28),km += 1028,60,70,true,2);
    this->addNewTank(QDate(2013, 8,18),km += 1029,60,70,true,1);
    this->addNewTank(QDate(2013, 8,28),km += 1018,60,70,true,3);
    this->addNewTank(QDate(2013, 9,18),km += 1011,60,70,true,1);
    this->addNewTank(QDate(2013, 9,28),km += 1012,60,70,true,2);
    this->addNewTank(QDate(2013,10, 1),km += 1013,60,70,true,1);
    this->addNewCost(QDate(2013,10, 8),km += 15,"Vidange 30000",220);


    this->addNewTire(QDate(2014,01,20),QDate(2014,01,20),"Pneu hiver","Michelin","Alpin A4",160,4);
    this->addNewTire(QDate(2014,01,20),QDate(2014,01,20),"Pneu hiver","Michelin","Winter 2",160,2);
    this->addNewTire(QDate(2014,01,20),QDate(2014,01,20),"Pneu hiver","Michelin","Winter 2",160,2);
    this->addNewTire(QDate(2014,02, 1),QDate(2014,02, 1),"Pneu été","Michelin","EnergySaver",110,4);
}

