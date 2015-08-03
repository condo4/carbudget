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
#include <stdlib.h>

#define CREATE_NEW_EVENT (0)

bool sortTankByDistance(const Tank *c1, const Tank *c2)
{
    return c1->distance() > c2->distance();
}

bool sortCostByDate(const Cost *c1, const Cost *c2)
{
    return c1->date() > c2->date();
}

bool sortCosttypeByName(const Costtype *c1, const Costtype *c2)
{
    return c1->name() < c2->name();
}


bool sortTiresetByName(const Tireset *c1, const Tireset *c2)
{
    return c1->name() < c2->name();
}

bool sortTireByDate(const Tire *c1, const Tire *c2)
{
    return c1->buydate() > c2->buydate();
}

bool sortFueltypeByName(const Fueltype *c1, const Fueltype *c2)
{
    return c1->name() < c2->name();
}

bool sortStationByQuantity(const Station *c1, const Station *c2)
{
    return c1->quantity() > c2->quantity();
}

bool sortTiremountByDistance (const Tiremount *s1, const Tiremount * s2)
{
    return s1->mountdistance() > s2->mountdistance();
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

void Car::migrateTiresToTiresets()
{
    // Migrates tires used before db versio 5 to tiresets
    // Each Tire will be migrated to a single tireset
    // If the mounted tires are equal to the amount of wheels, this tireset will be mounted
    QSqlQuery query(this->db);
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
            // first create corresponding tireset
            Tireset *tireset = new Tireset(this);
            tireset->setName("Migrated "+ name);
            tireset->save();
            _tiresetlist.append(tireset);
            Tire *tire = new Tire(buydate,trashdate,name,manufacturer,model,price,quantity,id,tireset->id(),this);
            tire->save();
            _tirelist.append(tire);
        }
    }
}

void Car::db_load()
{
    db_loading=true;
    QSqlQuery query(this->db);

    _tanklist.clear();
    _stationlist.clear();
    _tirelist.clear();
    _costlist.clear();
    _fueltypelist.clear();
    _costtypelist.clear();
    _tirelist.clear();
    _tiresetlist.clear();
    _tiremountlist.clear();
    if(query.exec("SELECT event,date(date),distance,quantity,price,full,station,fueltype,note FROM TankList, Event WHERE TankList.event == Event.id;"))
    {
        while(query.next())
        {
            qDebug() << "Adding tank " << query.value(2).toInt();
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
    if(query.exec("SELECT id,name,sum(TankList.quantity) as quantity FROM StationList, TankList WHERE StationList.id == TankList.station GROUP BY StationList.id;"))
    {
        while(query.next())
        {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            double quantity = query.value(2).toDouble();
            Station *station = new Station(id, name, quantity, this);
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
    if(query.exec("SELECT event,date,distance,costtype,cost,desc FROM CostList, Event WHERE CostList.event == Event.id;"))
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

    if(query.exec("SELECT id,name FROM TiresetList;"))
    {
        while(query.next())
        {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            Tireset *tireset = new Tireset(id, name, this);
            _tiresetlist.append(tireset);
        }
    }
    else
    {
        qDebug() << query.lastError();
    }
    if(query.exec("SELECT id,buydate,trashdate,price,name,manufacturer,model,quantity,tireset FROM TireList;"))
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
            int tireset = query.value(8).toInt();
            Tire *tire = new Tire(buydate,trashdate,name,manufacturer,model,price,quantity,id,tireset,this);
            _tirelist.append(tire);
            Tireset *t=findTiresetById(tireset);
            qDebug() << "Tireset is " << tireset;
            if (t) {
                if (!tire->trashed())  t->setTires_associated(t->tires_associated()+quantity);
            }
        }
    }
    else
    {
        qDebug() << query.lastError();
    }
    // Now load tire mountings
    // First load all unmount events (event_umount exists in events table)
    if(query.exec("SELECT m.id, m.date, m.distance, u.id, u.date, u.distance, t.tire, t.tireset FROM TireUsage t, Event m, Event u WHERE t.event_mount == m.id AND t.event_umount==u.id;"))
    {
        while(query.next())
        {
            int mount_id = query.value(0).toInt();
            QDate mount_date = query.value(1).toDate();
            unsigned int mount_distance = query.value(2).toInt();
            int unmount_id = query.value(3).toInt();
            QDate unmount_date = query.value(4).toDate();
            unsigned int unmount_distance = query.value(5).toInt();
            unsigned int tire = query.value(6).toInt();
            unsigned int tireset = query.value(7).toInt();
            Tiremount *tiremount = new Tiremount(mount_id,mount_date,mount_distance,unmount_id,unmount_date,unmount_distance,tire,tireset,this);
            _tiremountlist.append(tiremount);
        }
    }
    else
    {
        qDebug() << "Failed to load tiremounts: " << query.lastError();
    }
    // Now load mounted tires
    if(query.exec("SELECT m.id, m.date, m.distance, t.tire, t.tireset FROM TireUsage t, Event m WHERE t.event_mount == m.id AND t.event_umount==0;"))
    {
        while(query.next())
        {
            int mount_id = query.value(0).toInt();
            QDate mount_date = query.value(1).toDate();
            unsigned int mount_distance = query.value(2).toInt();
            unsigned int tire = query.value(3).toInt();
            unsigned int tireset = query.value(4).toInt();
            QDate unmount_date = QDate(1900,1,1);
            Tiremount *tiremount = new Tiremount(mount_id,mount_date,mount_distance,0,unmount_date,0,tire,tireset,this);
            _tiremountlist.append(tiremount);
        }
    }
    else
    {
        qDebug() << "Failed to load tiremounts: " << query.lastError();
    }
    if (!_tanklist.empty()) qSort(_tanklist.begin(),    _tanklist.end(),    sortTankByDistance);
    if (!_costlist.empty()) qSort(_costlist.begin(),    _costlist.end(),    sortCostByDate);
    if (!_stationlist.empty()) qSort(_stationlist.begin(), _stationlist.end(), sortStationByQuantity);
    if (!_fueltypelist.empty()) qSort(_fueltypelist.begin(), _fueltypelist.end(), sortFueltypeByName);
    if (!_costtypelist.empty()) qSort(_costtypelist.begin(), _costtypelist.end(), sortCosttypeByName);
    if (!_tirelist.empty()) qSort(_tirelist.begin(), _tirelist.end(), sortTireByDate);
    if (!_tiresetlist.empty()) qSort(_tiresetlist.begin(), _tiresetlist.end(), sortTiresetByName);
    if (!_tiremountlist.empty())  qSort(_tiremountlist.begin(),_tiremountlist.end(),sortTiremountByDistance);
    db_loading=false;
    nbtankChanged(_tanklist.count());
    emit consumptionChanged(this->consumption());
    emit consumptionmaxChanged(this->consumptionmax());
    emit consumptionlastChanged(this->consumptionlast());
    emit consumptionminChanged(this->consumptionmin());
    emit fueltotalChanged(this->fueltotal());
    emit maxdistanceChanged(this->maxdistance());
    emit mindistanceChanged(this->mindistance());
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
    QString sql = "ALTER TABLE TankList ADD COLUMN fueltype INTEGER;";
    QSqlQuery query(this->db);

    if(query.exec(sql))
    {
        if(query.exec("UPDATE CarBudget SET  value='3' WHERE id='version';"))
        {
            if (query.exec("CREATE TABLE FueltypeList (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);"))
            {
                this->db.commit();
                return;
            }
        }
    }
    this->db.rollback();
}

void Car::db_upgrade_to_4()
{
    QString sql = "ALTER TABLE CostList ADD COLUMN costtype INTEGER;";
    QSqlQuery query(this->db);

    if(query.exec(sql))
    {
        if(query.exec("UPDATE CarBudget SET  value='4' WHERE id='version';"))
        {
            if (query.exec("CREATE TABLE CosttypeList (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);"))
            {
                this->db.commit();
                return;
            }
        }
    }
    this->db.rollback();
}

void Car::db_upgrade_to_5()
{
    QString sql = "CREATE TABLE TiresetList (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT,nbtire INTEGER,mounted TINYINT);";
    QSqlQuery query(this->db);

    if(query.exec(sql))
    {
        if(query.exec("UPDATE CarBudget SET  value='5' WHERE id='version';"))
        {
            if (query.exec("UPDATE TABLE Tires ADD COLUMN tireset INTEGER;"))
            {
                if (query.exec("UPDATE TABLE Tireusage ADD COLUMN tireset INTEGER;"))
                {
                    this->db.commit();
                    migrateTiresToTiresets();
                    return;
                }
            }
        }
    }
    this->db.rollback();
}

Car::Car(CarManager *parent) : QObject(parent), _manager(parent)
{

}

Car::Car(QString name, CarManager *parent) : QObject(parent), _manager(parent), _name(name), _nbtire(0),_buyingprice(0),_sellingprice(0),_lifetime(0)
{
    this->db_init();
    db_loading=false;
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
        if(this->db_get_version() < 4)
        {
            db_upgrade_to_4();
        }
        if(this->db_get_version() < 5)
        {
            db_upgrade_to_5();
        }
    }
    qDebug() << "Database version " << this->db_get_version();

    this->db_load();

    this->_stationlist.append(new Station);
    qSort(_stationlist.begin(), _stationlist.end(), sortStationByQuantity);
    this->_fueltypelist.append(new Fueltype);
    qSort(_fueltypelist.begin(), _fueltypelist.end(), sortFueltypeByName);
    this->_costtypelist.append(new Costtype);
    qSort(_costtypelist.begin(), _costtypelist.end(), sortCosttypeByName);
    this->_tiresetlist.append(new Tireset());
    nbtire();
    buyingprice();
    sellingprice();
    lifetime();
}

unsigned int Car::nbtank() const
{
    return _tanklist.count();
}

double Car::consumption() const
{
    if (_tanklist.empty()) return 0.0;
    unsigned long int maxDistance = 0;
    unsigned long int minDistance = 999999999;
    double totalConsumption = 0;
    double partConsumption = 0;
    foreach(Tank *tank, _tanklist)
    {
        if(tank->full())
        {
            totalConsumption += partConsumption;
            partConsumption = 0;
            if(tank->distance() > maxDistance)
                maxDistance = tank->distance();
            if(tank->distance() < minDistance)
                minDistance = tank->distance();
        }
        if(maxDistance > 0)
            partConsumption += tank->quantity();

    }
    if((maxDistance == 0) || (maxDistance == minDistance)) return 0;
    return totalConsumption / ((maxDistance - minDistance)/ 100.0);
}

double Car::consumptionmax() const
{
    double con=0;
    foreach (Tank *tank,_tanklist)
    {
        if (tank->consumption()>con)
            con = tank->consumption();
    }
    return con;
}

double Car::consumptionlast() const
{
    if (_tanklist.empty()) return 0.0;
    QList<Tank*>::const_iterator tank = _tanklist.constBegin();
    if (*tank)  return (*tank)->consumption();
       else return 0;
}

double Car::consumptionmin() const
{
    double con=99999;
    foreach (Tank *tank,_tanklist)
    {
        if ((tank->consumption()<con)&&(tank->consumption()!=0))
            con = tank->consumption();
    }
    return con;
}


double Car::fueltotal() const
{
    double total=0;
    foreach (Tank *tank,_tanklist)
        total += tank->quantity();
    return total;
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

QQmlListProperty<Tireset> Car::tiresets()
{
    return QQmlListProperty<Tireset>(this, _tiresetlist);
}

QQmlListProperty<Tiremount> Car::tiremounts()
{
    return QQmlListProperty<Tiremount>(this, _tiremountlist);
}

const Tank *Car::previousTank(unsigned int distance) const
{
    const Tank *previous = NULL;
    unsigned int currentPrevDistance=0;
    foreach(Tank *tank, _tanklist)
    {
        if ((tank->distance() < distance) && (tank->distance() > currentPrevDistance))
        {
            previous = tank;
            currentPrevDistance=tank->distance();
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
    qSort(_stationlist.begin(), _stationlist.end(), sortStationByQuantity);
}

unsigned long int Car::getDistance(QDate date)
{
    // returns the approx distance at a date
    // currently simply of last event
    // needs to be improved
    unsigned long int dist=0;
    foreach(Tank *tank, _tanklist)
    {
        if (tank->date() < (QDateTime) date)
            break;
        dist=tank->distance();
    }
    return dist;
}

double Car::budget_fuel_byType(unsigned int id)
{
    // Returns average price of fuel type per 100Km
    // Not sure if this really makes sense but it makes the statistic views consistent
    double price =0;
    double quantity = 0;
    foreach (Tank *tank, _tanklist)
    {
        if (tank->fueltype()==id)
        {
            price += tank->price();
            quantity += tank->quantity();
        }
    }
    double average = 0;
    if (quantity!=0)
        average = price/quantity;
    return average*budget_consumption_byType(id);
}

double Car::budget_fuel_total_byType(unsigned int id)
{
    // Returns total price of all tankstops by Type
    double total=0;
    foreach(Tank *tank,_tanklist)
    {
        if(tank->fueltype()==id)
            total +=tank->price();
    }
    return total;
}
double Car::budget_consumption_byType(unsigned int id)
{
    /* Return sum(fuel price) / ODO * 100 for fueltype */
    // We will calculate only full refills as partial refills cannat be calculated correctly
    double totalDistance=0;
    double totalQuantity=0;
    //go to last tankstop
    Tank *curTank = _tanklist.first();
    const Tank *prevTank=NULL;
    while (curTank)
    {
        prevTank=previousTank(curTank->distance());
        if (!(prevTank==NULL))
        {
            //prevous tank must have correct fueltype
            if (prevTank->fueltype()==id)
                if (prevTank->full())
                {
                    totalDistance += curTank->distance()-prevTank->distance();
                    totalQuantity += curTank->quantity();
                }
            //cannot set curTank to prevTank as it is const
            curTank=NULL;
            foreach(Tank *tmp,_tanklist)
            {
                if (tmp->id()==prevTank->id())
                    curTank=tmp;
            }
        }
        else {
            curTank=NULL;
        }
    }
    return (totalDistance==0) ? 0.0 : (totalQuantity / totalDistance * 100.0);
}
double Car::budget_consumption_max_byType(unsigned int type)
{
    double con=0;
    foreach (Tank *tank,_tanklist)
    {
        if ((tank->consumption()>con)&&(tank->fueltype()==type))
            con = tank->consumption();
    }
    return con;
}
double Car::budget_consumption_min_byType(unsigned int type)
{
    double con=99999;
    foreach (Tank *tank,_tanklist)
    {
        if ((tank->consumption()<con)&&(tank->consumption()!=0)&& (tank->fueltype()==type))
            con = tank->consumption();
    }
    return (con==99999) ? 0 : con;
}
double Car::budget_cost_total_byType(unsigned int id)
{
    double total=0;
    foreach(Cost *cost,_costlist)
    {
        if(cost->costtype()==id)
            total +=cost->cost();
    }
    return total;
}
double Car::budget_cost_byType(unsigned int id)
{
    /* Return sum(cost) / ODO * 100 */
    double totalPrice = 0;

    foreach(Cost *cost, _costlist)
    {
        if (cost->costtype()==id)
            totalPrice += cost->cost();
    }
    if (maxdistance()==mindistance()) return 0;
    return totalPrice / ((maxdistance() - mindistance())/ 100.0);
}
double Car::budget_fuel_total()
{
    double total = 0;
    foreach (Tank *tank, _tanklist)
    {
        total += tank->price();
    }
    return total;
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
    if((maxDistance == 0) || (maxDistance == minDistance)) return 0;
    return totalPrice / ((maxDistance - minDistance)/ 100.0);
}
double Car::budget_cost_total()
{
    // returns total costs for all bills
    double total=0;
    foreach(Cost *cost,_costlist)
    {
        total += cost->cost();
    }
    return total;
}
double Car::budget_cost()
{
    //returns costs for bills per 100KM
    if (maxdistance() ==mindistance()) return 0;
    return budget_cost_total() / ((maxdistance() - mindistance())/ 100.0);
}
double Car::budget_invest_total()
{
    //returns buying costs
    return _buyingprice - _sellingprice;
}
double Car::budget_invest()
{
    //returns bying costs per 100 KM
    if (maxdistance()== mindistance()) return 0;
    QDate today = QDate::currentDate();
    unsigned int monthsused = 1;
    double valuecosts;
    if (maxdistance()==mindistance() ) return 0.0;
    if (_buyingdate.toString()=="")
    {
        qDebug() << "Invalid buying date ";
        double tmp = (_buyingprice-_sellingprice)/(maxdistance()-mindistance())*100.0;
        qDebug() << tmp;
        return tmp;
    }
    while (_buyingdate.addMonths(monthsused) < today)
    {
        monthsused++;
    }
    if ((monthsused < _lifetime) && (_lifetime !=0) )
        valuecosts = (_buyingprice - _sellingprice)*monthsused/_lifetime;
    else valuecosts = (_buyingprice - _sellingprice);
    return valuecosts / ((maxdistance() - mindistance())/ 100.0);
}
double Car::budget_tire()
{
    //returns tire costs per 100km
    if (maxdistance() == mindistance()) return 0;
    return budget_tire_total() / ((maxdistance() - mindistance())/ 100.0);
}
double Car::budget_tire_total()
{
    //returns total tire costs
    double total = 0;
    foreach (Tire *tire, _tirelist)
    {
        total += tire->price();
    }
    return total;
}

double Car::budget_total()
{
    //returns all costs
    return budget_cost_total() + budget_fuel_total() + budget_tire_total() + budget_invest_total();
}
double Car::budget()
{
    // Return total costs  / ODO * 100
    return budget_cost()+budget_fuel()+budget_invest()+budget_tire();
}

void Car::addNewTank(QDate date, unsigned int distance, double quantity, double price, bool full, unsigned int fueltype, unsigned int station, QString note)
{
    Tank *tank = new Tank(date, distance, quantity, price, full, fueltype, station, CREATE_NEW_EVENT,  note, this);
    _tanklist.append(tank);
    qSort(_tanklist.begin(), _tanklist.end(), sortTankByDistance);
    tank->save();
    if (!db_loading)
    {
        emit nbtankChanged(_tanklist.count());
        emit consumptionChanged(this->consumption());
        emit consumptionmaxChanged(this->consumptionmax());
        emit consumptionlastChanged(this->consumptionlast());
        emit consumptionminChanged(this->consumptionmin());
        emit fueltotalChanged(this->fueltotal());
        emit maxdistanceChanged(this->maxdistance());
        emit tanksChanged();
    }
}

void Car::delTank(Tank *tank)
{
    qDebug() << "Remove tank " << tank->id();
    _tanklist.removeAll(tank);
    if (!_tanklist.empty()) qSort(_tanklist.begin(), _tanklist.end(), sortTankByDistance);
    tank->remove();
    if (!db_loading)
    {
        emit nbtankChanged(_tanklist.count());
        emit consumptionChanged(this->consumption());
        emit consumptionmaxChanged(this->consumptionmax());
        emit consumptionlastChanged(this->consumptionlast());
        emit consumptionminChanged(this->consumptionmin());
        emit maxdistanceChanged(this->maxdistance());
        emit tanksChanged();
    }
    tank->deleteLater();
}


void Car::addNewFueltype(QString name)
{
    // check for existing Fueltype
    if (findFueltype(name))
        return;
    Fueltype *fueltype = new Fueltype(-1, name, this);
    _fueltypelist.append(fueltype);
    qSort(_fueltypelist.begin(), _fueltypelist.end(), sortFueltypeByName);
    fueltype->save();
    emit fueltypesChanged();
}

void Car::delFueltype(Fueltype *fueltype)
{
    qDebug() << "Remove Fuel Type " << fueltype->id();
    _fueltypelist.removeAll(fueltype);
    if (!_fueltypelist.empty()) qSort(_fueltypelist.begin(), _fueltypelist.end(), sortFueltypeByName);
    QSqlQuery query(db);
    QString sql = QString("UPDATE TankList SET Fueltype = 0 WHERE fueltype=%1;").arg(fueltype->id());

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
    //First check for existing station
    if (findStation(name))
        return;
    Station *station = new Station(-1, name, 0, this);
    _stationlist.append(station);
    qSort(_stationlist.begin(), _stationlist.end(), sortStationByQuantity);
    station->save();
    emit stationsChanged();
}

void Car::delStation(Station *station)
{
    qDebug() << "Remove Station " << station->id();
    _stationlist.removeAll(station);
    if (!_stationlist.empty()) qSort(_stationlist.begin(), _stationlist.end(), sortStationByQuantity);
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
    //First check for existing costtype
    if (findCosttype(name))
        return;
    Costtype *costtype = new Costtype(-1, name, this);
    _costtypelist.append(costtype);
    qSort(_costtypelist.begin(), _costtypelist.end(), sortCosttypeByName);
    costtype->save();
    emit costtypesChanged();
}

void Car::delCosttype(Costtype *costtype)
{
    qDebug() << "Remove Cost Type " << costtype->id();
    _costtypelist.removeAll(costtype);
    if (!_costtypelist.empty()) qSort(_costtypelist.begin(), _costtypelist.end(), sortCosttypeByName);
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
    qSort(_costlist.begin(), _costlist.end(), sortCostByDate);
    cost->save();
    qDebug() << "Price for new cost: " << price;
    emit costsChanged();
}

void Car::delCost(Cost *cost)
{
    qDebug() << "Remove Cost " << cost->id();
    _costlist.removeAll(cost);
    if (!_costlist.empty()) qSort(_costlist.begin(), _costlist.end(), sortCostByDate);
    cost->remove();
    emit costsChanged();
    cost->deleteLater();
}

Tire *Car::addNewTire(QDate buydate, QString name, QString manufacturer, QString model, double price, unsigned int quantity, int tireset)
{
    Tire *tire = new Tire(buydate,buydate,name,manufacturer,model,price,quantity,-1,tireset,this);
    _tirelist.append(tire);
    tire->save();
    emit tiresChanged();
    qSort(_tirelist.begin(), _tirelist.end(), sortTireByDate);
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
    if (!_tirelist.empty()) qSort(_tirelist.begin(), _tirelist.end(), sortTireByDate);
}

QString Car::getTireName(unsigned int id)
{
    foreach (Tire *tire, _tirelist)
    {
        if (tire->id()==id)
            return tire->name();
    }
    return "";
}
Tire* Car::findTireById(unsigned int id)
{
    foreach (Tire *tire, _tirelist)
    {
        if (tire->id()==id)
            return tire;
    }
    return NULL;
}


void Car::addNewTireset(QString name)
{
    //First check for existing costtype
    if (findTireset(name))
        return;
    Tireset *tireset = new Tireset(-1, name, this);
    _tiresetlist.append(tireset);
    qSort(_tiresetlist.begin(), _tiresetlist.end(), sortTiresetByName);
    tireset->save();
    emit tiresetsChanged();
}

Tireset* Car::findTireset(QString name)
{
    foreach (Tireset *tireset, _tiresetlist)
    {
        if (tireset->name()==name)
            return tireset;
    }
    return NULL;
}

void Car::updateTiresets()
{
    emit tiresChanged();
}

Tireset* Car::findTiresetById(unsigned int id)
{
    foreach (Tireset *tireset, _tiresetlist)
    {
        if (tireset->id()==id)
            return tireset;
    }
    return NULL;
}

QString Car::getTiresetName(unsigned int id)
{
    foreach (Tireset *tireset, _tiresetlist)
    {
        if (tireset->id()==id)
            return tireset->name();
    }
    return "";
}

void Car::mountTireset(QDate mountdate, unsigned int distance, Tireset *tireset)
{
    qDebug() << "Mount tire";
    QSqlQuery query(db);

    if(!tireset->mountable())
    {
        qDebug() << "Can't mount this tire";
        return;
    }

    int id;
    QString sql = QString("INSERT INTO Event (id,date,distance) VALUES(NULL,'%1',%2)").arg(mountdate.toString("yyyy-MM-dd 00:00:00.00")).arg(distance);
    if(query.exec(sql))
    {
        id = query.lastInsertId().toInt();
        qDebug() << "Create Event(mount) in database with id " << id;
        // First umnount all mounted tires
        qDebug() << "Unmounting all tires";
        foreach (Tire *t, _tirelist) {
            if (t->mounted())
            {
                QString sql2 = QString("UPDATE TireUsage SET event_umount=%1 WHERE tire=%2 AND event_umount=0").arg(id).arg(t->id());
                if(query.exec(sql2))
                {
                    qDebug() << "Update TireUsage in database";
                    //Now modify tiremountlist
                    foreach (Tiremount *tm, _tiremountlist)
                    {
                        if ((tm->unmountid()==0) && (tm->tire()==t->id()))
                        {
                            CarEvent *ev = new CarEvent(mountdate,distance,id,this);
                            tm->setUnmountEvent(ev);
                        }
                    }
                }
                else {
                    id =-1;
                    break;
                }
            }
        }
        // Now loop through tires to check which ones are associated with current tireset
        qDebug() << " Mounting all associated tires";
        foreach (Tire *t, _tirelist) {
            if ((t->tireset()==tireset->id()) && (!t->trashed()))
            {
                QString sql3 = QString("INSERT INTO TireUsage (event_mount,event_umount,tire,tireset) VALUES(%1,0,%2,%3)").arg(id).arg(t->id()).arg(t->tireset());
                if(query.exec(sql3))
                {
                    id = query.lastInsertId().toInt();
                    qDebug() << "Create TireUsage in database with id " << id;
                    // Now add new mount to the tiremountlist
                    Tiremount *tiremount = new Tiremount(id,mountdate,distance,0,QDate(1900,1,1),0,t->id(),t->tireset(),this);
                    _tiremountlist.append(tiremount);
                }
                else {
                    id = -1;
                    break;
                }
            }
        }
    }
    else id =-1;
    if(id == -1)
    {
        qDebug() << "Can't mount this tire set (db error)";
        qDebug() << query.lastError();
    }
    else {
        qSort(_tiremountlist.begin(),_tiremountlist.end(),sortTiremountByDistance);
        emit tireMountedChanged();
        db.commit();
    }
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

        QString sql2 = QString("INSERT INTO TireUsage (event_mount,event_umount,tire,tireset) VALUES(%1,0,%2,%3)").arg(id).arg(tire->id()).arg(tire->tireset());
        if(query.exec(sql2))
        {
            id = query.lastInsertId().toInt();
            qDebug() << "Create TireUsage in database with id " << id;
            // Now add new mount to the tiremountlist
            Tiremount *tiremount = new Tiremount(id,mountdate,distance,0,QDate(1900,1,1),0,tire->id(),tire->tireset(),this);
            _tiremountlist.append(tiremount);
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
        qDebug() << "Create Event(Tirmount) in database with id " << id;

        QString sql2 = QString("UPDATE TireUsage SET event_umount=%1 WHERE tire=%2 AND event_umount=0").arg(id).arg(tire->id());
        if(query.exec(sql2))
        {
            qDebug() << "Update TireUsage in database";
            //Now modify tiremountlist
            foreach (Tiremount *tm, _tiremountlist)
            {
                if ((tm->unmountid()==0) && (tm->tire()==tire->id()))
                {
                    CarEvent *ev = new CarEvent(umountdate,distance,id,this);
                    tm->setUnmountEvent(ev);
                }
            }

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
                tire->setTrashdate(umountdate);
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

QString Car::tiresetMounted() const
{
    if (_tiresetlist.isEmpty()) return "";
    foreach (Tireset *t , _tiresetlist)
    {
        if (t->mounted())
        {
            return t->name();
        }
        return "";
    }

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

        qDebug() << "Change currency in database: " << _currency << " >> " << currency;
    }
    _currency = currency;
    emit currencyChanged();
}

QString Car::distanceunity()
{
    if(_distanceunity.length() < 1)
    {
        QSqlQuery query(this->db);

        if(query.exec("SELECT value FROM CarBudget WHERE id='distanceunity';"))
        {
            query.next();
            _distanceunity = query.value(0).toString();
            qDebug() << "Find distanceunity in database: " << _distanceunity;
        }
        if(_distanceunity.length() < 1)
        {
            qDebug() << "Default distanceunity not set in database, set to Km";
            query.exec("INSERT INTO CarBudget (id, value) VALUES ('distanceunity','Km');");
            _distanceunity = "Km";
        }
    }

    return _distanceunity;
}

void Car::setDistanceunity(QString distanceunity)
{
    QSqlQuery query(this->db);

    if(query.exec("SELECT count(*) FROM CarBudget WHERE id='distanceunity';"))
    {
        query.next();
        if(query.value(0).toString().toInt() < 1)
            query.exec(QString("INSERT INTO CarBudget (id, value) VALUES ('distanceunity','%1');").arg(distanceunity));
        else
            query.exec(QString("UPDATE CarBudget SET  value='%1' WHERE id='distanceunity';").arg(distanceunity));

        qDebug() << "Change distanceunity in database: " << _distanceunity << " >> " << distanceunity;
    }
    _distanceunity = distanceunity;
    emit distanceunityChanged();
}

unsigned int Car::nbtire()
{
    if(_nbtire == 0)
    {
        QSqlQuery query(this->db);

        if(query.exec("SELECT value FROM CarBudget WHERE id='nbtire';"))
        {
            query.next();
            _nbtire = query.value(0).toInt();
            qDebug() << "Find nbtire in database: " << _nbtire;
        }
        if(_nbtire == 0)
        {
            qDebug() << "Default nbtire not set in database, set to 4";
            query.exec("INSERT INTO CarBudget (id, value) VALUES ('nbtire','4');");
            _nbtire = 4;
        }
    }

    return _nbtire;
}

void Car::setNbtire(unsigned int nbtire)
{
    QSqlQuery query(this->db);

    if(query.exec("SELECT count(*) FROM CarBudget WHERE id='nbtire';"))
    {
        query.next();
        if(query.value(0).toString().toInt() < 1)
            query.exec(QString("INSERT INTO CarBudget (id, value) VALUES ('nbtire','%1');").arg(nbtire));
        else
            query.exec(QString("UPDATE CarBudget SET  value='%1' WHERE id='nbtire';").arg(nbtire));

        qDebug() << "Change number of tires in database: " << _nbtire;
    }
    _nbtire = nbtire;
    emit nbtireChanged();
}

double Car::buyingprice()
{
    if(_buyingprice == 0)
    {
        QSqlQuery query(this->db);

        if(query.exec("SELECT value FROM CarBudget WHERE id='buyingprice';"))
        {
            query.next();
            _buyingprice = query.value(0).toDouble();
            qDebug() << "Find buyingprice in database: " << _buyingprice;
        }
        if(_buyingprice == 0)
        {
            qDebug() << "Buying price not set in database, set to 0";
            query.exec("INSERT INTO CarBudget (id, value) VALUES ('buyingprice','0');");
            _buyingprice = 0;
        }
    }
    return _buyingprice;
}

void Car::setBuyingprice(double price)
{
    QSqlQuery query(this->db);

    if(query.exec("SELECT count(*) FROM CarBudget WHERE id='buyingprice';"))
    {
        query.next();
        if(query.value(0).toString().toInt() < 1)
            query.exec(QString("INSERT INTO CarBudget (id, value) VALUES ('buyingprice','%1');").arg(price));
        else
            query.exec(QString("UPDATE CarBudget SET  value='%1' WHERE id='buyingprice';").arg(price));

        qDebug() << "Change buyingprice in database: " << price;
    }
    _buyingprice = price;
    emit buyingpriceChanged();
}

double Car::sellingprice()
{
    if(_sellingprice == 0)
    {
        QSqlQuery query(this->db);

        if(query.exec("SELECT value FROM CarBudget WHERE id='sellingprice';"))
        {
            query.next();
            _sellingprice = query.value(0).toDouble();
            qDebug() << "Find sellingprice in database: " << _sellingprice;
        }
        if(_sellingprice == 0)
        {
            qDebug() << "Selling price not set in database, set to 0";
            query.exec("INSERT INTO CarBudget (id, value) VALUES ('sellingprice','0');");
            _sellingprice = 0;
        }
    }
    return _sellingprice;
}

void Car::setSellingprice(double price)
{
    QSqlQuery query(this->db);

    if(query.exec("SELECT count(*) FROM CarBudget WHERE id='sellingprice';"))
    {
        query.next();
        if(query.value(0).toString().toInt() < 1)
            query.exec(QString("INSERT INTO CarBudget (id, value) VALUES ('sellingprice','%1');").arg(price));
        else
            query.exec(QString("UPDATE CarBudget SET  value='%1' WHERE id='sellingprice';").arg(price));

        qDebug() << "Change sellingprice in database: " << price;
    }
    _sellingprice = price;
    emit sellingpriceChanged();
}

unsigned int Car::lifetime()
{
    if(_lifetime == 0)
    {
        QSqlQuery query(this->db);

        if(query.exec("SELECT value FROM CarBudget WHERE id='lifetime';"))
        {
            query.next();
            _lifetime = query.value(0).toInt();
            qDebug() << "Find lifetime in database: " << _lifetime;
        }
        if(_lifetime == 0)
        {
            qDebug() << "Default lifetime not set in database, set to 0";
            query.exec("INSERT INTO CarBudget (id, value) VALUES ('lifetime','0');");
            _lifetime = 0;
        }
    }
    return _lifetime;
}

void Car::setLifetime(int months)
{
    QSqlQuery query(this->db);

    if(query.exec("SELECT count(*) FROM CarBudget WHERE id='lifetime';"))
    {
        query.next();
        if(query.value(0).toString().toInt() < 1)
            query.exec(QString("INSERT INTO CarBudget (id, value) VALUES ('lifetime','%1');").arg(months));
        else
            query.exec(QString("UPDATE CarBudget SET  value='%1' WHERE id='lifetime';").arg(months));

        qDebug() << "Change lifetime in database: " << months;
    }
    _lifetime = months;
    emit lifetimeChanged();
}

QDate Car::buyingdate()
{
        QSqlQuery query(this->db);

        if(query.exec("SELECT value FROM CarBudget WHERE id='buyingdate';"))
        {
            query.next();
            _buyingdate = QDate::fromString(query.value(0).toString());
            qDebug() << "Find buying date in database: " << _buyingdate;
        }
        else
        {
            qDebug() << "buying date not set in database, set to today";
            query.exec(QString("INSERT INTO CarBudget (id, value) VALUES ('buyingdate','%1');").arg(QDate::currentDate().toString()));
            _buyingdate = QDate::currentDate();
        }
    return _buyingdate;
}

void Car::setBuyingdate(QDate date)
{
    QSqlQuery query(this->db);
    qDebug() << "setBuyingdate invoked";
    if(query.exec("SELECT count(*) FROM CarBudget WHERE id='buyingdate';"))
    {
        query.next();
        if(query.value(0).toString().toInt() < 1)
            query.exec(QString("INSERT INTO CarBudget (id, value) VALUES ('buyingdate','%1');").arg(date.toString()));
        else
            query.exec(QString("UPDATE CarBudget SET  value='%1' WHERE id='buyingdate';").arg(date.toString()));

        qDebug() << "Change buying date in database: " << date;
    }
    _buyingdate = date;
    emit buyingdateChanged();
}

void Car::simulation()
{
    Tire *winter1, *winter2, *summer1;

    unsigned int km = 0;
    this->addNewStation("Saint Pal Mairie");
    this->addNewStation("Super U Craponne");
    this->addNewStation("Auchan Villard");
    this->addNewTireset("Winter Tires");

    winter1 = this->addNewTire(QDate(2010,11,15),"Pneu hiver","Michelin","Alpin A4",160,4,1);
    this->mountTire(QDate(2012,11,15), km, winter1);

    this->addNewTank(QDate(2010,11,15),km += 850,52,70,true,0, 0,"t1");
    this->addNewTank(QDate(2010,11,29),km += 981,55,74,true,0, 1,"t2");
    this->addNewTank(QDate(2010,11,15),km += 1042,47,63,true,0, 2,"t3");
    this->addNewTank(QDate(2010,11,29),km += 1021,48,60,true,0, 3,"t4");
    this->addNewTank(QDate(2010,12,15),km += 1051,60,70,true,0, 1,"t5");
    this->addNewCost(QDate(2011, 1, 1),km += 100, 0, "Revision 5000",50);
    this->addNewTank(QDate(2011, 1,29),km += 1101,60,70,true,0, 1,"t6");
    this->addNewTank(QDate(2011, 1,15),km += 1099,60,70,true,0, 2,"t7");
    this->addNewTank(QDate(2011, 1,29),km += 1080,60,70,true,0, 3,"t8");
    this->addNewTank(QDate(2011, 2,15),km += 1010,60,70,true,0, 1,"t9");

    this->addNewFueltype("Diesel");

    this->addNewTank(QDate(2011, 2,29),km += 1071,60,70,true,1, 2,"t10");
    this->addNewTank(QDate(2011, 3, 5),km += 1031,60,70,true,1, 2,"t11");
    this->addNewTank(QDate(2011, 3,19),km += 1121,60,70,true,1, 3,"");
    this->addNewTank(QDate(2011, 3,25),km += 1134,60,70,true,1, 3,"");
    this->addNewTank(QDate(2011, 3,30),km += 1021,60,70,true,1, 1,"");
    this->umountTire(QDate(2011, 4, 5), km += 100, winter1);
    summer1 = this->addNewTire(QDate(2011,4,5),"Pneu été","Michelin","EnergySaver",110,4,1);
    this->mountTire(QDate(2011,4,5), km, summer1);

    this->addNewTank(QDate(2011, 4, 8),km += 1051,60,70,true,1, 1,"");
    this->addNewCost(QDate(2011, 4,18),km += 10, 0, "Vidange 15000",220);
    this->addNewTank(QDate(2011, 4,28),km += 1028,60,70,true,1, 3,"");
    this->addNewTank(QDate(2011, 5, 9),km += 1021,60,70,true,1, 3,"");
    this->addNewTank(QDate(2011, 5,18),km += 1022,60,70,true,1, 3,"");
    this->addNewTank(QDate(2011, 5,20),km += 1023,60,70,true,1, 2,"");
    this->addNewTank(QDate(2011, 5,28),km += 1024,60,70,true,1, 1,"");
    this->addNewTank(QDate(2011, 6,18),km += 1025,60,70,true,1, 3,"");
    this->addNewTank(QDate(2011, 6,28),km += 1026,60,70,true,1, 2,"");
    this->addNewTank(QDate(2011, 7,18),km += 1027,60,70,true,1, 1,"A new note");
    this->addNewTank(QDate(2011, 7,28),km += 1028,60,70,true,1, 2,"");
    this->addNewTank(QDate(2011, 8,18),km += 1029,60,70,true,1, 1,"");
    this->addNewTank(QDate(2011, 8,28),km += 1018,60,70,true,1, 3,"");

    this->addNewFueltype("Super Diesel");

    this->addNewTank(QDate(2011, 9,18),km += 1011,60,70,true,1, 1,"");
    this->addNewTank(QDate(2011, 9,28),km += 1012,60,70,true,2, 2,"");
    this->addNewTank(QDate(2011,10, 1),km += 1013,60,70,true,2, 1,"");
    this->addNewCost(QDate(2011,10, 8),km += 15, 0, "Vidange 30000",220);
    this->addNewTank(QDate(2011,10,29),km += 1101,60,70,true,1, 1,"");
    this->umountTire(QDate(2011,10,30), km += 100, summer1);
    this->mountTire(QDate(2011,10,30), km, winter1);

    this->addNewTank(QDate(2011,11,15),km += 1099,60,70,true,2, 2,"");
    this->addNewTank(QDate(2011,11,29),km += 1080,60,70,true,1, 3,"");
    this->addNewTank(QDate(2011,12,15),km += 1010,60,70,true,1, 1,"");
    this->addNewTank(QDate(2011,12,29),km += 1071,60,70,true,2, 2,"");
    this->addNewTank(QDate(2012, 1,15),km += 1031,60,70,true,1, 2,"");
    this->addNewTank(QDate(2012, 1,29),km += 1121,60,70,true,1, 3,"");
    this->addNewTank(QDate(2012, 2,15),km += 1134,60,70,true,2, 3,"");
    this->addNewTank(QDate(2012, 2,27),km += 1021,60,70,true,2, 1,"");
    this->addNewTank(QDate(2012, 3,15),km += 1051,60,70,true,1, 1,"");
    this->addNewTank(QDate(2012, 4, 8),km += 1051,60,70,true,1, 1,"");
    this->addNewTank(QDate(2012, 4,28),km += 1028,60,70,true,2, 3,"");/*43*/
    this->addNewTank(QDate(2012, 5, 9),km += 1021,60,70,true,1, 3,"");
    this->addNewTank(QDate(2012, 5,18),km += 1022,60,70,true,1, 3,"");
    this->addNewTank(QDate(2012, 5,20),km += 1023,60,70,true,1, 2,"");
    this->addNewTank(QDate(2012, 5,28),km += 1024,60,70,true,2, 1,"");
    this->umountTire(QDate(2012, 5,30), km += 100,winter1,true); /* Trash it */
    this->mountTire(QDate(2012, 5,30), km,  summer1);

    this->addNewTank(QDate(2012, 6,18),km += 1025,60,70,true,1, 3,"");
    this->addNewTank(QDate(2012, 6,28),km += 1026,60,70,true,1, 2,"");
    this->addNewTank(QDate(2012, 7,18),km += 1027,60,70,true,1, 1,"");
    this->addNewTank(QDate(2012, 7,28),km += 1028,60,70,true,2, 2,"");
    this->addNewTank(QDate(2012, 8,18),km += 1029,60,70,true,2, 1,"");
    this->addNewTank(QDate(2012, 8,28),km += 1018,60,70,true,2, 3,"");
    this->addNewTank(QDate(2012, 9,18),km += 1011,60,70,true,1, 1,"");
    this->addNewTank(QDate(2012, 9,28),km += 1012,60,70,true,2, 2,"");
    this->addNewTank(QDate(2012,10, 1),km += 1013,60,70,true,1, 1,""); /* 55 */
    winter1 = this->addNewTire(QDate(2012,10,9),"Pneu hiver AV","Michelin","Winter 2",160,2,1);
    winter2 = this->addNewTire(QDate(2014,10,9),"Pneu hiver AR","Michelin","Winter 2",160,2,1);
    this->umountTire(QDate(2012,10,10), km += 100, summer1);
    this->mountTire(QDate(2012,10,10), km, winter1);
    this->mountTire(QDate(2012,10,10), km, winter2);

    this->addNewTank(QDate(2012,10,22),km += 1013,60,70,true,1, 1,"");
    this->addNewTank(QDate(2012,11,15),km += 1099,60,70,true,2, 2,"");
    this->addNewTank(QDate(2012,11,29),km += 1080,60,70,true,1, 3,"");
    this->addNewTank(QDate(2012,12,15),km += 1010,60,70,true,1, 1,"");
    this->addNewTank(QDate(2012,12,29),km += 1071,60,70,true,1, 2,"");
    this->addNewCost(QDate(2012,12,30),km += 15, 0, "Vidange 60000",222);
    this->addNewTank(QDate(2013, 1,15),km += 1031,60,70,true,1, 2,"");
    this->addNewTank(QDate(2013, 1,29),km += 1121,60,70,true,1, 3,"");
    this->addNewTank(QDate(2013, 2,15),km += 1134,60,70,true,1, 3,"");
    this->addNewTank(QDate(2013, 2,27),km += 1021,60,70,true,1, 1,"");
    this->addNewTank(QDate(2013, 3,15),km += 1051,60,70,true,2, 1,"");
    this->addNewTank(QDate(2013, 4, 8),km += 1051,60,70,true,2, 1,"");
    this->addNewTank(QDate(2013, 4,28),km += 1028,60,70,true,1, 3,"");
    this->umountTire(QDate(2012,10,10), km, winter1);
    this->umountTire(QDate(2012,10,10), km, winter2);
    this->mountTire(QDate(2012,10,10), km += 100, summer1);

    this->addNewTank(QDate(2013, 5, 9),km += 1021,60,70,true,1, 3,"");
    this->addNewTank(QDate(2013, 5,18),km += 1022,60,70,true,1, 3,"");
    this->addNewTank(QDate(2013, 5,20),km += 1023,60,70,true,2, 2,"");
    this->addNewTank(QDate(2013, 5,28),km += 1024,60,70,true,2, 1,"");
    this->addNewTank(QDate(2013, 6,18),km += 1025,60,70,true,1, 3,"");
    this->addNewTank(QDate(2013, 6,28),km += 1026,60,70,true,1, 2,"");
    this->addNewTank(QDate(2013, 7,18),km += 1027,60,70,true,1, 1,"");
    this->addNewTank(QDate(2013, 7,28),km += 1028,60,70,true,1, 2,"");
    this->addNewTank(QDate(2013, 8,18),km += 1029,60,70,true,1, 1,"");
    this->addNewTank(QDate(2013, 8,28),km += 1018,60,70,true,2, 3,"");
    this->addNewTank(QDate(2013, 9,18),km += 1011,60,70,true,2, 1,"");
    this->addNewTank(QDate(2013, 9,28),km += 1012,60,70,true,2, 2,"");
    this->addNewTank(QDate(2013,10,22),km += 1013,60,70,true,1, 1,"");
    this->addNewTank(QDate(2013,11,15),km += 1099,60,70,true,1, 2,"");
    this->addNewTank(QDate(2013,11,29),km += 1080,60,70,true,1, 3,"Latest simulation entry");
    this->umountTire(QDate(2012,10,10), km += 100, summer1,true); /* Trash it */
    this->mountTire(QDate(2012,10,10), km, winter1);
    this->mountTire(QDate(2012,10,10), km, winter2);
}

