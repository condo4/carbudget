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

class Car;

class Tank : public QObject
{
    Q_OBJECT
    Q_PROPERTY(unsigned int distance    READ distance   WRITE setDistance       NOTIFY distanceChanged )
    Q_PROPERTY(QDateTime    date        READ date       WRITE setDate           NOTIFY dateChanged )
    Q_PROPERTY(double       quantity    READ quantity   WRITE setQuantity       NOTIFY quantityChanged )
    Q_PROPERTY(double       price       READ price      WRITE setPrice          NOTIFY priceChanged )
    Q_PROPERTY(bool         full        READ full       WRITE setFull           NOTIFY fullChanged )
    Q_PROPERTY(unsigned int station     READ station    WRITE setStation        NOTIFY stationChanged )
    Q_PROPERTY(double       consumption READ consumption                        NOTIFY consumptionChanged)
    Q_PROPERTY(double       priceu      READ priceu                             NOTIFY priceuChanged )
    Q_PROPERTY(unsigned int newDistance READ newDistance                        NOTIFY distanceChanged )


private:
    Car *_car;
    int _id;
    QDate _date;
    unsigned int _distance;
    double _quantity;
    double _price;
    bool _full;
    unsigned int _station;

public:
    explicit Tank(Car *parent = 0);
    Tank(int id, QDate date,unsigned int distance,double quantity,double price, bool full, unsigned int station, Car* parent);
    Tank(QDate date, unsigned int distance, double quantity, double price, bool full, unsigned int station, Car *parent);

    QDateTime date() const;
    void setDate(QDateTime date);

    unsigned int distance() const;
    void setDistance(unsigned int distance);

    double quantity() const;
    void setQuantity(double quantity);

    double price() const;
    double priceu() const;
    void setPrice(double price);

    bool full() const;
    void setFull(bool full);

    double consumption() const;
    unsigned int newDistance() const;

    unsigned int station() const;
    void setStation(unsigned int station);

    int id() const;

signals:
    void distanceChanged(unsigned int distance);
    void dateChanged(QDateTime date);
    void quantityChanged(double quantity);
    void priceChanged(double price);
    void priceuChanged(double price);
    void previousChanged();
    void stationChanged();
    void fullChanged(bool full);
    void consumptionChanged(double consumption);

public slots:
    void save();
    void remove();
};

#endif // TANK_H
