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
 * Authors: Fabien Proriol, Matti Viljanen
 */

#include "car.h"
#include "tank.h"
#include "cost.h"
#include "carmanager.h"
#include "fueltype.h"
#include "station.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <stdlib.h>


#define CREATE_NEW_EVENT (0)

bool sortTankByDistance(const Tank *c1, const Tank *c2) { return c1->distance() > c2->distance(); }
bool sortCostByDate(const Cost *c1, const Cost *c2) { return c1->date() > c2->date(); }
bool sortCostTypeById(const CostType *c1, const CostType *c2) { return c1->id() < c2->id(); }
bool sortFuelTypeById(const FuelType *c1, const FuelType *c2) { return c1->id() < c2->id(); }
bool sortStationByQuantity(const Station *c1, const Station *c2) { return c1->quantity() > c2->quantity(); }
bool sortTireMountByDistance (const TireMount *s1, const TireMount * s2) { return s1->mountDistance() > s2->mountDistance(); }

void Car::_dbInit()
{
    if(this->db.contains("CarManagerDatabase")) {
        this->db.close();
        QSqlDatabase::removeDatabase("CarManagerDatabase");
    }

    this->db = QSqlDatabase::addDatabase("QSQLITE", "CarManagerDatabase");

    QString db_name = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + _name + ".cbg";
    this->db.setDatabaseName(db_name);

    bool databaseOK = this->db.open();
    qDebug() << "Opening database file" << db_name << (databaseOK ? "succeeded" : "failed");
}

void Car::_dbLoad()
{
    qDebug() << "Loading database...";
    _dbLoading=true;
    QSqlQuery query(this->db);

    _tankList.clear();
    _stationList.clear();
    _costList.clear();
    _fuelTypeList.clear();
    _costTypeList.clear();
    _tireList.clear();
    _tireMountList.clear();
    if(query.exec("SELECT event,date(date),distance,quantity,price,full,station,fueltype,note FROM TankList, Event WHERE TankList.event == Event.id;"))
    {
        while(query.next())
        {
            //qDebug() << "Adding tank" << query.value(2).toInt();
            int id = query.value(0).toInt();
            QDate date = query.value(1).toDate();
            unsigned int distance = query.value(2).toInt();
            double quantity = query.value(3).toDouble();
            double price = query.value(4).toDouble();
            bool full = query.value(5).toBool();
            unsigned int station = query.value(6).toInt();
            unsigned int fuelType = query.value(7).toInt();
            QString note = query.value(8).toString();
            Tank *tank = new Tank(date, distance, quantity, price, full, fuelType, station, id, note, this);
            _tankList.append(tank);
        }

    }
    else if(query.size() > 0)
    {
        qDebug() << query.lastError();
    }
    if(query.exec("SELECT id,name FROM FueltypeList;"))
    {
        while(query.next())
        {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            FuelType *fuelType = new FuelType(id, name, this);
            _fuelTypeList.append(fuelType);
        }
    }
    else if(query.size() > 0)
    {
        qDebug() << query.lastError();
    }
    if(query.exec("SELECT id,name,sum(TankList.quantity) as quantity FROM StationList LEFT JOIN TankList ON StationList.id == TankList.station GROUP BY StationList.id;"))
    {
        while(query.next())
        {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            double quantity = query.value(2).toDouble();
            Station *station = new Station(id, name, quantity, this);
            _stationList.append(station);
        }
    }
    else if(query.size() > 0)
    {
        qDebug() << query.lastError();
    }
    if(query.exec("SELECT id,name FROM CosttypeList;"))
    {
        while(query.next())
        {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            CostType *costType = new CostType(id, name, this);
            _costTypeList.append(costType);
        }
    }
    else if(query.size() > 0)
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
            unsigned int costType = query.value(3).toInt();
            double price = query.value(4).toDouble();
            QString description = query.value(5).toString();
            Cost *cost = new Cost(date,distance,costType,description,price,id,this);
            _costList.append(cost);
        }
    }
    else if(query.size() > 0)
    {
        qDebug() << query.lastError();
    }

    if(query.exec("SELECT id,buydate,trashdate,price,name,manufacturer,model,quantity FROM TireList;"))
    {
        while(query.next())
        {
            int id = query.value(0).toInt();
            QDate buyDate = query.value(1).toDate();
            QDate trashDate = query.value(2).toDate();
            double price = query.value(3).toDouble();
            QString name = query.value(4).toString();
            QString manufacturer = query.value(5).toString();
            QString model = query.value(6).toString();
            unsigned int quantity = query.value(7).toInt();
            Tire *tire = new Tire(buyDate,trashDate,name,manufacturer,model,price,quantity,id,this);
            _tireList.append(tire);
        }
    }
    else if(query.size() > 0)
    {
        qDebug() << query.lastError();
    }
    // Now load tire mountings
    // First load all unmount events (event_umount exists in events table)
    if(query.exec("SELECT m.id, m.date, m.distance, u.id, u.date, u.distance, t.tire FROM TireUsage t, Event m, Event u WHERE t.event_mount == m.id AND t.event_umount==u.id;"))
    {
        while(query.next())
        {
            int mountId = query.value(0).toInt();
            QDate mountDate = query.value(1).toDate();
            unsigned int mountDistance = query.value(2).toInt();
            int unmountId = query.value(3).toInt();
            QDate unmountDate = query.value(4).toDate();
            unsigned int unmountDistance = query.value(5).toInt();
            unsigned int tire = query.value(6).toInt();
            TireMount *tireMount = new TireMount(mountId,mountDate,mountDistance,unmountId,unmountDate,unmountDistance,tire,this);
            _tireMountList.append(tireMount);
        }
    }
    else if(query.size() > 0)
    {
        qDebug() << "Failed to load tire unmounts:" << query.lastError();
    }
    // Now load mounted tires
    if(query.exec("SELECT m.id, m.date, m.distance, t.tire FROM TireUsage t, Event m WHERE t.event_mount == m.id AND t.event_umount==0;"))
    {
        while(query.next())
        {
            int mountId = query.value(0).toInt();
            QDate mountDate = query.value(1).toDate();
            unsigned int mountDistance = query.value(2).toInt();
            unsigned int tire = query.value(3).toInt();
            QDate unmountDate = QDate(1900,1,1);
            TireMount *tireMount = new TireMount(mountId,mountDate,mountDistance,0,unmountDate,0,tire,this);
            _tireMountList.append(tireMount);
        }
    }
    else if(query.size() > 0)
    {
        qDebug() << "Failed to load tire mounts:" << query.lastError();
    }
    if (!_tankList.empty()) qSort(_tankList.begin(),    _tankList.end(),    sortTankByDistance);
    if (!_costList.empty()) qSort(_costList.begin(),    _costList.end(),    sortCostByDate);
    if (!_stationList.empty()) qSort(_stationList.begin(), _stationList.end(), sortStationByQuantity);
    if (!_fuelTypeList.empty()) qSort(_fuelTypeList.begin(), _fuelTypeList.end(), sortFuelTypeById);
    if (!_costTypeList.empty()) qSort(_costTypeList.begin(), _costTypeList.end(), sortCostTypeById);
    if (!_tireMountList.empty())  qSort(_tireMountList.begin(),_tireMountList.end(),sortTireMountByDistance);
    _dbLoading=false;
    numTanksChanged(_tankList.count());
    emit consumptionChanged(this->consumption());
    emit consumptionMaxChanged(this->consumptionMax());
    emit consumptionLastChanged(this->consumptionLast());
    emit consumptionMinChanged(this->consumptionMin());
    emit fuelTotalChanged(this->fuelTotal());
    emit maxDistanceChanged(this->maxDistance());
    emit minDistanceChanged(this->minDistance());
    qDebug() << "Loaded" << _tankList.count() << "fuel refills";
    qDebug() << "Loaded" << _stationList.count() << "gas stations";
    qDebug() << "Loaded" << _tireList.count() << "sets of tires";
    qDebug() << "Loaded" << _costList.count() << "bills";
    qDebug() << "Loaded" << _fuelTypeList.count() << "fuel types";
    qDebug() << "Loaded" << _costTypeList.count() << "cost types";
    qDebug() << "Loaded" << _tireMountList.count() << "tire mounts";
}

int Car::_dbGetVersion()
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

int Car::_dbUpgrade() {
    QSqlQuery query(db);
    QString sqlBump = QString("UPDATE CarBudget SET value='%1' WHERE id='version';");
    int currVersion = this->_dbGetVersion();
    int nextVersion = 0;
    bool success = true;
    int numErrors = 0;

    QHash<int, QStringList> sqlUpdates = QHash<int, QStringList>();

    // Create the SQL query container
    // Update to version 1 not necessary ;)

    for(int i = 2; i <= DB_VERSION; i++) {
        sqlUpdates[i] = QStringList();
    }

    // Insert the SQL queries, at correct index, in execution order
    // Note: Do NOT include version bump here, the while loop does it!
    // Version 1 -> 2
    sqlUpdates[2].append(QString("ALTER TABLE TankList ADD COLUMN note TEXT;"));
    // Version 2 -> 3
    sqlUpdates[3].append(QString("ALTER TABLE TankList ADD COLUMN fueltype INTEGER;"));
    sqlUpdates[3].append(QString("CREATE TABLE FueltypeList (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);"));
    // Version 3 -> 4
    sqlUpdates[4].append(QString("ALTER TABLE CostList ADD COLUMN costtype INTEGER;"));
    sqlUpdates[4].append(QString("CREATE TABLE CosttypeList (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);"));
    // Version 4 -> 5
    sqlUpdates[5].append(QString("INSERT INTO CarBudget (id, value) VALUES ('make',''),('model',''),('year',''),('licensePlate','');"));
    // Version 5 -> 6
    sqlUpdates[6].append(QString("INSERT INTO CarBudget (id, value) VALUES ('defaultFuelType','');"));

    while(currVersion < DB_VERSION)
    {
        nextVersion = currVersion + 1;
        qDebug() << "Updating database to version" << nextVersion;
        db.transaction();
        foreach (const QString &sql, sqlUpdates[nextVersion]) {
            success = query.exec(sql);
            if(!success){
                numErrors++;
                qDebug() << "Error running command:" << query.lastQuery();
                qDebug() << "Error was:" << query.lastError();
            }
        }
        if(numErrors == 0) {
            query.exec(sqlBump.arg(nextVersion));
            db.commit();
            qDebug() << "Update to version" << nextVersion << "succesful.";
            currVersion = nextVersion;
        }
        else{
            db.rollback();
            qDebug() << "Update to version" << nextVersion << "failed with" << numErrors << "errors.";
            break;
        }
    }
    return numErrors;
}

Car::Car(CarManager *parent) : QObject(parent), _manager(parent), _chartType(chartTypeConsumptionOf100)
{

}

Car::Car(QString name, CarManager *parent) : QObject(parent), _manager(parent), _name(name), _numTires(0),_buyingPrice(0),_sellingPrice(0),_lifetime(0), _chartType(chartTypeConsumptionOf100)
{
    this->_dbInit();
    _dbLoading=false;
    if(this->_dbGetVersion() < 1)
    {
        qDebug() << "Database is uninitialised or corrupted. Creating database...";
        this->_manager->createTables(this->db);
    }

    if(this->_dbGetVersion() < DB_VERSION)
        this->_dbUpgrade();

    qDebug() << "Database version" << this->_dbGetVersion();

    this->_dbLoad();

    this->_stationList.append(new Station);
    qSort(_stationList.begin(), _stationList.end(), sortStationByQuantity);
    this->_fuelTypeList.append(new FuelType);
    qSort(_fuelTypeList.begin(), _fuelTypeList.end(), sortFuelTypeById);
    this->_costTypeList.append(new CostType);
    qSort(_costTypeList.begin(), _costTypeList.end(), sortCostTypeById);
    make();
    model();
    year();
    licensePlate();
    numTires();
    buyingPrice();
    sellingPrice();
    lifetime();

    _beginChartIndex = numTanks() - 2; // -2 is the one with the first valid data
    _endChartIndex = 0;
}

unsigned int Car::numTanks() const
{
    return _tankList.count();
}

double Car::consumption() const
{
    if (_tankList.empty()) return 0.0;
    unsigned long int maxDistance = 0;
    unsigned long int minDistance = 999999999;
    double totalConsumption = 0;
    double partConsumption = 0;
    foreach(Tank *tank, _tankList)
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

double Car::consumptionMax() const
{
    double con=0;
    foreach (Tank *tank,_tankList)
    {
        if (tank->consumption()>con)
            con = tank->consumption();
    }
    return con;
}

double Car::consumptionLast() const
{
    if (_tankList.empty()) return 0.0;
    QList<Tank*>::const_iterator tank = _tankList.constBegin();
    if (*tank)  return (*tank)->consumption();
    else return 0;
}

double Car::consumptionMin() const
{
    double con=99999;
    foreach (Tank *tank,_tankList)
    {
        if ((tank->consumption()<con)&&(tank->consumption()!=0))
            con = tank->consumption();
    }
    return con;
}


double Car::fuelTotal() const
{
    double total=0;
    foreach (Tank *tank,_tankList)
        total += tank->quantity();
    return total;
}

unsigned int Car::maxDistance() const
{
    if(_costList.isEmpty() && _tankList.isEmpty())
        return 0;

    unsigned long int maxDistance = 0;

    foreach(Cost *cost, _costList)
    {
        if(cost->distance() > maxDistance)
            maxDistance = cost->distance();
    }
    foreach(Tank *tank, _tankList)
    {
        if(tank->distance() > maxDistance)
            maxDistance = tank->distance();
    }
    return maxDistance;
}

unsigned int Car::minDistance() const
{
    if(_costList.isEmpty() && _tankList.isEmpty())
        return 0;

    unsigned long int minDistance = 1000000;

    foreach(Cost *cost, _costList)
    {
        if(cost->distance() < minDistance)
            minDistance = cost->distance();
    }
    foreach(Tank *tank, _tankList)
    {
        if(tank->distance() < minDistance)
            minDistance = tank->distance();
    }
    return minDistance;
}

void Car::setChartType(enum chartTypeTankStatistics type)
{
    _chartType = type;
}

void Car::setChartTypeOilPrice()
{
    this->setChartType(chartTypeOilPrice);
    emit statisticTypeChanged();
}

void Car::setChartTypeConsumption()
{
    this->setChartType(chartTypeConsumptionOf100);
    emit statisticTypeChanged();
}

void Car::setChartTypeCosts()
{
    this->setChartType(chartTypeCostsOf100);
    emit statisticTypeChanged();
}

void Car::setChartBeginIndex(unsigned int index)
{
    setChartBorders(index, _endChartIndex);
    emit chartDataChanged();
}

void Car::setChartEndIndex(unsigned int index)
{
    setChartBorders(_beginChartIndex, index);
    emit chartDataChanged();
}

unsigned int Car::getChartBeginIndex()
{
    return _beginChartIndex;
}

unsigned int Car::getChartEndIndex()
{
    return _endChartIndex;
}

// begin is the earlier tank entry. So, the begin index must be bigger then the end index
void Car::setChartBorders(unsigned int begin, unsigned int end)
{
    // swap the indexes, if in wrong order
    if (begin < end)
    {
        unsigned int tmp = begin;
        begin = end;
        end = tmp;
    }

    // check the border
    if (begin > numTanks() - 2)
        begin = numTanks() - 2;

    _beginChartIndex = begin;
    _endChartIndex = end;
}

QJsonObject Car::getChartData()
{
    QJsonArray labelArray;
    QJsonArray dataArray;
    QJsonObject dataSet;
    QJsonArray dataSetArray;
    QJsonObject jsonO;

    for (unsigned int i = _beginChartIndex; i >= _endChartIndex ; i--)
    {
        labelArray.append(QString(""));

        if (_chartType == chartTypeConsumptionOf100)
        {
            dataArray.append(_tankList[i]->consumption());
        }
        else if (_chartType == chartTypeOilPrice)
        {
            dataArray.append(_tankList[i]->pricePerUnit());
        }
        else
        {
            dataArray.append(_tankList[i]->costsOn100());
        }

        // break for loop if end index will be reached
        if (i == 0)
            break;
    }

    dataSet.insert("fillColor", QString("rgba(151,187,205,0.5)"));
    dataSet.insert("strokeColor", QString("rgba(151,187,205,1)"));

    dataSet.insert("data", dataArray);
    dataSetArray.append(dataSet);

    jsonO.insert("labels", labelArray);
    jsonO.insert("datasets", dataSetArray);

    //    qDebug() << jsonO;

    return jsonO;
}


QQmlListProperty<Tank> Car::tanks()
{
    return QQmlListProperty<Tank>(this, _tankList);
}

QQmlListProperty<FuelType> Car::fuelTypes()
{
    return QQmlListProperty<FuelType>(this, _fuelTypeList);
}
QQmlListProperty<Station> Car::stations()
{
    return QQmlListProperty<Station>(this, _stationList);
}

QQmlListProperty<CostType> Car::costTypes()
{
    return QQmlListProperty<CostType>(this, _costTypeList);
}
QQmlListProperty<Cost> Car::costs()
{
    return QQmlListProperty<Cost>(this, _costList);
}

QQmlListProperty<Tire> Car::tires()
{
    return QQmlListProperty<Tire>(this, _tireList);
}

QQmlListProperty<TireMount> Car::tireMounts()
{
    return QQmlListProperty<TireMount>(this, _tireMountList);
}

const Tank *Car::previousTank(unsigned int distance) const
{
    const Tank *previous = NULL;
    unsigned int currentPrevDistance=0;
    foreach(Tank *tank, _tankList)
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
    _tankList.clear();
    _fuelTypeList.clear();
    _stationList.clear();
    _tireList.clear();
    _costList.clear();
    this->_dbInit();

    if(this->_dbGetVersion() < DB_VERSION)
        this->_dbUpgrade();

    this->_dbLoad();

    this->_stationList.append(new Station);
    qSort(_stationList.begin(), _stationList.end(), sortStationByQuantity);
}

unsigned long int Car::getDistance(QDate date)
{
    // returns the approx distance at a date
    // currently simply of last event
    // needs to be improved
    unsigned long int dist=0;
    foreach(Tank *tank, _tankList)
    {
        if (tank->date() < (QDateTime) date)
            break;
        dist=tank->distance();
    }
    return dist;
}

double Car::budgetFuel_byType(unsigned int id)
{
    // Returns average price of fuel type per 100km
    // Not sure if this really makes sense but it makes the statistic views consistent
    double price =0;
    double quantity = 0;
    foreach (Tank *tank, _tankList)
    {
        if (tank->fuelType()==id)
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

double Car::budgetFuelTotal_byType(unsigned int id)
{
    // Returns total price of all tankstops by Type
    double total=0;
    foreach(Tank *tank,_tankList)
    {
        if(tank->fuelType()==id)
            total +=tank->price();
    }
    return total;
}
double Car::budget_consumption_byType(unsigned int id)
{
    /* Return sum(fuel price) / odometer * 100 for fuelType */
    // We will calculate only full refills as partial refills cannat be calculated correctly
    double totalDistance=0;
    double totalQuantity=0;
    //go to last tankstop
    Tank *curTank = _tankList.first();
    const Tank *prevTank=NULL;
    while (curTank)
    {
        prevTank=previousTank(curTank->distance());
        if (!(prevTank==NULL))
        {
            //previous tank must have correct fuelType
            if (prevTank->fuelType()==id)
                if (prevTank->full())
                {
                    totalDistance += curTank->distance()-prevTank->distance();
                    totalQuantity += curTank->quantity();
                }
            //cannot set curTank to prevTank as it is const
            curTank=NULL;
            foreach(Tank *tmp,_tankList)
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
    foreach (Tank *tank,_tankList)
    {
        if ((tank->consumption()>con)&&(tank->fuelType()==type))
            con = tank->consumption();
    }
    return con;
}
double Car::budget_consumption_min_byType(unsigned int type)
{
    double con=99999;
    foreach (Tank *tank,_tankList)
    {
        if ((tank->consumption()<con)&&(tank->consumption()!=0)&& (tank->fuelType()==type))
            con = tank->consumption();
    }
    return (con==99999) ? 0 : con;
}
double Car::budgetCostTotal_byType(unsigned int id)
{
    double total=0;
    foreach(Cost *cost,_costList)
    {
        if(cost->costType()==id)
            total +=cost->cost();
    }
    return total;
}
double Car::budgetCost_byType(unsigned int id)
{
    /* Return sum(cost) / odometer * 100 */
    double totalPrice = 0;

    foreach(Cost *cost, _costList)
    {
        if (cost->costType()==id)
            totalPrice += cost->cost();
    }
    if (maxDistance()==minDistance()) return 0;
    return totalPrice / ((maxDistance() - minDistance())/ 100.0);
}
double Car::budgetFuelTotal()
{
    double total = 0;
    foreach (Tank *tank, _tankList)
    {
        total += tank->price();
    }
    return total;
}
double Car::budgetFuel()
{
    /* Return sum(fuel price) / odometer * 100 */
    unsigned long int maxDistance = 0;
    unsigned long int minDistance = 999999999;
    double totalPrice = 0;

    foreach(Tank *tank, _tankList)
    {
        if(tank->distance() > maxDistance)
            maxDistance = tank->distance();
        if(tank->distance() < minDistance)
            minDistance = tank->distance();
        totalPrice += tank->price();
    }
    foreach(Tank *tank, _tankList)
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
double Car::budgetCostTotal()
{
    // returns total costs for all bills
    double total=0;
    foreach(Cost *cost,_costList)
    {
        total += cost->cost();
    }
    return total;
}
double Car::budgetCost()
{
    //returns costs for bills per 100KM
    if (maxDistance() == minDistance()) return 0;
    return budgetCostTotal() / ((maxDistance() - minDistance())/ 100.0);
}
double Car::budgetInvestTotal()
{
    //returns buying costs
    return (_buyingPrice - _sellingPrice) * _amortisation();
}
double Car::budgetInvest()
{
    //returns bying costs per 100 KM
    if (maxDistance() == minDistance()) return 0.0;
    double valuecosts;
    valuecosts = (_buyingPrice - _sellingPrice) * _amortisation();
    return valuecosts / ((maxDistance() - minDistance())/ 100.0);
}
double Car::budgetTire()
{
    //returns tire costs per 100km
    if (maxDistance() == minDistance()) return 0;
    return budgetTireTotal() / ((maxDistance() - minDistance())/ 100.0);
}
double Car::budgetTireTotal()
{
    //returns total tire costs
    double total = 0;
    foreach (Tire *tire, _tireList)
    {
        total += tire->price();
    }
    return total;
}

double Car::budgetTotal()
{
    //returns all costs
    return budgetCostTotal() + budgetFuelTotal() + budgetTireTotal() + budgetInvestTotal();
}

double Car::_amortisation()
{
    if (maxDistance() == minDistance()) return 1;
    QDate today = QDate::currentDate();
    double monthsused = 1;

    if (buyingDate().toString()=="")
    {
        return 1;
    }
    while (buyingDate().addMonths(monthsused) < today)
    {
        monthsused++;
    }
    if ((monthsused < _lifetime) && (_lifetime !=0) )
    {
        return monthsused / _lifetime;
    }
    else
    {
        return 1;
    }
}

double Car::budget()
{
    // Return total costs  / odometer * 100
    return budgetCost()+budgetFuel()+budgetInvest()+budgetTire();
}

void Car::addNewTank(QDate date, unsigned int distance, double quantity, double price, bool full, unsigned int fuelType, unsigned int station, QString note)
{
    Tank *tank = new Tank(date, distance, quantity, price, full, fuelType, station, CREATE_NEW_EVENT,  note, this);
    _tankList.append(tank);
    qSort(_tankList.begin(), _tankList.end(), sortTankByDistance);
    tank->save();
    if (!_dbLoading)
    {
        emit numTanksChanged(_tankList.count());
        emit consumptionChanged(this->consumption());
        emit consumptionMaxChanged(this->consumptionMax());
        emit consumptionLastChanged(this->consumptionLast());
        emit consumptionMinChanged(this->consumptionMin());
        emit fuelTotalChanged(this->fuelTotal());
        emit maxDistanceChanged(this->maxDistance());
        emit tanksChanged();
    }
}

Tank* Car::modifyTank(Tank *tank, QDate date, unsigned int distance, double quantity, double price, bool full, unsigned int fuelType, unsigned int station, QString note)
{
    //Tank *tank = new Tank(date, distance, quantity, price, full, fuelType, station, CREATE_NEW_EVENT,  note, this);
    //_tankList.append(tank);

    tank->setDate(date);
    tank->setDistance(distance);
    tank->setQuantity(quantity);
    tank->setPrice(price);
    tank->setFull(full);
    tank->setFuelType(fuelType);
    tank->setStation(station);
    tank->setNote(note);
    tank->save();
    qSort(_tankList.begin(), _tankList.end(), sortTankByDistance);
    if (!_dbLoading)
    {
        emit numTanksChanged(_tankList.count());
        emit consumptionChanged(this->consumption());
        emit consumptionMaxChanged(this->consumptionMax());
        emit consumptionLastChanged(this->consumptionLast());
        emit consumptionMinChanged(this->consumptionMin());
        emit fuelTotalChanged(this->fuelTotal());
        emit maxDistanceChanged(this->maxDistance());
        emit tanksChanged();
    }
    return tank;
}

void Car::delTank(Tank *tank)
{
    qDebug() << "Removing tank" << tank->id();
    _tankList.removeAll(tank);
    if (!_tankList.empty()) qSort(_tankList.begin(), _tankList.end(), sortTankByDistance);
    tank->remove();
    if (!_dbLoading)
    {
        emit numTanksChanged(_tankList.count());
        emit consumptionChanged(this->consumption());
        emit consumptionMaxChanged(this->consumptionMax());
        emit consumptionLastChanged(this->consumptionLast());
        emit consumptionMinChanged(this->consumptionMin());
        emit maxDistanceChanged(this->maxDistance());
        emit tanksChanged();
    }
    tank->deleteLater();
}


void Car::addNewFuelType(QString name)
{
    // check for existing FuelType
    if (findFuelType(name))
        return;
    FuelType *fuelType = new FuelType(-1, name, this);
    _fuelTypeList.append(fuelType);
    qSort(_fuelTypeList.begin(), _fuelTypeList.end(), sortFuelTypeById);
    fuelType->save();
    emit fuelTypesChanged();
}

void Car::delFuelType(FuelType *fuelType)
{
    qDebug() << "Remove Fuel Type" << fuelType->id();
    _fuelTypeList.removeAll(fuelType);
    if (!_fuelTypeList.empty()) qSort(_fuelTypeList.begin(), _fuelTypeList.end(), sortFuelTypeById);
    QSqlQuery query(db);
    QString sql = QString("UPDATE TankList SET Fueltype = 0 WHERE fueltype=%1;").arg(fuelType->id());

    if(query.exec(sql)) {
        QString sql2 = QString("DELETE FROM FueltypeList WHERE id=%1;").arg(fuelType->id());
        qDebug() << sql2;
        if(query.exec(sql2))
        {
            qDebug() << "DELETE Fueltype in database with id" << fuelType->id();
            db.commit();
        }
        else {
            qDebug() << "Error during DELETE Fueltype in database: "<< query.lastError();
        }
    }
    else {
        qDebug() << "Error during DELETE Fueltype in database" << query.lastError();
    }
    foreach(Tank *tank, _tankList) {
        if(tank->fuelType() == fuelType->id()) {
            tank->setFuelType(0);
        }
    }
    emit fuelTypesChanged();
    fuelType->deleteLater();
}

FuelType* Car::findFuelType(QString name)
{
    foreach (FuelType *fuelType, _fuelTypeList)
    {
        if (fuelType->name()==name)
            return fuelType;
    }
    return NULL;
}

QString Car::getFuelTypeName(unsigned int id)
{
    foreach (FuelType *fuelType, _fuelTypeList)
    {
        if (fuelType->id()==id)
            return fuelType->name();
    }
    return "";
}

void Car::addNewStation(QString name)
{
    //First check for existing station
    if (findStation(name))
        return;
    Station *station = new Station(-1, name, 0, this);
    _stationList.append(station);
    qSort(_stationList.begin(), _stationList.end(), sortStationByQuantity);
    station->save();
    emit stationsChanged();
}

void Car::delStation(Station *station)
{
    qDebug() << "Remove Station" << station->id();
    _stationList.removeAll(station);
    if (!_stationList.empty()) qSort(_stationList.begin(), _stationList.end(), sortStationByQuantity);
    QSqlQuery query(db);
    QString sql = QString("UPDATE TankList SET station = 0 WHERE station=%1;").arg(station->id());

    if(query.exec(sql))
    {
        QString sql2 = QString("DELETE FROM StationList WHERE id=%1;").arg(station->id());
        qDebug() << sql2;
        if(query.exec(sql2))
        {
            qDebug() << "DELETE Station in database with id" << station->id();
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
    foreach(Tank *tank, _tankList)
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
    foreach (Station *station, _stationList)
    {
        if (station->name()==name)
            return station;
    }
    return NULL;
}

QString Car::getStationName(unsigned int id)
{
    foreach (Station *station, _stationList)
    {
        if (station->id()==id)
            return station->name();
    }
    return "";
}

void Car::addNewCostType(QString name)
{
    //First check for existing costType
    if (findCostType(name))
        return;
    CostType *costType = new CostType(-1, name, this);
    _costTypeList.append(costType);
    qSort(_costTypeList.begin(), _costTypeList.end(), sortCostTypeById);
    costType->save();
    emit costTypesChanged();
}

void Car::delCostType(CostType *costType)
{
    qDebug() << "Remove Cost Type" << costType->id();
    _costTypeList.removeAll(costType);
    if (!_costTypeList.empty()) qSort(_costTypeList.begin(), _costTypeList.end(), sortCostTypeById);
    QSqlQuery query(db);
    QString sql = QString("UPDATE CostList SET costtype = 0 WHERE costtype=%1;").arg(costType->id());

    if(query.exec(sql))
    {
        QString sql2 = QString("DELETE FROM CosttypeList WHERE id=%1;").arg(costType->id());
        qDebug() << sql2;
        if(query.exec(sql2))
        {
            qDebug() << "DELETE Costtype in database with id" << costType->id();
            db.commit();
        }
        else
        {
            qDebug() << "Error during DELETE CostType in database";
            qDebug() << query.lastError();
        }
    }
    else
    {
        qDebug() << "Error during DELETE CostType in database";
        qDebug() << query.lastError();
    }
    foreach(Cost *cost, _costList)
    {
        if(cost->costType() == costType->id())
        {
            cost->setCostType(0);
        }
    }
    emit costTypesChanged();
    costType->deleteLater();
}

CostType* Car::findCostType(QString name)
{
    foreach (CostType *costType, _costTypeList)
    {
        if (costType->name()==name)
            return costType;
    }
    return NULL;
}

QString Car::getCostTypeName(unsigned int id)
{
    foreach (CostType *costType, _costTypeList)
    {
        if (costType->id()==id)
            return costType->name();
    }
    return "";
}

void Car::addNewCost(QDate date, unsigned int distance, unsigned int costType, QString description, double price)
{
    Cost *cost = new Cost(date,distance,costType,description,price,CREATE_NEW_EVENT,this);
    _costList.append(cost);
    qSort(_costList.begin(), _costList.end(), sortCostByDate);
    cost->save();
    qDebug() << "Price for new cost:" << price;
    emit costsChanged();
}

void Car::delCost(Cost *cost)
{
    qDebug() << "Remove Cost" << cost->id();
    _costList.removeAll(cost);
    if (!_costList.empty()) qSort(_costList.begin(), _costList.end(), sortCostByDate);
    cost->remove();
    emit costsChanged();
    cost->deleteLater();
}

Tire *Car::addNewTire(QDate buyDate, QString name, QString manufacturer, QString model, double price, unsigned int quantity)
{
    Tire *tire = new Tire(buyDate,QDate(),name,manufacturer,model,price,quantity,-1,this);
    _tireList.append(tire);
    tire->save();
    emit tiresChanged();
    return tire;
}

Tire *Car::modifyTire(Tire* tire, QDate buyDate, QDate trashDate, QString name, QString manufacturer, QString model, double price, unsigned int quantity)
{
    tire->setBuyDate(QDateTime(buyDate));
    tire->setTrashDate(QDateTime(trashDate));
    tire->setName(name);
    tire->setManufacturer(manufacturer);
    tire->setModel(model);
    tire->setPrice(price);
    tire->setQuantity(quantity);
    tire->save();
    emit tiresChanged();
    return tire;
}

void Car::delTire(Tire *tire)
{
    qDebug() << "Remove Tire" << tire->id() << ":" << _tireList.removeAll(tire);
    QSqlQuery query(db);
    QString sql = QString("DELETE FROM TireList WHERE id=%1;").arg(tire->id());
    if(query.exec(sql))
    {
        qDebug() << "DELETE Tire in database with id" << tire->id();
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

QString Car::getTireName(unsigned int id)
{
    foreach (Tire *tire, _tireList)
    {
        if (tire->id()==id)
            return tire->name();
    }
    return "";
}

void Car::mountTire(QDate mountDate, unsigned int distance, Tire *tire)
{
    qDebug() << "Mount tire";
    QSqlQuery query(db);

    if(!tire->mountable())
    {
        qDebug() << "Can't mount this tire";
        return;
    }

    int id;
    QString sql = QString("INSERT INTO Event (id,date,distance) VALUES(NULL,'%1',%2)").arg(mountDate.toString("yyyy-MM-dd 00:00:00.00")).arg(distance);
    if(query.exec(sql))
    {
        id = query.lastInsertId().toInt();
        qDebug() << "Create Event(Tank) in database with id" << id;

        QString sql2 = QString("INSERT INTO TireUsage (event_mount,event_umount,tire) VALUES(%1,0,%2)").arg(id).arg(tire->id());
        if(query.exec(sql2))
        {
            id = query.lastInsertId().toInt();
            qDebug() << "Create TireUsage in database with id" << id;
            // Now add new mount to the tireMountList
            TireMount *tireMount = new TireMount(id,mountDate,distance,0,QDate(1900,1,1),0,tire->id(),this);
            _tireMountList.append(tireMount);
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

void Car::umountTire(QDate umountDate, unsigned int distance, Tire *tire, bool trashit)
{
    qDebug() << "Umount tire";
    if(!tire->mounted())
    {
        qDebug() << "Can't umount this tire";
        return;
    }

    QSqlQuery query(db);
    int id;
    QString sql = QString("INSERT INTO Event (id,date,distance) VALUES(NULL,'%1',%2)").arg(umountDate.toString("yyyy-MM-dd 00:00:00.00")).arg(distance);
    if(query.exec(sql))
    {
        id = query.lastInsertId().toInt();
        qDebug() << "Create Event(Tirmount) in database with id" << id;

        QString sql2 = QString("UPDATE TireUsage SET event_umount=%1 WHERE tire=%2 AND event_umount=0").arg(id).arg(tire->id());
        if(query.exec(sql2))
        {
            qDebug() << "Update TireUsage in database";
            //Now modify tireMountList
            foreach (TireMount *tm, _tireMountList)
            {
                if ((tm->unmountid()==0) && (tm->tire()==tire->id()))
                {
                    CarEvent *ev = new CarEvent(umountDate,distance,id,this);
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
            QString sql2 = QString("UPDATE TireList SET trashdate='%1' WHERE id=%2").arg(umountDate.toString("yyyy-MM-dd 00:00:00.00")).arg(tire->id());
            if(query.exec(sql2))
            {
                qDebug() << "Update TireList in database to trash";
                tire->setTrashDate(QDateTime(umountDate));
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

void Car::untrashTire(Tire *tire)
{
    QString sql = QString("UPDATE TireList SET trashdate='' WHERE id=%1").arg(tire->id());
    QSqlQuery query(db);
    if(query.exec(sql))
    {
        tire->setTrashDate(QDateTime());
        db.commit();
        emit tiresChanged();
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

QString Car::make()
{
    if(_make.length() < 1) {
        QSqlQuery query(this->db);

        if(query.exec("SELECT value FROM CarBudget WHERE id='make';")) {
            query.next();
            _make = query.value(0).toString();
            qDebug() << "Found car manufacturer in database:" << _make;
        }
        if(_make.length() < 1)
        {
            qDebug() << "Default car manufacturer not set in database, defaulting to empty";
            query.exec("INSERT INTO CarBudget (id, value) VALUES ('make','');");
            _make = "";
        }
    }

    return _make;
}

void Car::setMake(QString make)
{
    QSqlQuery query(this->db);

    if(query.exec("SELECT count(*) FROM CarBudget WHERE id='make';"))
    {
        query.next();
        if(query.value(0).toString().toInt() < 1)
            query.exec(QString("INSERT INTO CarBudget (id, value) VALUES ('make','%1');").arg(make));
        else
            query.exec(QString("UPDATE CarBudget SET value='%1' WHERE id='make';").arg(make));

        qDebug() << "Change car model in database:" << _make << ">>" << make;
    }
    _make = make;
    emit makeChanged();
}

QString Car::model()
{
    if(_model.length() < 1) {
        QSqlQuery query(this->db);

        if(query.exec("SELECT value FROM CarBudget WHERE id='model';")) {
            query.next();
            _model = query.value(0).toString();
            qDebug() << "Found car model in database:" << _model;
        }
        if(_model.length() < 1)
        {
            qDebug() << "Default car model not set in database, defaulting to empty";
            query.exec("INSERT INTO CarBudget (id, value) VALUES ('model','');");
            _model = "";
        }
    }

    return _model;
}

void Car::setModel(QString model)
{
    QSqlQuery query(this->db);

    if(query.exec("SELECT count(*) FROM CarBudget WHERE id='model';"))
    {
        query.next();
        if(query.value(0).toString().toInt() < 1)
            query.exec(QString("INSERT INTO CarBudget (id, value) VALUES ('model','%1');").arg(model));
        else
            query.exec(QString("UPDATE CarBudget SET value='%1' WHERE id='model';").arg(model));

        qDebug() << "Change car model in database:" << _model << ">>" << model;
    }
    _model = model;
    emit modelChanged();
}

int Car::year()
{
    if(_year < 1000 || _year > 9999) {
        QSqlQuery query(this->db);

        if(query.exec("SELECT value FROM CarBudget WHERE id='year';")) {
            query.next();
            _year = query.value(0).toInt();
            qDebug() << "Found car manufacture year in database:" << _year;
        }
        if(_year < 1000)
        {
            qDebug() << "Car manufacture year not in database, defaulting to" << QDate::currentDate().year();
            query.exec(QString("INSERT INTO CarBudget (id, value) VALUES ('year','%1');").arg(QDate::currentDate().year()));
            _year = (int) QDate::currentDate().year();
        }
    }

    return _year;
}

void Car::setYear(int year)
{
    QSqlQuery query(this->db);

    if(query.exec("SELECT count(*) FROM CarBudget WHERE id='year';"))
    {
        query.next();
        if(year < 1000) year = 1000;
        if(year > 9999) year = 9999;
        if(query.value(0).toString().toInt() < 1)
            query.exec(QString("INSERT INTO CarBudget (id, value) VALUES ('year',%1);").arg(year));
        else
            query.exec(QString("UPDATE CarBudget SET value=%1 WHERE id='year';").arg(year));

        qDebug() << "Change car manufacture year in database:" << _year << ">>" << year;
    }
    _year = year;
    emit yearChanged();
}

QString Car::licensePlate()
{
    if(_licensePlate.length() < 1) {
        QSqlQuery query(this->db);

        if(query.exec("SELECT value FROM CarBudget WHERE id='licensePlate';")) {
            query.next();
            _licensePlate = query.value(0).toString();
            qDebug() << "Found car license plate in database:" << _licensePlate;
        }
        if(_licensePlate.length() < 1)
        {
            _licensePlate = "";
        }
    }
    return _licensePlate;
}

void Car::setLicensePlate(QString licensePlate)
{
    QSqlQuery query(this->db);

    if(query.exec("SELECT count(*) FROM CarBudget WHERE id='licensePlate';"))
    {
        query.next();
        if(query.value(0).toString().toInt() < 1)
            query.exec(QString("INSERT INTO CarBudget (id, value) VALUES ('licensePlate','%1');").arg(licensePlate));
        else
            query.exec(QString("UPDATE CarBudget SET value='%1' WHERE id='licensePlate';").arg(licensePlate));

        qDebug() << "Change license plate in database:" << _licensePlate << ">>" << licensePlate;
    }
    _licensePlate = licensePlate;
    emit licensePlateChanged();
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
            qDebug() << "Find currency in database:" << _currency;
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
            query.exec(QString("UPDATE CarBudget SET value='%1' WHERE id='currency';").arg(currency));

        qDebug() << "Change currency in database:" << _currency << ">>" << currency;
    }
    _currency = currency;
    emit currencyChanged();
}

QString Car::distanceUnit()
{
    if(_distanceUnit.length() < 1)
    {
        QSqlQuery query(this->db);

        if(query.exec("SELECT value FROM CarBudget WHERE id='distanceunity';"))
        {
            query.next();
            _distanceUnit = query.value(0).toString();
            qDebug() << "Find distanceUnit in database:" << _distanceUnit;
        }
        if(_distanceUnit.length() < 1)
        {
            qDebug() << "Default distanceUnit not set in database, set to km";
            _distanceUnit = "km";
            query.exec("INSERT INTO CarBudget (id, value) VALUES ('distanceunity','km');");
        }
    }

    return _distanceUnit;
}

void Car::setDistanceUnit(QString distanceUnit)
{
    QSqlQuery query(this->db);

    if(query.exec("SELECT count(*) FROM CarBudget WHERE id='distanceunity';"))
    {
        query.next();
        if(query.value(0).toString().toInt() < 1)
            query.exec(QString("INSERT INTO CarBudget (id, value) VALUES ('distanceunity','%1');").arg(distanceUnit));
        else
            query.exec(QString("UPDATE CarBudget SET value='%1' WHERE id='distanceunity';").arg(distanceUnit));

        qDebug() << "Change distanceUnit in database:" << _distanceUnit << ">>" << distanceUnit;
    }
    _distanceUnit = distanceUnit;
    emit distanceUnitChanged();
}

unsigned int Car::numTires()
{
    if(_numTires == 0)
    {
        QSqlQuery query(this->db);

        if(query.exec("SELECT value FROM CarBudget WHERE id='nbtire';"))
        {
            query.next();
            _numTires = query.value(0).toInt();
            qDebug() << "Number of tires:" << _numTires;
        }
        if(_numTires == 0)
        {
            qDebug() << "Number of tires not set, assuming 4";
            query.exec("INSERT INTO CarBudget (id, value) VALUES ('nbtire','4');");
            _numTires = 4;
        }
    }

    return _numTires;
}

void Car::setNbtire(unsigned int numTires)
{
    QSqlQuery query(this->db);

    if(query.exec("SELECT count(*) FROM CarBudget WHERE id='nbtire';"))
    {
        query.next();
        if(query.value(0).toString().toInt() < 1)
            query.exec(QString("INSERT INTO CarBudget (id, value) VALUES ('nbtire','%1');").arg(numTires));
        else
            query.exec(QString("UPDATE CarBudget SET value='%1' WHERE id='nbtire';").arg(numTires));

        qDebug() << "Change number of tires in database:" << _numTires;
    }
    _numTires = numTires;
    emit numTiresChanged();
}

double Car::buyingPrice()
{
    if(_buyingPrice == 0)
    {
        QSqlQuery query(this->db);

        if(query.exec("SELECT value FROM CarBudget WHERE id='buyingprice';"))
        {
            query.next();
            _buyingPrice = query.value(0).toDouble();
            qDebug() << "Car purchase price:" << _buyingPrice;
        }
        if(_buyingPrice == 0)
        {
            qDebug() << "Car purchase price not set, assuming 0";
            query.exec("INSERT INTO CarBudget (id, value) VALUES ('buyingprice','0');");
            _buyingPrice = 0;
        }
    }
    return _buyingPrice;
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
            query.exec(QString("UPDATE CarBudget SET value='%1' WHERE id='buyingprice';").arg(price));

        qDebug() << "Car purchase price set to" << price;
    }
    _buyingPrice = price;
    emit buyingPriceChanged();
}

double Car::sellingPrice()
{
    if(_sellingPrice == 0)
    {
        QSqlQuery query(this->db);

        if(query.exec("SELECT value FROM CarBudget WHERE id='sellingprice';"))
        {
            query.next();
            _sellingPrice = query.value(0).toDouble();
            qDebug() << "Find sellingPrice in database:" << _sellingPrice;
        }
        if(_sellingPrice == 0)
        {
            qDebug() << "Selling price not set in database, set to 0";
            query.exec("INSERT INTO CarBudget (id, value) VALUES ('sellingprice','0');");
            _sellingPrice = 0;
        }
    }
    return _sellingPrice;
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
            query.exec(QString("UPDATE CarBudget SET value='%1' WHERE id='sellingprice';").arg(price));

        qDebug() << "Change sellingPrice in database:" << price;
    }
    _sellingPrice = price;
    emit sellingPriceChanged();
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
            qDebug() << "Find lifetime in database:" << _lifetime;
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
            query.exec(QString("UPDATE CarBudget SET value='%1' WHERE id='lifetime';").arg(months));

        qDebug() << "Change lifetime in database:" << months;
    }
    _lifetime = months;
    emit lifetimeChanged();
}

QDate Car::buyingDate()
{
    if(!_buyingDate.isValid())
    {
        QSqlQuery query(this->db);

        if(query.exec("SELECT value FROM CarBudget WHERE id='buyingdate';"))
        {
            query.next();
            _buyingDate = QDate::fromString(query.value(0).toString());
            qDebug() << "Find buying date in database:" << _buyingDate;
        }
        else
        {
            qDebug() << "buying date not set in database, set to today";
            query.exec(QString("INSERT INTO CarBudget (id, value) VALUES ('buyingdate','%1');").arg(QDate::currentDate().toString()));
            _buyingDate = QDate::currentDate();
        }
    }
    return _buyingDate;
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
            query.exec(QString("UPDATE CarBudget SET value='%1' WHERE id='buyingdate';").arg(date.toString()));

        qDebug() << "Change buying date in database:" << date;
    }
    _buyingDate = date;
    emit buyingDateChanged();
}

QString Car::getStatisticType()
{
    QString statisticType = "";

    switch (_chartType) {
    case chartTypeConsumptionOf100:
        statisticType = "Consumption";
        break;
    case chartTypeCostsOf100:
        statisticType = "Costs";
        break;
    default: // oil price
        statisticType = "Oil Price";
        break;
    }

    //emit statisticTypeChanged();

    return statisticType;
}

QString Car::consumptionUnit()
{
    if (_consumptionUnit.length() < 1)
    {
        QSqlQuery query(this->db);

        if(query.exec("SELECT value FROM CarBudget WHERE id='consumptionunit';"))
        {
            query.next();
            _consumptionUnit = query.value(0).toString();
            qDebug() << "Consumption unit in database:" << _consumptionUnit;
        }
        if(_consumptionUnit.length() < 1)
        {
            qDebug() << "Consumption unit not set in database, set to l/100km";
            query.exec("INSERT INTO CarBudget (id, value) VALUES ('consumptionunit', 'l/100km');");
            _consumptionUnit = "l/100km";
        }
    }

    return _consumptionUnit;
}

void Car::setConsumptionUnit(QString consumptionUnit)
{
    QSqlQuery query(this->db);

    if(query.exec("SELECT count(*) FROM CarBudget WHERE id='consumptionunit';"))
    {
        query.next();
        if(query.value(0).toString().toInt() < 1)
            query.exec(QString("INSERT INFO CarBudget (id, value) VALUES ('consumptionunit','%1');").arg(consumptionUnit));
        else
            query.exec(QString("UPDATE CarBudget SET value='%1' WHERE id='consumptionunit';").arg(consumptionUnit));

        qDebug() << "Change consumptionUnit in database:" << _consumptionUnit << ">>" << consumptionUnit;
    }
    _consumptionUnit = consumptionUnit;
    emit consumptionUnitChanged();
}

int Car::getDefaultFuelType()
{
    if(_defaultFuelType < 0 || _defaultFuelType > 9999) {
        QSqlQuery query(this->db);

        if(query.exec("SELECT value FROM CarBudget WHERE id='defaultFuelType';")) {
            query.next();
            _defaultFuelType = query.value(0).toInt();
            qDebug() << "Found default fuel type in database:" << _defaultFuelType;
        }
        else
        {
            qDebug() << "Car default fuel type not in database, defaulting to 0";
            query.exec(QString("INSERT INTO CarBudget (id, value) VALUES ('defaultFuelType','0');"));
            _defaultFuelType = 0;
        }
    }

    return _defaultFuelType;
}

void Car::setDefaultFuelType(int defaultFuelType)
{
    QSqlQuery query(this->db);

    if(query.exec("SELECT value FROM CarBudget WHERE id='defaultFuelType';"))
    {
        if(defaultFuelType < 0 || defaultFuelType > 9999) _defaultFuelType = 0;
        if(query.next()) {
            query.exec(QString("UPDATE CarBudget SET value=%1 WHERE id='defaultFuelType';").arg(defaultFuelType));
        }
        else {
            query.exec(QString("INSERT INTO CarBudget (id, value) VALUES ('defaultFuelType',%1);").arg(defaultFuelType));
        }

        qDebug() << "Changed default fuel type in database:" << _defaultFuelType << ">>" << defaultFuelType;
    }
    _defaultFuelType = defaultFuelType;
    emit defaultFuelTypeChanged();
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

    this->addNewFuelType("Diesel");

    this->addNewTank(QDate(2011, 2,29),km += 1071,60,70,true,1, 2,"t10");
    this->addNewTank(QDate(2011, 3, 5),km += 1031,60,70,true,1, 2,"t11");
    this->addNewTank(QDate(2011, 3,19),km += 1121,60,70,true,1, 3,"");
    this->addNewTank(QDate(2011, 3,25),km += 1134,60,70,true,1, 3,"");
    this->addNewTank(QDate(2011, 3,30),km += 1021,60,70,true,1, 1,"");
    this->umountTire(QDate(2011, 4, 5), km += 100, winter1);
    summer1 = this->addNewTire(QDate(2011,4,5),"Pneu été","Michelin","EnergySaver",110,4);
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

    this->addNewFuelType("Super Diesel");

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
    winter1 = this->addNewTire(QDate(2012,10,9),"Pneu hiver AV","Michelin","Winter 2",160,2);
    winter2 = this->addNewTire(QDate(2014,10,9),"Pneu hiver AR","Michelin","Winter 2",160,2);
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

