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


#include "tire.h"
#include <QDebug>
#include <car.h>

Tire::Tire(Car *parent) :
    QObject(parent),
    _car(parent)
{

}

Tire::Tire(QDate buydate, QDate trashdate, QString name, QString manufacturer, QString model, double price, unsigned int quantity, int id, int tireset, Car *parent):
    QObject(parent),
    _car(parent),
    _tireset(tireset),
    _id(id),
    _name(name),
    _manufacturer(manufacturer),
    _model(model),
    _buydate(buydate),
    _trashdate(trashdate),
    _price(price),
    _quantity(quantity)
{
    connect(_car, SIGNAL(tireMountedChanged()), this, SLOT(updateMountState()));
}

QString Tire::name() const
{
    return _name;
}

void Tire::setName(QString name)
{
    _name = name;
    emit nameChanged();
}

QString Tire::manufacturer() const
{
    return _manufacturer;
}

void Tire::setManufacturer(QString manufacturer)
{
    _manufacturer = manufacturer;
    emit manufacturerChanged();
}

QString Tire::model() const
{
    return _model;
}

void Tire::setModel(QString model)
{
    _model = model;
    emit modelChanged();
}

QDateTime Tire::buydate() const
{
    return QDateTime(_buydate);
}

void Tire::setBuydate(QDateTime date)
{
    _buydate = date.date();
    emit buydateChanged();
}

QDateTime Tire::trashdate() const
{
    return QDateTime(_trashdate);
}

void Tire::setTrashdate(QDateTime date)
{
    _trashdate = date.date();
    emit trashdateChanged();
}

double Tire::price() const
{
    return _price;
}

void Tire::setPrice(double price)
{
    _price = price;
    emit priceChanged();
}

unsigned int Tire::quantity() const
{
    return _quantity;
}

void Tire::setQuantity(unsigned int quantity)
{
    _quantity = quantity;
    emit quantityChanged();
}

unsigned int Tire::distance() const
{
    unsigned int distance = 0;
    QSqlQuery query(_car->db);
    if(query.exec(QString("select sum(UEvent.distance - MEvent.distance) from TireUsage, Event as MEvent, Event as UEvent WHERE TireUsage.tire = %1 AND TireUsage.event_mount = MEvent.id AND TireUsage.event_umount = UEvent.id;").arg(_id)))
    {
        if(query.next())
        {
            distance = query.value(0).toInt();
        }
    }
    else
    {
        qDebug() << query.lastError();
    }

    if(query.exec(QString("select MEvent.distance from TireUsage, Event as MEvent WHERE TireUsage.tire = %1 AND TireUsage.event_mount = MEvent.id AND TireUsage.event_umount = 0").arg(_id)))
    {
        if(query.next())
        {
            if(_car->maxdistance() > query.value(0).toUInt())
                distance += _car->maxdistance() - query.value(0).toUInt();
        }
    }
    else
    {
        qDebug() << query.lastError();
    }

    return distance;
}

bool Tire::trashed() const
{
    return _trashdate > _buydate;
}

bool Tire::mounted() const
{
    QSqlQuery query(_car->db);
    QString sql = QString("SELECT count(*) FROM TireUsage WHERE tire=%1 AND event_umount == 0").arg(_id);
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

bool Tire::mountable() const
{
    if(_trashdate != _buydate)
        return false;

    if(_car->tireMounted() + _quantity > _car->nbtire())
        return false;

    if(mounted())
        return false;

    return true;
}

unsigned int Tire::id() const
{
    return _id;
}

void Tire::setId(unsigned int id)
{
    _id = id;
    emit idChanged();
}

unsigned int Tire::tireset() const
{
    return _tireset;
}

void Tire::setTireset (unsigned int id)
{
    _tireset = id;
}

void Tire::save()
{
    if(_id < 0)
    {
        QSqlQuery query(_car->db);
        QString sql = QString("INSERT INTO TireList (id,buydate,trashdate,name,manufacturer,model,price,quantity,tireset) VALUES(NULL,'%1','%2','%3','%4','%5',%6,%7,%8)").arg(_buydate.toString("yyyy-MM-dd 00:00:00.00")).arg(_trashdate.toString("yyyy-MM-dd 00:00:00.00")).arg(_name).arg(_manufacturer).arg(_model).arg(_price).arg(_quantity).arg(_tireset);
        if(query.exec(sql))
        {
            _id = query.lastInsertId().toInt();
            qDebug() << "Create Tire in database with id " << _id;
            _car->db.commit();
        }
        else
        {
            qDebug() << "Error during Create Tire in database";
            qDebug() << query.lastError();
        }
    }
    else
    {
        QSqlQuery query(_car->db);
        QString sql = QString("UPDATE TireList SET buydate='%1', trashdate='%2', name='%3', manufacturer='%4', model='%5', price=%6, quantity=%7, tireset=%8 WHERE id=%8;").arg(_buydate.toString("yyyy-MM-dd 00:00:00.00")).arg(_trashdate.toString("yyyy-MM-dd 00:00:00.00")).arg(_name).arg(_manufacturer).arg(_model).arg(_price).arg(_quantity).arg(_id).arg(_tireset);
        qDebug() << sql;
        if(query.exec(sql))
        {
            qDebug() << "Update Tire in database with id " << _id;
            _car->db.commit();
        }
        else
        {
            qDebug() << "Error during Update Tire in database";
            qDebug() << query.lastError();
        }
    }
}

void Tire::updateMountState()
{
    emit mountChanged();
}
