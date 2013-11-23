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

    Q_PROPERTY(unsigned int id    READ id       WRITE setId   NOTIFY idChanged )
    Q_PROPERTY(QString      name  READ name     WRITE setName NOTIFY nameChanged )

private:
    Car *_car;
    int _id;
    QString _name;

public:
    explicit Station(QObject *parent = 0);
    Station(unsigned int id, QString name, Car *parent = 0);

signals:
    void idChanged();
    void nameChanged();

public slots:
    void save();

    unsigned int id() const;
    void setId(unsigned int id);

    QString name() const;
    void setName(QString name);

};

#endif // STATION_H
