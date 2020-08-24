#include "task.h"

Task::Task(int ID, QDateTime tBegin, QDateTime tAccept, QDateTime tEnd,
             QString ticket, int operatorID, QString serviceName)
{
    this->ID = ID;
    this->tBegin = tBegin;
    this->tAccept = tAccept;
    this->tEnd = tEnd;
    this->ticket = ticket;
    this->operatorID = operatorID;
    this->serviceName = serviceName;
}

int Task::getID() const
{
    return ID;
}

QDateTime Task::getTBegin() const
{
    return tBegin;
}

QDateTime Task::getTAccept() const
{
    return tAccept;
}

QDateTime Task::getTEnd() const
{
    return tEnd;
}

QString Task::getTicket() const
{
    return ticket;
}

int Task::getOperatorID() const
{
    return operatorID;
}

QString Task::getServiceName() const
{
    return serviceName;
}
