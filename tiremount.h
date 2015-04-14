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

class Tiremount : public QObject
{
    // not derived from CarEvent as we need to events per Mount (Mount / Umnount)
    Q_OBJECT
    Q_PROPERTY(unsigned int tire      READ tire    WRITE setTire   NOTIFY tiremountChanged)
    Q_PROPERTY(QString tirename      READ tirename  NOTIFY tiremountChanged)
    Q_PROPERTY(unsigned int mountid      READ mountid  NOTIFY tiremountChanged)
    Q_PROPERTY(QDateTime mountdate      READ mountdate    WRITE setMountdate  NOTIFY tiremountChanged)
    Q_PROPERTY(unsigned int mountdistance      READ mountdistance    WRITE setMountdistance  NOTIFY tiremountChanged)
    Q_PROPERTY(unsigned int unmountid      READ unmountid NOTIFY tiremountChanged)
    Q_PROPERTY(QDateTime unmountdate      READ unmountdate    WRITE setUnmountdate  NOTIFY tiremountChanged)
    Q_PROPERTY(unsigned int unmountdistance      READ unmountdistance    WRITE setUnmountdistance NOTIFY tiremountChanged)


private:
    Car *_car;
    unsigned int _tire;
    CarEvent *_mountEvent;
    CarEvent *_unmountEvent;
    const QDate _notUnmounted;

public:
    explicit Tiremount(Car *parent = 0);
    explicit Tiremount(unsigned int mountid, QDate mountdate, unsigned int mountdistance,unsigned int unmountid, QDate unmountdate, unsigned int unmountdistance,unsigned int tire, Car* parent);
    unsigned int tire() const;
    unsigned int mountdistance() const;
    QDateTime mountdate() const;
    unsigned int mountid() const;
    unsigned int unmountdistance() const;
    QDateTime unmountdate() const;
    unsigned int unmountid() const;
    void setMountdistance(unsigned int distance);
    void setMountdate(QDateTime date);
    void setUnmountdistance(unsigned int distance);
    void setUnmountdate(QDateTime date);
    void setUnmountEvent(CarEvent *ev);

    QString tirename() const;
    void setTire(unsigned int tire);


signals:
    void tiremountChanged();

public slots:
    void save();
    void remove();
};


#endif // TIREMOUNT_H
