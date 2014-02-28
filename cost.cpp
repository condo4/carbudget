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

Cost::Cost(Car *parent) :
    CarEvent(parent),
  _description(""),
  _cost(0)
{
}

Cost::Cost(QDate date, unsigned int distance, QString desc, double cost, unsigned int id, Car *parent):
    CarEvent(date, distance, id, parent),
    _description(desc),
    _cost(cost)
{
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

double Cost::cost() const
{
    return _cost;
}

void Cost::setCost(double cost)
{
    _cost = cost;
    emit costChanged();
}

void Cost::save()
{
    if(_eventid == 0)
    {
        _eventid = saveevent();
        if(_eventid)
        {
            QSqlQuery query(_car->db);
            QString sql = QString("INSERT INTO CostList (event,cost,desc) VALUES(%1,%2,'%3')").arg(_eventid).arg(_cost).arg(_description);
            if(query.exec(sql))
            {
                qDebug() << "Create Cost in database with id " << _eventid;
                _car->db.commit();
            }
            else _eventid = 0;
        }

        if(_eventid == 0)
        {
            qDebug() << "Error during Create Cost in database";
            _car->db.rollback();
        }
    }
    else
    {
        if(saveevent())
        {
            QSqlQuery query(_car->db);
            QString sql = QString("UPDATE CostList SET cost=%1, desc='%2' WHERE event=%3;").arg(_cost).arg(_description).arg(_eventid);
            if(query.exec(sql))
            {
                qDebug() << "Update Cost in database with id " << _eventid;
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
        }
    }
}

void Cost::remove()
{
    QSqlQuery query(_car->db);
    QString sql = QString("DELETE FROM CostList WHERE event=%1;").arg(_eventid);
    if(query.exec(sql))
    {
        if(delevent())
        {
            qDebug() << "DELETE Cost in database with id " << _eventid;
            _car->db.commit();
            return;
        }
    }
    qDebug() << "Error during DELETE Cost in database";
    qDebug() << query.lastError();
    _car->db.rollback();
}
