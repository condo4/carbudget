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


#include "tireset.h"
#include <QDebug>
#include <car.h>

Tireset::Tireset(Car *parent) :
    QObject(parent),
    _car(parent)
{
    _tires_associated=0;
}

Tireset::Tireset(int id, QString name, Car *parent):
    QObject(parent),
    _car(parent),
    _id(id),
    _name(name)
{
    _tires_associated=0;
    connect(_car, SIGNAL(TiresetMountedChanged()), this, SLOT(updateMountState()));
    connect(_car, SIGNAL(tiresChanged()), this, SLOT(updateTires_associated()));
}

QString Tireset::name() const
{
    return _name;
}

void Tireset::setName(QString name)
{
    _name = name;
    emit nameChanged();
}



bool Tireset::mounted() const
{
    QSqlQuery query(_car->db);
    QString sql = QString("SELECT count(*) FROM TireUsage WHERE Tireset=%1 AND event_umount == 0").arg(_id);
    if(query.exec(sql))
    {
        query.next();
        if(query.value(0).toInt() != 0)
        {
            return true;
        }
    }
    return false;
}

bool Tireset::mountable() const
{
    if(mounted())
        return false;
    if (_tires_associated == _car->nbtire())
        return true;
    return false;
}

unsigned int Tireset::id() const
{
    return _id;
}

void Tireset::setId(unsigned int id)
{
    _id = id;
    emit idChanged();
}

unsigned int Tireset::tires_associated() const
{
    //returns number of associated tires
    return _tires_associated;
}

void Tireset::setTires_associated(unsigned int number)
{
    //sets number of associated tires
    _tires_associated = number;
    emit tires_associatedChanged();
}

void Tireset::save()
{
    if(_id < 0)
    {
        QSqlQuery query(_car->db);
        QString sql = QString("INSERT INTO TiresetList (id,name) VALUES(NULL,'%1')").arg(_name);
        if(query.exec(sql))
        {
            _id = query.lastInsertId().toInt();
            qDebug() << "Create Tireset in database with id " << _id;
            _car->db.commit();
        }
        else
        {
            qDebug() << "Error during Create Tireset in database";
            qDebug() << query.lastError();
        }
    }
    else
    {
        QSqlQuery query(_car->db);
        QString sql = QString("UPDATE TiresetList SET name='%1' WHERE id=%2;").arg(_name).arg(_id);
        qDebug() << sql;
        if(query.exec(sql))
        {
            qDebug() << "Update Tireset in database with id " << _id;
            _car->db.commit();
        }
        else
        {
            qDebug() << "Error during Update Tireset in database";
            qDebug() << query.lastError();
        }
    }
}

void Tireset::updateTires_associated()
{
    QSqlQuery query(_car->db);
    qDebug() << "updating tires associated";
    if(query.exec("SELECT buydate,trashdate,quantity,tireset FROM TireList;"))
    {
        while(query.next())
        {
            QDate buydate = query.value(0).toDate();
            QDate trashdate = query.value(1).toDate();
            unsigned int quantity = query.value(2).toInt();
            int tireset = query.value(3).toInt();
            if (tireset == _id)
            {
                if (trashdate <= buydate)  _tires_associated+=quantity;
            }
        }
    }

}

void Tireset::updateMountState()
{
    emit mountChanged();
}

