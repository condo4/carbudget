#ifndef TIREMOUNT_H
#define TIREMOUNT_H

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
 */

#include <QObject>
#include <QDate>
#include "carevent.h"

class TireMount : public QObject
{
    // not derived from CarEvent as we need to events per Mount (Mount / Umnount)
    Q_OBJECT
    Q_PROPERTY(unsigned int tire      READ tire    WRITE setTire   NOTIFY tireMountChanged)
    Q_PROPERTY(QString tireName      READ tireName  NOTIFY tireMountChanged)
    Q_PROPERTY(unsigned int mountid      READ mountid  NOTIFY tireMountChanged)
    Q_PROPERTY(QDateTime mountDate      READ mountDate    WRITE setMountdate  NOTIFY tireMountChanged)
    Q_PROPERTY(unsigned int mountDistance      READ mountDistance    WRITE setMountDistance  NOTIFY tireMountChanged)
    Q_PROPERTY(unsigned int unmountid      READ unmountid NOTIFY tireMountChanged)
    Q_PROPERTY(QDateTime unmountDate      READ unmountDate    WRITE setUnmountDate  NOTIFY tireMountChanged)
    Q_PROPERTY(unsigned int unmountDistance      READ unmountDistance    WRITE setUnmountDistance NOTIFY tireMountChanged)


private:
    Car *_car;
    int _tire;
    CarEvent *_mountEvent;
    CarEvent *_unmountEvent;
    const QDate _notUnmounted;

public:
    explicit TireMount(Car *parent = nullptr);
    explicit TireMount(unsigned int mountid, QDate mountDate, unsigned int mountDistance,unsigned int unmountid, QDate unmountDate, unsigned int unmountDistance, int tire, Car* parent);
    int tire() const;
    unsigned int mountDistance() const;
    QDateTime mountDate() const;
    int mountid() const;
    unsigned int unmountDistance() const;
    QDateTime unmountDate() const;
    int unmountid() const;
    void setMountDistance(unsigned int distance);
    void setMountdate(QDateTime date);
    void setUnmountDistance(unsigned int distance);
    void setUnmountDate(QDateTime date);
    void setUnmountEvent(CarEvent *ev);

    QString tireName() const;
    void setTire(int tire);


signals:
    void tireMountChanged();

public slots:
    void save();
    void remove();
};


#endif // TIREMOUNT_H
