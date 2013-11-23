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


#ifndef COST_H
#define COST_H

#include <QObject>
#include <QDate>

class Car;

class Cost : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDateTime    date        READ date        WRITE setDate           NOTIFY dateChanged )
    Q_PROPERTY(unsigned int distance    READ distance    WRITE setDistance       NOTIFY distanceChanged )
    Q_PROPERTY(QString      description READ description WRITE setDescription    NOTIFY descriptionChanged )
    Q_PROPERTY(double       cost        READ cost        WRITE setCost           NOTIFY costChanged )
    Q_PROPERTY(unsigned int id          READ id          WRITE setId             NOTIFY idChanged )



private:
    Car *_car;
    QDate _date;
    unsigned int _distance;
    QString _description;
    double _cost;
    int _id;

public:
    explicit Cost(QObject *parent = 0);
    explicit Cost(QDate date,unsigned int distance, QString desc, double cost, int id = -1, Car *parent = 0);

signals:
    void dateChanged();
    void descriptionChanged();
    void costChanged();
    void idChanged();
    void distanceChanged();

public slots:
    QDateTime date() const;
    void setDate(QDateTime date);

    QString description() const;
    void setDescription(QString desc);

    unsigned int distance() const;
    void setDistance(unsigned int distance);

    double cost() const;
    void setCost(double cost);

    unsigned int id() const;
    void setId(unsigned int id);

    void save();
};

#endif // COST_H
