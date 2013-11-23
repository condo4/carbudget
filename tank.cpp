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


#include "tank.h"
#include <QDebug>
#include <car.h>
#include <carmanager.h>

Tank::Tank(Car *parent) :
    QObject(parent),
    _car(parent),
    _id(-1),
    _date(QDate::currentDate()),
    _distance(0),
    _quantity(0),
    _price(0),
    _full(true),
    _station(0)
{
}

Tank::Tank(QDate date, unsigned int distance, double quantity, double price, bool full, unsigned int station, Car *parent) :
    QObject(parent),
    _car(parent),
    _id(-1),
    _date(date),
    _distance(distance),
    _quantity(quantity),
    _price(price),
    _full(full),
    _station(station)
{
}

Tank::Tank(int id, QDate date, unsigned int distance, double quantity, double price, bool full, unsigned int station, Car *parent):
    QObject(parent),
    _car(parent),
    _id(id),
    _date(date),
    _distance(distance),
    _quantity(quantity),
    _price(price),
    _full(full),
    _station(station)
{
}

QDateTime Tank::date() const
{
    return QDateTime(_date);
}

void Tank::setDate(QDateTime date)
{
    _date = date.date();
    emit dateChanged(date);
}

unsigned int Tank::distance() const
{
    return _distance;
}

void Tank::setDistance(unsigned int distance)
{
    _distance = distance;
    emit distanceChanged(distance);
    emit consumptionChanged(this->consumption());
}

double Tank::quantity() const
{
    return _quantity;
}

void Tank::setQuantity(double quantity)
{
    _quantity = quantity;
    emit quantityChanged(quantity);
    emit consumptionChanged(this->consumption());
}

double Tank::price() const
{
    return this->_price;
}

double Tank::priceu() const
{
    return _price / _quantity;
}

void Tank::setPrice(double price)
{
    _price = price;
    emit priceChanged(price);
    emit priceuChanged(this->priceu());
}

bool Tank::full() const
{
    return _full;
}

void Tank::setFull(bool full)
{
    _full = full;
    emit fullChanged(full);
}

double Tank::consumption() const
{
    const Tank *previous = _car->previousTank(_distance);
    if(previous == NULL)
        return 0;
    return _quantity / ((_distance - previous->distance()) / 100.0);
}

unsigned int Tank::newDistance() const
{
    const Tank *previous = _car->previousTank(_distance);
    if(previous == NULL)
        return _distance;
    return _distance - previous->distance();
}

unsigned int Tank::station() const
{
    return _station;
}

void Tank::setStation(unsigned int station)
{
    _station = station;
    emit stationChanged();
}

int Tank::id() const
{
    return _id;
}

void Tank::save()
{
    if(_id < 0)
    {
        QSqlQuery query(_car->db);
        QString sql = QString("INSERT INTO Event (id,date,distance) VALUES(NULL,'%1',%2)").arg(_date.toString("yyyy-MM-dd 00:00:00.00")).arg(_distance);
        if(query.exec(sql))
        {
            _id = query.lastInsertId().toInt();
            qDebug() << "Create Event(Tank) in database with id " << _id;

            QString sql2 = QString("INSERT INTO TankList (event,quantity,price,full,station) VALUES(%1,%2,%3,%4,%5)").arg(_id).arg(_quantity).arg(_price).arg(_full).arg(_station);
            if(query.exec(sql2))
            {
                _id = query.lastInsertId().toInt();
                qDebug() << "Create Tank in database with id " << _id;
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
            QString sql2 = QString("UPDATE TankList SET quantity=%1, price=%2, full=%3, station=%4 WHERE event=%5;").arg(_quantity).arg(_price).arg(_full).arg(_station).arg(_id);
            if(query.exec(sql2))
            {
                qDebug() << "Update Tank in database with id " << _id;
                _car->db.commit();
            }
            else
            {
                qDebug() << "Error during Update Tank in database";
                qDebug() << query.lastError();
            }
        }
        else
        {
            qDebug() << "Error during Update Tank in database";
            qDebug() << query.lastError();
        }
    }
}

void Tank::remove()
{
    QSqlQuery query(_car->db);
    QString sql = QString("DELETE FROM TankList WHERE event=%1;").arg(_id);
    if(query.exec(sql))
    {
        QString sql2 = QString("DELETE FROM Event WHERE id=%1;").arg(_id);
        if(query.exec(sql2))
        {
            qDebug() << "DELETE Tank in database with id " << _id;
            _car->db.commit();
            return;
        }
    }
    qDebug() << "Error during DELETE Tank in database";
    qDebug() << query.lastError();
    _car->db.rollback();
}
