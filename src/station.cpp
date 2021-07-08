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


#include "station.h"
#include "car.h"

Station::Station(QObject *parent) :
    QObject(parent),
    _id(0),
    _name("Not Set")
{
}

Station::Station(int id, QString name, double quantity, Car *parent):
    QObject(parent),
    _car(parent),
    _id(id),
    _name(name),
    _quantity(quantity)
{
}

int Station::id() const
{
    return _id;
}

void Station::setId(int id)
{
    _id = id;
    emit idChanged();
}

QString Station::name() const
{
    return _name;
}

void Station::setName(QString name)
{
    _name = name;
    emit nameChanged();
}

void Station::addQuantity(double newq)
{
    _quantity += newq;
}

double Station::quantity() const
{
    return _quantity;
}

void Station::save()
{
    if(_id < 0)
    {

        QSqlQuery query(_car->db);
        QString sql = QString("INSERT INTO StationList (id,name) VALUES(NULL,'%1')").arg(_name);
        if(query.exec(sql))
        {
            _id = query.lastInsertId().toInt();
            qDebug() << "Create Station in database with id " << _id;
            _car->db.commit();
        }
        else
        {
            qDebug() << "Error during Create Station in database";
            qDebug() << query.lastError();
        }
    }
    else
    {
        QSqlQuery query(_car->db);
        QString sql = QString("UPDATE StationList SET name='%1' WHERE id=%2;").arg(_name).arg(_id);
        qDebug() << sql;
        if(query.exec(sql))
        {
            qDebug() << "Update Station in database with id " << _id;
            _car->db.commit();
        }
        else
        {
            qDebug() << "Error during Update Station in database";
            qDebug() << query.lastError();
        }
    }
}
