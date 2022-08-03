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
#include "car.h"
#include "carmanager.h"

Tank::Tank(Car *parent) :
    CarEvent(parent),
    _quantity(0),
    _price(0),
    _full(true),
    _station(0),
    _fuelType(0),
    _note("")
{
    connect(this,SIGNAL(distanceChanged()), SIGNAL(consumptionChanged()));
}

Tank::Tank(QDate date, unsigned int distance, double quantity, double price, bool full, bool missed, int fuelType, int station, unsigned int id, QString note, Car *parent):
    CarEvent(date, distance, id, parent),
    _quantity(quantity),
    _price(price),
    _full(full),
    _missed(missed),
    _station(station),
    _fuelType(fuelType),
    _note(note)
{
    connect(this,SIGNAL(distanceChanged()), SIGNAL(consumptionChanged()));
}

void Tank::setDate(QDate date)
{
    this->_date = date;
    emit dateChanged();
}

QDate Tank::getDate() const
{
    return _date;
}


double Tank::quantity() const
{
    return _quantity;
}

void Tank::setQuantity(double quantity)
{
    _quantity = quantity;
    emit quantityChanged(quantity);
    emit distanceChanged();
}

double Tank::price() const
{
    return this->_price;
}

double Tank::pricePerUnit() const
{
    if (_quantity == 0) return 0;
    return _price / _quantity;
}

void Tank::setPrice(double price)
{
    _price = price;
    emit priceChanged(price);
    emit pricePerUnitChanged(this->pricePerUnit());
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

bool Tank::missed() const
{
    return _missed;
}

void Tank::setMissed(bool missed)
{
    _missed = missed;
    emit missedChanged(missed);
}

QString Tank::stationName() const
{
    return _car->getStationName(_station);
}

double Tank::consumption() const
{
    return this->calcCostsOrConsumptionType(chartTypeConsumptionOf100);
}

double Tank::costsOn100() const
{
    return this->calcCostsOrConsumptionType(chartTypeCostsOf100);
}

double Tank::calcCostsOrConsumptionType(enum chartTypeTankStatistics type) const
{
    if (!full()) return 0.0;
    if (missed()) return 0.0;

    if (type != chartTypeConsumptionOf100 && type != chartTypeCostsOf100)
    {
        return 0.0;
    }

    const auto tour = getTour();

    if (tour.distance > 0)
    {
        if (type == chartTypeConsumptionOf100)
        {
            return tour.quantity / (tour.distance / 100.0);
        }
        else if (type == chartTypeCostsOf100)
        {
            return tour.price / (tour.distance / 100.0);
        }
    }
    return 0.0;
}

Tour Tank::getTour() const {
    Tour temporaryTour;
    Tour invalidTour{0,0,0};

    if (!full()) return invalidTour;
    if (missed()) return invalidTour;

    const Tank *previous = _car->previousTank(distance());
    temporaryTour.price = price();
    temporaryTour.quantity = quantity();

    while(previous != nullptr)
    {
        if (!(previous->missed()) && !(previous->full()))
        {
            temporaryTour.price += previous->price();
            temporaryTour.quantity += previous->quantity();
            previous = _car->previousTank(previous->distance());
        }
        else break;
    }
    if (previous == nullptr) return invalidTour;
    if (previous->missed() && !previous->full()) return invalidTour;
    if (distance() == previous->distance()) return invalidTour;
    temporaryTour.distance = distance() - previous->distance();
    return temporaryTour;
}

unsigned int Tank::newDistance() const
{
    const Tank *previous = _car->previousTank(distance());
    if(previous == nullptr)
        return 0;
    return distance() - previous->distance();
}

int Tank::station() const
{
    return _station;
}

void Tank::setStation(int station)
{
    _station = station;
    emit stationChanged();
}

QString Tank::fuelTypename() const
{
    return _car->getFuelTypeName(_fuelType);
}

int Tank::fuelType() const
{
    return _fuelType;
}

void Tank::setFuelType(int fuelType)
{
    _fuelType = fuelType;
    emit fuelTypeChanged();
}

QString Tank::note() const
{
    return _note;
}

void Tank::setNote(QString note)
{
    _note = note;
    emit noteChanged();
}

void Tank::save()
{
    if(_eventId == 0)
    {
        _eventId = saveEvent();
        if(_eventId)
        {
            QSqlQuery query(_car->db);
            QString sql = QString("INSERT INTO TankList (event,quantity,price,full,missed,station,fueltype,note) VALUES(%1,%2,%3,%4,%5,%6,%7,'%8')").arg(_eventId).arg(_quantity).arg(_price).arg(_full).arg(_missed).arg(_station).arg(_fuelType).arg(_note);
            if(query.exec(sql))
            {
                qDebug() << "Create Tank in database with id " << _eventId;
                _car->db.commit();
            }
            else _eventId = 0;
        }

        if(_eventId == 0)
        {
            qDebug() << "Error during Create Tank in database";
            _car->db.rollback();
        }
    }
    else
    {
        if(saveEvent())
        {
            QSqlQuery query(_car->db);
            QString sql = QString("UPDATE TankList SET quantity=%1, price=%2, full=%3, missed=%4, station=%5, fueltype=%6, note='%7' WHERE event=%8;").arg(_quantity).arg(_price).arg(_full).arg(_missed).arg(_station).arg(_fuelType).arg(_note).arg(_eventId);
            if(query.exec(sql))
            {
                qDebug() << "Update Tank in database with id " << _eventId;
                if(_car->db.commit()) {
                    _car->tanksChanged();
                }
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
        }
    }
    return;
}

void Tank::remove()
{
    QSqlQuery query(_car->db);
    QString sql = QString("DELETE FROM TankList WHERE event=%1;").arg(_eventId);
    if(query.exec(sql))
    {
        if(deleteEvent())
        {
            qDebug() << "DELETE Tank in database with id " << _eventId;
            if(_car->db.commit()) {
                _car->tanksChanged();
            }

        }
    }
    else {
        qDebug() << "Error during DELETE Tank in database";
        qDebug() << query.lastError();
        _car->db.rollback();
    }
    return;
}
