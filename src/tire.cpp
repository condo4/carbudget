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
#include "car.h"

Tire::Tire(Car *parent) :
    QObject(parent),
    _car(parent)
{

}

Tire::Tire(QDate buyDate, QDate trashDate, QString name, QString manufacturer, QString model, double price, unsigned int quantity, int id, Car *parent):
    QObject(parent),
    _car(parent),
    _id(id),
    _name(name),
    _manufacturer(manufacturer),
    _model(model),
    _buyDate(buyDate),
    _trashDate(trashDate),
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

QDateTime Tire::buyDate() const
{
    return QDateTime(_buyDate);
}

void Tire::setBuyDate(QDateTime date)
{
    _buyDate = date.date();
    emit buyDateChanged();
}

QDateTime Tire::trashDate() const
{
    return QDateTime(_trashDate);
}

void Tire::setTrashDate(QDateTime date)
{
    if(date.date() < _buyDate)
        _trashDate = QDate();
    else
        _trashDate = date.date();
    emit trashDateChanged();
    emit trashedChanged();
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
            if(_car->maxDistance() > query.value(0).toUInt())
                distance += _car->maxDistance() - query.value(0).toUInt();
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
    if(_trashDate.isValid()
    && _trashDate <= QDate::currentDate())
    {
        //qDebug() << "Trashed.";
        return true;
    }
    else
    {
        //qDebug() << "Not trashed.";
        return false;
    }
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
            //qDebug() << "Mounted.";
            return true;
        }
    }
    //qDebug() << "Not mounted.";
    return false;
}

bool Tire::mountable() const
{
    if(_buyDate > QDate::currentDate()                 // Are the tires bought yet? :)
    || _car->tireMounted() + _quantity > _car->numTires() // Are there too many wheels to be mounted?
    || mounted()                                        // Are the tires already mounted?
    || trashed())                                       // Are the tires trashed?
    {
        //qDebug() << "Not mountable.";
        return false;
    }

    //qDebug() << "Mountable.";
    return true;
}

int Tire::id() const
{
    return _id;
}

void Tire::setId(int id)
{
    _id = id;
    emit idChanged();
}

void Tire::save()
{
    if(_id < 0)
    {
        QSqlQuery query(_car->db);
        QString sql = QString("INSERT INTO TireList (id,buydate,trashdate,name,manufacturer,model,price,quantity) VALUES(NULL,'%1','%2','%3','%4','%5',%6,%7)").arg(_buyDate.toString("yyyy-MM-dd 00:00:00.00")).arg(_trashDate.toString("yyyy-MM-dd 00:00:00.00")).arg(_name).arg(_manufacturer).arg(_model).arg(_price).arg(_quantity);
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
        QString sql = QString("UPDATE TireList SET buydate='%1', trashdate='%2', name='%3', manufacturer='%4', model='%5', price=%6, quantity=%7 WHERE id=%8;").arg(_buyDate.toString("yyyy-MM-dd 00:00:00.00")).arg(_trashDate.toString("yyyy-MM-dd 00:00:00.00")).arg(_name).arg(_manufacturer).arg(_model).arg(_price).arg(_quantity).arg(_id);
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
