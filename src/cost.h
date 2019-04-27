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


#ifndef COST_H
#define COST_H

#include <QObject>
#include <QDate>
#include "carevent.h"

class Cost : public CarEvent
{
    Q_OBJECT
    Q_PROPERTY(QString      description READ description WRITE setDescription    NOTIFY descriptionChanged )
    Q_PROPERTY(double       cost        READ cost        WRITE setCost           NOTIFY costChanged )
    Q_PROPERTY(unsigned int costType    READ costType    WRITE setCostType       NOTIFY costTypeChanged )

private:
    QString _description;
    double _cost;
    unsigned int _costType;

public:
    explicit Cost(Car *parent = 0);
    explicit Cost(QDate date,unsigned int distance,  unsigned int costType, QString desc, double cost, unsigned int id = 0, Car *parent = 0);

signals:
    void descriptionChanged();
    void costChanged();
    void costTypeChanged();

public slots:
    QString description() const;
    void setDescription(QString desc);

    double cost() const;
    void setCost(double cost);

    unsigned int costType() const;
    void setCostType(unsigned int costType);

    void save();
    void remove();
};

#endif // COST_H
