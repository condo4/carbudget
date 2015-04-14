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

Tiremount::Tiremount(Car *parent) :
    _notUnmounted (QDate(1900,1,1))

{
    _car = parent;
    _tire=0;
    _mountEvent = NULL;
    _unmountEvent = NULL;
}


Tiremount::Tiremount(unsigned int mountid, QDate mountdate, unsigned int mountdistance,unsigned int unmountid, QDate unmountdate, unsigned int unmountdistance,unsigned int tire, Car* parent) :
_notUnmounted (QDate(1900,1,1))
{
    _car=parent;
    _mountEvent = new CarEvent(mountdate,mountdistance,mountid,parent);
    if (unmountid!=0)
        _unmountEvent = new CarEvent(unmountdate,unmountdistance,unmountid,parent);
    else _unmountEvent = NULL;
    _tire = tire;
}




QString Tiremount::tirename() const
{
    return _car->getTireName(_tire);
}

unsigned int Tiremount::tire() const
{
    return _tire;
}

void Tiremount::setTire(unsigned int tire)
{
    _tire = tire;
    emit tiremountChanged();
}
unsigned int Tiremount::mountdistance() const
{
    if (_mountEvent)
        return _mountEvent->distance();
    else return 0;
}

void Tiremount::setMountdistance(unsigned int distance)
{
    if (_mountEvent)
    _mountEvent->setDistance(distance);
    emit tiremountChanged();
}

QDateTime Tiremount::mountdate() const
{
    if (_mountEvent)
        return _mountEvent->date();
    else return QDateTime (_notUnmounted);
}

void Tiremount::setMountdate(QDateTime date)
{
    if (_mountEvent)
    _mountEvent->setDate(date);
    emit tiremountChanged();
}
unsigned int Tiremount::mountid() const
{
    if (_mountEvent)
        return _mountEvent->id();
    else return 0;

}
unsigned int Tiremount::unmountdistance() const
{
    if (_unmountEvent)
        return _unmountEvent->distance();
    else return 0;
}

void Tiremount::setUnmountdistance(unsigned int distance)
{
    if (_unmountEvent)
    _unmountEvent->setDistance(distance);
    emit tiremountChanged();
}

QDateTime Tiremount::unmountdate() const
{
    if (_unmountEvent)
        return _unmountEvent->date();
    else return QDateTime(_notUnmounted);
}

void Tiremount::setUnmountdate(QDateTime date)
{
    if (_unmountEvent)
    _unmountEvent->setDate(date);
    emit tiremountChanged();
}
unsigned int Tiremount::unmountid() const
{
    if (_unmountEvent)
        return _unmountEvent->id();
    else return 0;

}

void Tiremount::setUnmountEvent(CarEvent *ev)
{
    _unmountEvent=ev;
}

void Tiremount::save()
{
    if (_mountEvent)
        _mountEvent->saveevent();
    if (_unmountEvent)
        _unmountEvent->saveevent();
  }

void Tiremount::remove()
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

