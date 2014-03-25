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


#ifndef TIRE_H
#define TIRE_H

#include <QObject>
#include <QDate>

class Car;

class Tire : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name         READ name           WRITE setName           NOTIFY nameChanged )
    Q_PROPERTY(QString manufacturer READ manufacturer   WRITE setManufacturer   NOTIFY manufacturerChanged )
    Q_PROPERTY(QString modelname    READ model          WRITE setModel          NOTIFY modelChanged )

    Q_PROPERTY(QDateTime buydate    READ buydate        WRITE setBuydate        NOTIFY buydateChanged)
    Q_PROPERTY(QDateTime trashdate  READ trashdate      WRITE setTrashdate      NOTIFY trashdateChanged)

    Q_PROPERTY(double    price      READ price          WRITE setPrice          NOTIFY priceChanged )
    Q_PROPERTY(unsigned int quantity          READ quantity WRITE setQuantity   NOTIFY quantityChanged )

    Q_PROPERTY(bool      trashed    READ trashed                                NOTIFY trashedChanged)
    Q_PROPERTY(bool      mounted    READ mounted                                NOTIFY mountChanged)
    Q_PROPERTY(bool      mountable  READ mountable                              NOTIFY mountChanged)
    Q_PROPERTY(unsigned int  distance READ distance                             NOTIFY mountChanged)

    Q_PROPERTY(unsigned int id          READ id          WRITE setId            NOTIFY idChanged )
private:
    Car *_car;
    int _id;

    QString _name;
    QString _manufacturer;
    QString _model;

    QDate _buydate;
    QDate _trashdate;

    double _price;

    unsigned int _quantity;

public:
    explicit Tire(Car *parent = 0);
    explicit Tire(QDate buydate, QDate trashdate, QString name, QString manufacturer, QString model, double price, unsigned int quantity = 4, int id = -1, Car *parent = 0);

    QString name() const;
    void setName(QString name);

    QString manufacturer() const;
    void setManufacturer(QString manufacturer);

    QString model() const;
    void setModel(QString model);

    QDateTime buydate() const;
    void setBuydate(QDateTime date);

    QDateTime trashdate() const;
    void setTrashdate(QDateTime date);

    double price() const;
    void setPrice(double price);

    unsigned int quantity() const;
    void setQuantity(unsigned int quantity);

    unsigned int distance() const;


    bool trashed() const;
    bool mounted() const;
    bool mountable() const;

    unsigned int id() const;
    void setId(unsigned int id);

signals:
    void nameChanged();
    void manufacturerChanged();
    void modelChanged();
    void buydateChanged();
    void trashdateChanged();
    void priceChanged();
    void quantityChanged();
    void trashedChanged();
    void idChanged();
    void mountChanged();

public slots:
    void save();
    void updateMountState();

};

#endif // TIRE_H
