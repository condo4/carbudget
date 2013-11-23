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


#include "cost.h"
#include <QDebug>
#include <car.h>

Cost::Cost(QObject *parent) :
    QObject(parent)
{
    _id = -1;
}

Cost::Cost(QDate date, unsigned int distance, QString desc, double cost, int id, Car *parent):
    QObject(parent),
    _car(parent),
    _date(date),
    _distance(distance),
    _description(desc),
    _cost(cost),
    _id(id)
{

}

QDateTime Cost::date() const
{
    return QDateTime(_date);
}

void Cost::setDate(QDateTime date)
{
    _date = date.date();
    emit dateChanged();
}

QString Cost::description() const
{
    return _description;
}

void Cost::setDescription(QString desc)
{
    _description = desc;
    emit descriptionChanged();
}

unsigned int Cost::distance() const
{
    return _distance;
}

void Cost::setDistance(unsigned int distance)
{
    this->_distance = distance;
    emit distanceChanged();
}

double Cost::cost() const
{
    return _cost;
}

void Cost::setCost(double cost)
{
    _cost = cost;
    emit costChanged();
}

unsigned int Cost::id() const
{
    return _id;
}

void Cost::setId(unsigned int id)
{
    _id = id;
    emit idChanged();
}

void Cost::save()
{
    if(_id < 0)
    {
        QSqlQuery query(_car->db);
        QString sql = QString("INSERT INTO Event (id,date,distance) VALUES(NULL,'%1',%2)").arg(_date.toString("yyyy-MM-dd 00:00:00.00")).arg(_distance);
        if(query.exec(sql))
        {
            _id = query.lastInsertId().toInt();
            qDebug() << "Create Event(Cost) in database with id " << _id;

            QString sql2 = QString("INSERT INTO CostList (event,cost,desc) VALUES(%1,%2,'%3')").arg(_id).arg(_cost).arg(_description);
            if(query.exec(sql2))
            {
                _id = query.lastInsertId().toInt();
                qDebug() << "Create Cost in database with id " << _id;
                _car->db.commit();
            }
            else _id = -1;
        }
        else _id = -1;

        if(_id == -1)
        {
            qDebug() << "Error during Create Tank in database";
            qDebug() << query.lastError();
            _car->db.rollback();
        }
    }
    else
    {
        QSqlQuery query(_car->db);
        QString sql = QString("UPDATE Event SET date='%1', distance=%2 WHERE id=%3;").arg(_date.toString("yyyy-MM-dd 00:00:00.00")).arg(_distance).arg(_id);
        if(query.exec(sql))
        {
            qDebug() << "Update Event in database with id " << _id;
            QString sql2 = QString("UPDATE CostList SET cost=%1, desc='%2'' WHERE event=%3;").arg(_cost).arg(_description).arg(_id);
            if(query.exec(sql2))
            {
                qDebug() << "Update Cost in database with id " << _id;
                _car->db.commit();
            }
            else
            {
                qDebug() << "Error during Update Cost in database";
                qDebug() << query.lastError();
            }
        }
        else
        {
            qDebug() << "Error during Update Cost in database";
            qDebug() << query.lastError();
        }
    }
}
