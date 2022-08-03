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


#ifndef TANK_H
#define TANK_H

#include <QObject>
#include <QDate>
#include "carevent.h"
#include "charttypes.h"

struct Tour {
    double quantity;
    double price;
    unsigned long distance;
};

class Tank : public CarEvent
{
    Q_OBJECT
    Q_PROPERTY(double       quantity     READ quantity    WRITE setQuantity       NOTIFY quantityChanged )
    Q_PROPERTY(double       price        READ price       WRITE setPrice          NOTIFY priceChanged )
    Q_PROPERTY(bool         full         READ full        WRITE setFull           NOTIFY fullChanged )
    Q_PROPERTY(bool         missed       READ missed      WRITE setMissed         NOTIFY missedChanged )
    Q_PROPERTY(unsigned int station      READ station     WRITE setStation        NOTIFY stationChanged )
    Q_PROPERTY(double       consumption  READ consumption                         NOTIFY consumptionChanged )
    Q_PROPERTY(double       pricePerUnit READ pricePerUnit                        NOTIFY pricePerUnitChanged )
    Q_PROPERTY(unsigned int newDistance  READ newDistance                         NOTIFY consumptionChanged )
    Q_PROPERTY(unsigned int fuelType     READ fuelType    WRITE setFuelType       NOTIFY fuelTypeChanged )
    Q_PROPERTY(QString      fuelTypename READ fuelTypename                        NOTIFY fuelTypeChanged )
    Q_PROPERTY(QString      stationName  READ stationName                         NOTIFY stationChanged )
    Q_PROPERTY(QString      note         READ note        WRITE setNote           NOTIFY noteChanged )
    Q_PROPERTY(QDate        date         READ getDate     WRITE setDate           NOTIFY dateChanged )


private:
    double calcCostsOrConsumptionType(enum chartTypeTankStatistics type) const;

    double _quantity;
    double _price;
    bool _full;
    bool _missed;
    int _station;
    int _fuelType;
    QString _note;

public:
    explicit Tank(Car *parent = nullptr);
    explicit Tank(QDate date, unsigned int distance, double quantity, double price, bool full, bool missed, int fuelType,  int station, unsigned int id, QString note, Car* parent);

    void setDate(QDate date);
    QDate getDate() const;

    double quantity() const;
    void setQuantity(double quantity);

    double price() const;
    double pricePerUnit() const;
    void setPrice(double price);

    bool full() const;
    void setFull(bool full);

    bool missed() const;
    void setMissed(bool missed);

    Tour getTour() const;
    double consumption() const;
    double costsOn100() const;
    unsigned int newDistance() const;

    int station() const;
    QString stationName() const;
    void setStation(int station);

    int fuelType() const;
    QString fuelTypename() const;
    void setFuelType(int fuelType);

    QString note() const;
    void setNote(QString note);

signals:
    void quantityChanged(double quantity);
    void priceChanged(double price);
    void pricePerUnitChanged(double price);
    void previousChanged();
    void stationChanged();
    void fullChanged(bool full);
    void missedChanged(bool missed);
    void consumptionChanged();
    void fuelTypeChanged();
    void noteChanged();
    void dateChanged();
    void distanceChanged();

public slots:
    void save();
    void remove();
};

#endif // TANK_H
