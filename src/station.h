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


#ifndef STATION_H
#define STATION_H

#include <QObject>

class Car;

class Station : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int          id       READ id       WRITE setId   NOTIFY idChanged )
    Q_PROPERTY(QString      name     READ name     WRITE setName NOTIFY nameChanged )
    Q_PROPERTY(double       quantity READ quantity               NOTIFY quantityChanged )

private:
    Car *_car;
    int _id;
    QString _name;
    double _quantity;

public:
    explicit Station(QObject *parent = nullptr);
    Station(int id, QString name, double quantity, Car *parent = nullptr);

signals:
    void idChanged();
    void nameChanged();
    void quantityChanged();

public slots:
    void save();

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(QString name);

    double quantity() const;
    void addQuantity(double newq);
};

#endif // STATION_H
