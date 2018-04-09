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

private:
    unsigned int _distance;


protected:
    Car *_car;
    QDate _date;
    int _eventId;

public:
    explicit CarEvent(Car *parent = nullptr);
    explicit CarEvent(QDate date, unsigned int distance, int eventId, Car *parent = nullptr);

    int saveEvent();
    bool deleteEvent();

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
