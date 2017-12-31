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
 * Authors: Fabien Proriol, Thomas Michel
 *
 */


#include "tiremount.h"
#include <QDebug>
#include <car.h>
#include <carmanager.h>

TireMount::TireMount(Car *parent) :
    _notUnmounted (QDate(1900,1,1))

{
    _car = parent;
    _tire=0;
    _mountEvent = NULL;
    _unmountEvent = NULL;
}


TireMount::TireMount(unsigned int mountid, QDate mountDate, unsigned int mountDistance,unsigned int unmountid, QDate unmountDate, unsigned int unmountDistance,unsigned int tire, Car* parent) :
_notUnmounted (QDate(1900,1,1))
{
    _car=parent;
    _mountEvent = new CarEvent(mountDate,mountDistance,mountid,parent);
    if (unmountid!=0)
        _unmountEvent = new CarEvent(unmountDate,unmountDistance,unmountid,parent);
    else _unmountEvent = NULL;
    _tire = tire;
}




QString TireMount::tireName() const
{
    return _car->getTireName(_tire);
}

unsigned int TireMount::tire() const
{
    return _tire;
}

void TireMount::setTire(unsigned int tire)
{
    _tire = tire;
    emit tireMountChanged();
}
unsigned int TireMount::mountDistance() const
{
    if (_mountEvent)
        return _mountEvent->distance();
    else return 0;
}

void TireMount::setMountDistance(unsigned int distance)
{
    if (_mountEvent)
    _mountEvent->setDistance(distance);
    emit tireMountChanged();
}

QDateTime TireMount::mountDate() const
{
    if (_mountEvent)
        return _mountEvent->date();
    else return QDateTime (_notUnmounted);
}

void TireMount::setMountdate(QDateTime date)
{
    if (_mountEvent)
    _mountEvent->setDate(date);
    emit tireMountChanged();
}
unsigned int TireMount::mountid() const
{
    if (_mountEvent)
        return _mountEvent->id();
    else return 0;

}
unsigned int TireMount::unmountDistance() const
{
    if (_unmountEvent)
        return _unmountEvent->distance();
    else return 0;
}

void TireMount::setUnmountDistance(unsigned int distance)
{
    if (_unmountEvent)
    _unmountEvent->setDistance(distance);
    emit tireMountChanged();
}

QDateTime TireMount::unmountDate() const
{
    if (_unmountEvent)
        return _unmountEvent->date();
    else return QDateTime(_notUnmounted);
}

void TireMount::setUnmountDate(QDateTime date)
{
    if (_unmountEvent)
    _unmountEvent->setDate(date);
    emit tireMountChanged();
}
unsigned int TireMount::unmountid() const
{
    if (_unmountEvent)
        return _unmountEvent->id();
    else return 0;

}

void TireMount::setUnmountEvent(CarEvent *ev)
{
    _unmountEvent=ev;
}

void TireMount::save()
{
    if (_mountEvent)
        _mountEvent->saveEvent();
    if (_unmountEvent)
        _unmountEvent->saveEvent();
  }

void TireMount::remove()
{
    if (!_mountEvent) return;
    QSqlQuery query(_car->db);
    QString sql = QString("DELETE FROM TireUsage WHERE event_mount=%1;").arg(_mountEvent->id());
    if(query.exec(sql))
    {
        qDebug() << "DELETE Tire Mount in database with id " << _mountEvent->id();
        _car->db.commit();
        return;
    }
    qDebug() << "Error during DELETE Tire Mount in database";
    qDebug() << query.lastError();
    _car->db.rollback();
}

