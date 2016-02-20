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


#ifndef COREAPPS_H
#define COREAPPS_H

#include <QObject>
#include <QStringList>
#include <tank.h>
#include <cost.h>
#include <tire.h>
#include <tiremount.h>
#include <fueltype.h>
#include <station.h>
#include <costtype.h>
#include <QtQuick>
#include <QtSql/QtSql>
#include "charttypes.h"

class CarManager;
#define DB_VERSION 4

class Car : public QObject
{
    Q_OBJECT

    Q_PROPERTY(unsigned int nbtank READ nbtank NOTIFY nbtankChanged)
    Q_PROPERTY(double consumption READ consumption NOTIFY consumptionChanged)
    Q_PROPERTY(double consumptionmax READ consumptionmax NOTIFY consumptionmaxChanged)
    Q_PROPERTY(double consumptionmin READ consumptionmin NOTIFY consumptionminChanged)
    Q_PROPERTY(double consumptionlast READ consumptionlast NOTIFY consumptionlastChanged)
    Q_PROPERTY(double fueltotal READ fueltotal NOTIFY fueltotalChanged)
    Q_PROPERTY(unsigned int maxdistance READ maxdistance NOTIFY maxdistanceChanged)
    Q_PROPERTY(unsigned int mindistance READ mindistance NOTIFY mindistanceChanged)
    Q_PROPERTY(QQmlListProperty<Tank> tanks READ tanks NOTIFY tanksChanged())
    Q_PROPERTY(QQmlListProperty<Fueltype> fueltypes READ fueltypes NOTIFY fueltypesChanged())
    Q_PROPERTY(QQmlListProperty<Station> stations READ stations NOTIFY stationsChanged())
    Q_PROPERTY(QQmlListProperty<Costtype> costtypes READ costtypes NOTIFY costtypesChanged())
    Q_PROPERTY(QQmlListProperty<Cost> costs READ costs NOTIFY costsChanged())
    Q_PROPERTY(QQmlListProperty<Tire> tires READ tires NOTIFY tiresChanged())
    Q_PROPERTY(QQmlListProperty<Tiremount> tiremounts READ tiremounts NOTIFY tiresChanged())
    Q_PROPERTY(int tireMounted READ tireMounted NOTIFY tireMountedChanged())
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged())
    Q_PROPERTY(QString currency READ currency WRITE setCurrency NOTIFY currencyChanged())
    Q_PROPERTY(QString distanceunity READ distanceunity WRITE setDistanceunity NOTIFY distanceunityChanged())
    Q_PROPERTY(unsigned int nbtire READ nbtire WRITE setNbtire NOTIFY nbtireChanged)
    Q_PROPERTY(double buyingprice READ buyingprice WRITE setBuyingprice NOTIFY buyingpriceChanged)
    Q_PROPERTY(double sellingprice READ sellingprice WRITE setSellingprice NOTIFY sellingpriceChanged)
    Q_PROPERTY(unsigned int lifetime READ lifetime WRITE setLifetime NOTIFY lifetimeChanged)
    Q_PROPERTY(QDate buyingdate READ buyingdate WRITE setBuyingdate NOTIFY buyingdateChanged)
    Q_PROPERTY(double budget_fuel_total READ budget_fuel_total NOTIFY budgetChanged)
    Q_PROPERTY(double budget_fuel READ budget_fuel NOTIFY budgetChanged)
    Q_PROPERTY(double budget_cost_total READ budget_cost_total NOTIFY budgetChanged)
    Q_PROPERTY(double budget_cost READ budget_cost NOTIFY budgetChanged)
    Q_PROPERTY(double budget_tire_total READ budget_tire_total NOTIFY budgetChanged)
    Q_PROPERTY(double budget_tire READ budget_tire NOTIFY budgetChanged)
    Q_PROPERTY(double budget_invest_total READ budget_invest_total NOTIFY budgetChanged)
    Q_PROPERTY(double budget_invest READ budget_invest NOTIFY budgetChanged)
    Q_PROPERTY(double budget_total      READ budget_total      NOTIFY budgetChanged)
    Q_PROPERTY(double budget      READ budget      NOTIFY budgetChanged)

    Q_PROPERTY(QJsonObject chartData READ getChartData NOTIFY chartDataChanged)
    Q_PROPERTY(QString statisticType READ getStatisticType NOTIFY statisticTypeChanged)
    Q_PROPERTY(unsigned int beginIndex READ getChartBeginIndex WRITE setChartBeginIndex NOTIFY chartDataChanged)
    Q_PROPERTY(unsigned int endIndex READ getChartEndIndex WRITE setChartEndIndex NOTIFY chartDataChanged)

private:
    CarManager *_manager;
    QString _name;

    QList<Tank*>    _tanklist;
    QList<Fueltype*> _fueltypelist;
    QList<Station*> _stationlist;
    QList<Costtype*>    _costtypelist;
    QList<Cost*>    _costlist;
    QList<Tire*>    _tirelist;
    QList<Tiremount*>    _tiremountlist;

    QString _currency;
    QString _distanceunity;

    unsigned int _nbtire;
    double _buyingprice;
    double _sellingprice;
    unsigned int _lifetime;
    QDate _buyingdate;

    unsigned int beginChartIndex_;
    unsigned int endChartIndex_;

    void db_init();
    void db_load();
    int db_get_version();

    void db_upgrade_to_2();
    void db_upgrade_to_3();
    void db_upgrade_to_4();
    bool db_loading;

    enum chartTypeTankStatistics chartType_;
    void setChartType(enum chartTypeTankStatistics type);
    void setChartBorders(unsigned int begin, unsigned int end);

public:
    QSqlDatabase db;
    explicit Car(CarManager *parent = 0);
    //explicit Car(const Car &car);
    explicit Car(QString name, CarManager *parent = 0);

    unsigned int nbtank() const;
    double fueltotal() const;
    double consumption() const;
    double consumptionmax() const;
    double consumptionmin() const;
    double consumptionlast() const;
    unsigned int maxdistance() const;
    unsigned int mindistance() const;

    QQmlListProperty<Tank> tanks();
    QQmlListProperty<Fueltype> fueltypes();
    QQmlListProperty<Station> stations();
    QQmlListProperty<Costtype> costtypes();
    QQmlListProperty<Cost> costs();
    QQmlListProperty<Tire> tires();
    QQmlListProperty<Tiremount> tiremounts();

    const Tank *previousTank(unsigned int distance) const;

    void setCar(QString name);
    QString getName() const { return _name; }

    QJsonObject getChartData();

    unsigned long int getDistance(QDate Date);

    Q_INVOKABLE double budget_fuel_byType(unsigned int id);
    Q_INVOKABLE double budget_fuel_total_byType(unsigned int id);
    Q_INVOKABLE double budget_consumption_byType(unsigned int id);
    Q_INVOKABLE double budget_consumption_max_byType(unsigned int id);
    Q_INVOKABLE double budget_consumption_min_byType(unsigned int id);
    Q_INVOKABLE double budget_cost_total_byType(unsigned int id);
    Q_INVOKABLE double budget_cost_byType(unsigned int id);
    Q_INVOKABLE void setChartTypeOilPrice();
    Q_INVOKABLE void setChartTypeConsumption();
    Q_INVOKABLE void setChartTypeCosts();
    Q_INVOKABLE void setChartBeginIndex(unsigned int index);
    Q_INVOKABLE void setChartEndIndex(unsigned int index);

    double budget_fuel_total();
    double budget_fuel();
    double budget_cost_total();
    double budget_cost();
    double budget_tire_total();
    double budget_tire();
    double budget_invest_total();
    double budget_invest();
    double budget();
    double budget_total();
    unsigned int getChartBeginIndex();
    unsigned int getChartEndIndex();

signals:
    void nbtankChanged(unsigned int nbtank);
    void consumptionChanged(double consumption);
    void consumptionmaxChanged(double consumptionmax);
    void consumptionlastChanged(double consumptionlast);
    void consumptionminChanged(double consumptionmin);
    void fueltotalChanged(double fueltotal);
    void maxdistanceChanged(double consumption);
    void mindistanceChanged(double consumption);
    void tanksChanged();
    void fueltypesChanged();
    void stationsChanged();
    void nameChanged();
    void costtypesChanged();
    void costsChanged();
    void tiresChanged();
    void tireMountedChanged();
    void currencyChanged();
    void distanceunityChanged();
    void nbtireChanged();
    void sellingpriceChanged();
    void buyingpriceChanged();
    void lifetimeChanged();
    void buyingdateChanged();
    void budgetChanged();
    void statisticTypeChanged();
    void chartDataChanged();

public slots:
    void addNewTank(QDate date, unsigned int distance, double quantity, double price, bool full, unsigned int fueltype, unsigned int station, QString note);
    void delTank(Tank *tank);

    void addNewFueltype(QString fueltype);
    void delFueltype(Fueltype *fueltype);
    Fueltype* findFueltype(QString name);
    QString getFueltypeName(unsigned int id);
    void addNewStation(QString station);
    void delStation(Station *station);
    Station* findStation(QString name);
    QString getStationName(unsigned int id);
    void addNewCosttype(QString costtype);
    void delCosttype(Costtype *costtype);
    Costtype* findCosttype(QString name);
    QString getCosttypeName(unsigned int id);
    void addNewCost(QDate date, unsigned int distance, unsigned int costtype,QString description, double price);
    void delCost(Cost *cost);

    Tire* addNewTire(QDate buydate, QString name, QString manufacturer, QString model, double price, unsigned int quantity);
    void delTire(Tire *tire);
    QString getTireName(unsigned int id);

    void mountTire(QDate mountdate, unsigned int distance, Tire *tire);
    void umountTire(QDate umountdate, unsigned int distance, Tire *tire, bool trashit=false);

    int tireMounted() const;

    void simulation();

    QString currency();
    void setCurrency(QString currency);

    QString distanceunity();
    void setDistanceunity(QString distanceunity);

    unsigned int nbtire();
    void setNbtire(unsigned int nbtire);

    double buyingprice();
    void setBuyingprice(double price);

    double sellingprice();
    void setSellingprice(double price);

    unsigned int lifetime();
    void setLifetime(int months);

    QDate buyingdate();
    void setBuyingdate(QDate date);

    QString getStatisticType();

};


#endif // COREAPPS_H
