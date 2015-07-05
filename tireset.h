#ifndef TIRESET_H
#define TIRESET_H
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

class Car;

class Tireset : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name         READ name           WRITE setName           NOTIFY nameChanged )
    Q_PROPERTY(unsigned int  tires_associated         READ tires_associated           WRITE setTires_associated       NOTIFY tires_associatedChanged )
    Q_PROPERTY(bool      mountable  READ mountable                              NOTIFY mountChanged)

    Q_PROPERTY(unsigned int id          READ id          WRITE setId            NOTIFY idChanged )


private:
    Car *_car;
    int _id;

    QString _name;
    unsigned int _tires_associated;


public:
    explicit Tireset(Car *parent = 0);
    explicit Tireset(int id, QString name, Car *parent = 0);

    QString name() const;
    void setName(QString name);

    bool mounted() const;
    bool mountable() const;

    unsigned int id() const;
    void setId(unsigned int id);
    unsigned int tires_associated() const;
    void setTires_associated(unsigned int number);
signals:
    void nameChanged();
    void idChanged();
    void mountChanged();
    void tires_associatedChanged();

public slots:
    void save();
    void updateMountState();

};


#endif // TIRESET_H
