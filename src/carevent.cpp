#include "carevent.h"
#include "car.h"


CarEvent::CarEvent(Car *parent):
    QObject(parent),
    _distance(0),
    _car(parent),
    _date(QDate::currentDate()),
    _eventId(0)
{

}

CarEvent::CarEvent(QDate date, unsigned int distance, int eventId, Car *parent):
    QObject(parent),
    _distance(distance),
    _car(parent),
    _date(date),
    _eventId(eventId)
{

}

int CarEvent::saveEvent()
{
    QSqlQuery query(_car->db);

    if(_eventId == 0)
    {
        QString sql = QString("INSERT INTO Event (id,date,distance) VALUES(NULL,'%1',%2)").arg(_date.toString("yyyy-MM-dd 00:00:00.00")).arg(_distance);
        if(query.exec(sql))
        {
            _eventId = query.lastInsertId().toInt();
            qDebug() << "Create Event in database with id " << _eventId;
        }
        else _eventId = 0;
    }
    else
    {
        QString sql = QString("UPDATE Event SET date='%1', distance=%2 WHERE id=%3;").arg(_date.toString("yyyy-MM-dd 00:00:00.00")).arg(_distance).arg(_eventId);
        if(query.exec(sql))
        {
            qDebug() << "Update Event in database with id " << _eventId;
        }
        else
        {
            qDebug() << "Error during Update Event in database";
            qDebug() << query.lastError();
            return 0;
        }
    }
    return _eventId;
}

bool CarEvent::deleteEvent()
{
    QSqlQuery query(_car->db);
    QString sql = QString("DELETE FROM Event WHERE id=%1;").arg(_eventId);
    if(query.exec(sql))
    {
        qDebug() << "DELETE Event in database with id " << _eventId;
        return true;
    }
    return false;
}

int CarEvent::id() const
{
    return _eventId;
}

QDateTime CarEvent::date() const
{
    return QDateTime(_date);
}

void CarEvent::setDate(QDateTime date)
{
    _date = date.date();
    emit dateChanged();
}

unsigned int CarEvent::distance() const
{
    return _distance;
}

void CarEvent::setDistance(unsigned int distance)
{
    this->_distance = distance;
    emit distanceChanged();
}

