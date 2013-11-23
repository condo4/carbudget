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


#ifndef CARMANAGER_H
#define CARMANAGER_H

#include <QObject>
#include <QStringList>
#include <QQmlListProperty>
#include <car.h>

class CarManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList cars READ cars NOTIFY carsChanged())
    Q_PROPERTY(Car *car READ car NOTIFY carChanged())

private:
    QStringList _cars;
    Car *_car;

    void refresh();

public:
    explicit CarManager(QObject *parent = 0);

    Q_INVOKABLE QStringList cars();
    Car *car();

signals:
    void carsChanged();
    void carChanged();

public slots:

    void selectCar(QString name);
    void delCar(QString name);
    void createCar(QString name);
};
#endif // CARMANAGER_H
