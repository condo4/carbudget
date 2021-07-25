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


#ifndef COREAPPS_H
#define COREAPPS_H

#include <QObject>
#include <QStringList>
#include "tank.h"
#include "cost.h"
#include "tire.h"
#include "tiremount.h"
#include "fueltype.h"
#include "station.h"
#include "costtype.h"
#include <QtQuick>
#include <QtSql/QtSql>
#include "charttypes.h"

class CarManager;
#define DB_VERSION 6

class Car : public QObject
{
    Q_OBJECT

    Q_PROPERTY(unsigned int numTanks READ numTanks NOTIFY numTanksChanged)
    Q_PROPERTY(double consumption READ consumption NOTIFY consumptionChanged)
    Q_PROPERTY(double consumptionMax READ consumptionMax NOTIFY consumptionMaxChanged)
    Q_PROPERTY(double consumptionMin READ consumptionMin NOTIFY consumptionMinChanged)
    Q_PROPERTY(double consumptionLast READ consumptionLast NOTIFY consumptionLastChanged)
    Q_PROPERTY(double fuelTotal READ fuelTotal NOTIFY fuelTotalChanged)
    Q_PROPERTY(unsigned int maxDistance READ maxDistance NOTIFY maxDistanceChanged)
    Q_PROPERTY(unsigned int minDistance READ minDistance NOTIFY minDistanceChanged)
    Q_PROPERTY(QQmlListProperty<Tank> tanks READ tanks NOTIFY tanksChanged)
    Q_PROPERTY(QQmlListProperty<FuelType> fuelTypes READ fuelTypes NOTIFY fuelTypesChanged)
    Q_PROPERTY(QQmlListProperty<Station> stations READ stations NOTIFY stationsChanged)
    Q_PROPERTY(QQmlListProperty<CostType> costTypes READ costTypes NOTIFY costTypesChanged)
    Q_PROPERTY(QQmlListProperty<Cost> costs READ costs NOTIFY costsChanged)
    Q_PROPERTY(QQmlListProperty<Tire> tires READ tires NOTIFY tiresChanged)
    Q_PROPERTY(QQmlListProperty<TireMount> tireMounts READ tireMounts NOTIFY tiresChanged)
    Q_PROPERTY(int tireMounted READ tireMounted NOTIFY tireMountedChanged)
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QString make READ getMake WRITE setMake NOTIFY makeChanged)
    Q_PROPERTY(QString model READ getModel WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(int year READ getYear WRITE setYear NOTIFY yearChanged)
    Q_PROPERTY(QString licensePlate READ getLicensePlate WRITE setLicensePlate NOTIFY licensePlateChanged)
    Q_PROPERTY(QString currency READ currency WRITE setCurrency NOTIFY currencyChanged)
    Q_PROPERTY(QString distanceUnit READ distanceUnit WRITE setDistanceUnit NOTIFY distanceUnitChanged)
    Q_PROPERTY(QString consumptionUnit READ consumptionUnit WRITE setConsumptionUnit NOTIFY consumptionUnitChanged)
    Q_PROPERTY(int defaultFuelType READ getDefaultFuelType WRITE setDefaultFuelType NOTIFY defaultFuelTypeChanged)
    Q_PROPERTY(int lastFuelStation READ getLastFuelStation WRITE setLastFuelStation NOTIFY lastFuelStationChanged)
    Q_PROPERTY(unsigned int numTires READ numTires WRITE setNbtire NOTIFY numTiresChanged)
    Q_PROPERTY(double buyingPrice READ buyingPrice WRITE setBuyingprice NOTIFY buyingPriceChanged)
    Q_PROPERTY(double sellingPrice READ sellingPrice WRITE setSellingprice NOTIFY sellingPriceChanged)
    Q_PROPERTY(unsigned int lifetime READ lifetime WRITE setLifetime NOTIFY lifetimeChanged)
    Q_PROPERTY(QDate buyingDate READ buyingDate WRITE setBuyingdate NOTIFY buyingDateChanged)
    Q_PROPERTY(double budgetFuelTotal READ budgetFuelTotal NOTIFY budgetChanged)
    Q_PROPERTY(double budgetFuel READ budgetFuel NOTIFY budgetChanged)
    Q_PROPERTY(double budgetCostTotal READ budgetCostTotal NOTIFY budgetChanged)
    Q_PROPERTY(double budgetCost READ budgetCost NOTIFY budgetChanged)
    Q_PROPERTY(double budgetTireTotal READ budgetTireTotal NOTIFY budgetChanged)
    Q_PROPERTY(double budgetTire READ budgetTire NOTIFY budgetChanged)
    Q_PROPERTY(double budgetInvestTotal READ budgetInvestTotal NOTIFY budgetChanged)
    Q_PROPERTY(double budgetInvest READ budgetInvest NOTIFY budgetChanged)
    Q_PROPERTY(double budgetTotal      READ budgetTotal      NOTIFY budgetChanged)
    Q_PROPERTY(double budget      READ budget      NOTIFY budgetChanged)

    Q_PROPERTY(QJsonObject chartData READ getChartData NOTIFY chartDataChanged)
    Q_PROPERTY(QString statisticType READ getStatisticType NOTIFY statisticTypeChanged)
    Q_PROPERTY(unsigned int beginIndex READ getChartBeginIndex WRITE setChartBeginIndex NOTIFY chartDataChanged)
    Q_PROPERTY(unsigned int endIndex READ getChartEndIndex WRITE setChartEndIndex NOTIFY chartDataChanged)

private:
    CarManager *_manager;
    QString _name;
    QString _make;
    QString _model;
    int _year;
    QString _licensePlate;

    QList<Tank*>    _tankList;
    QList<FuelType*> _fuelTypeList;
    QList<Station*> _stationList;
    QList<CostType*>    _costTypeList;
    QList<Cost*>    _costList;
    QList<Tire*>    _tireList;
    QList<TireMount*>    _tireMountList;

    QString _currency;
    QString _distanceUnit;
    QString _consumptionUnit;
    int _defaultFuelType;
    int _lastFuelStation;

    unsigned int _numTires;
    double _buyingPrice;
    double _sellingPrice;
    unsigned int _lifetime;
    QDate _buyingDate;

    unsigned int _beginChartIndex;
    unsigned int _endChartIndex;

    bool _dbLoading;

    void _dbLoad();
    int _dbGetVersion();
    double _amortisation();
    int _dbUpgrade();


    enum chartTypeTankStatistics _chartType;

public:
    QSqlDatabase db;
    explicit Car(CarManager *parent = 0);
    //explicit Car(const Car &car);
    explicit Car(QString name, CarManager *parent = 0);

    unsigned int numTanks() const;
    double fuelTotal() const;
    double consumption() const;
    double consumptionMax() const;
    double consumptionMin() const;
    double consumptionLast() const;
    unsigned int maxDistance() const;
    unsigned int minDistance() const;

    QQmlListProperty<Tank> tanks();
    QQmlListProperty<FuelType> fuelTypes();
    QQmlListProperty<Station> stations();
    QQmlListProperty<CostType> costTypes();
    QQmlListProperty<Cost> costs();
    QQmlListProperty<Tire> tires();
    QQmlListProperty<TireMount> tireMounts();

    const Tank *previousTank(unsigned int distance) const;

    void setCar(QString name);
    QString getName() const { return _name; }
    QString getMake() const { return _make; }
    QString getModel() const { return _model; }
    int getYear() const { return _year; }
    int getDefaultFuelType() const { return _defaultFuelType; }
    int getLastFuelStation() const { return _lastFuelStation; }
    QString getLicensePlate() const { return _licensePlate; }

    QJsonObject getChartData();

    unsigned long int getDistance(QDate Date);

    Q_INVOKABLE double budgetFuel_byType(int id);
    Q_INVOKABLE double budgetFuelTotal_byType(int id);
    Q_INVOKABLE double budget_consumption_byType(int id);
    Q_INVOKABLE double budget_consumption_max_byType(int id);
    Q_INVOKABLE double budget_consumption_min_byType(int id);
    Q_INVOKABLE double budgetCostTotal_byType(int id);
    Q_INVOKABLE double budgetCost_byType(int id);
    Q_INVOKABLE void setChartTypeOilPrice();
    Q_INVOKABLE void setChartTypeConsumption();
    Q_INVOKABLE void setChartTypeCosts();
    Q_INVOKABLE void setChartBeginIndex(unsigned int index);
    Q_INVOKABLE void setChartEndIndex(unsigned int index);

    double budgetFuelTotal();
    double budgetFuel();
    double budgetCostTotal();
    double budgetCost();
    double budgetTireTotal();
    double budgetTire();
    double budgetInvestTotal();
    double budgetInvest();
    double budget();
    double budgetTotal();
    unsigned int getChartBeginIndex();
    unsigned int getChartEndIndex();

signals:
    void numTanksChanged(int numTanks);
    void consumptionChanged(double consumption);
    void consumptionMaxChanged(double consumptionMax);
    void consumptionLastChanged(double consumptionLast);
    void consumptionMinChanged(double consumptionMin);
    void fuelTotalChanged(double fuelTotal);
    void maxDistanceChanged(double consumption);
    void minDistanceChanged(double consumption);
    void lastFuelStationChanged(int station);
    void tanksChanged();
    void fuelTypesChanged();
    void stationsChanged();
    void nameChanged();
    void costTypesChanged();
    void costsChanged();
    void tiresChanged();
    void tireMountedChanged();
    void currencyChanged();
    void distanceUnitChanged();
    void consumptionUnitChanged();
    void numTiresChanged();
    void sellingPriceChanged();
    void buyingPriceChanged();
    void lifetimeChanged();
    void buyingDateChanged();
    void budgetChanged();
    void statisticTypeChanged();
    void chartDataChanged();
    void makeChanged();
    void modelChanged();
    void yearChanged();
    void defaultFuelTypeChanged();

    void licensePlateChanged();

public slots:
    void addNewTank(QDate date, unsigned int distance, double quantity, double price, bool full, unsigned int fuelType, unsigned int station, QString note);
    Tank* modifyTank(Tank *tank, QDate date, unsigned int distance, double quantity, double price, bool full, unsigned int fuelType, unsigned int station, QString note);
    void delTank(Tank *tank);

    void addNewFuelType(QString fuelType);
    void delFuelType(FuelType *fuelType);
    FuelType* findFuelType(QString name);
    QString getFuelTypeName(int id);
    void addNewStation(QString station);
    void delStation(Station *station);
    Station* findStation(QString name);
    QString getStationName(int id);
    void addNewCostType(QString costType);
    void delCostType(CostType *costType);
    CostType* findCostType(QString name);
    QString getCostTypeName(int id);
    void addNewCost(QDate date, unsigned int distance, int costType, QString description, double price);
    void delCost(Cost *cost);

    Tire* addNewTire(QDate buyDate, QString name, QString manufacturer, QString model, double price, unsigned int quantity);
    Tire* modifyTire(Tire* tire, QDate buyDate, QDate trashDate, QString name, QString manufacturer, QString model, double price, unsigned int quantity);
    void delTire(Tire *tire);
    QString getTireName(int id);

    void mountTire(QDate mountDate, unsigned int distance, Tire *tire);
    void umountTire(QDate umountDate, unsigned int distance, Tire *tire, bool trashit=false);
    void untrashTire(Tire *tire);

    int tireMounted() const;

#ifndef QT_NO_DEBUG
    void simulation();
#endif

    QString make();
    void setMake(QString make);

    QString model();
    void setModel(QString model);

    int year();
    void setYear(int year);

    int defaultFuelType();
    void setDefaultFuelType(int fuelType);

    void setLastFuelStation(int station);

    QString licensePlate();
    void setLicensePlate(QString licensePlate);

    QString currency();
    void setCurrency(QString currency);

    QString distanceUnit();
    void setDistanceUnit(QString distanceUnit);

    QString consumptionUnit();
    void setConsumptionUnit(QString consumptionUnit);

    unsigned int numTires();
    void setNbtire(unsigned int numTires);

    double buyingPrice();
    void setBuyingprice(double price);

    double sellingPrice();
    void setSellingprice(double price);

    unsigned int lifetime();
    void setLifetime(int months);

    QDate buyingDate();
    void setBuyingdate(QDate date);

    void setChartType(enum chartTypeTankStatistics type);
    void setChartBorders(unsigned int begin, unsigned int end);

    QString getStatisticType();

};


#endif // COREAPPS_H
