#ifndef CAREVENT_H
#define CAREVENT_H

#include <QObject>
#include <QDate>

class Car;

class CarEvent : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QDateTime    date        READ date        WRITE setDate           NOTIFY dateChanged )
    Q_PROPERTY(unsigned int distance    READ distance    WRITE setDistance       NOTIFY distanceChanged )
    Q_PROPERTY(unsigned int id          READ id                                  NOTIFY idChanged )

protected:
    Car *_car;
    QDate _date;
    unsigned int _distance;
    unsigned int _eventid;

public:
    explicit CarEvent(Car *parent = 0);
    explicit CarEvent(QDate date, unsigned int distance, unsigned int eventid, Car *parent = 0);

    unsigned int saveevent();
    bool delevent();

    int id() const;

signals:
    void dateChanged();
    void distanceChanged();
    void idChanged();

public slots:
    QDateTime date() const;
    void setDate(QDateTime date);

    unsigned int distance() const;
    void setDistance(unsigned int distance);
};

#endif // CAREVENT_H
