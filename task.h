#ifndef TASK_H
#define TASK_H

#include <QDateTime>


class Task
{
public:
    Task(int ID, QDateTime tBegin, QDateTime tAccept, QDateTime tEnd,
         QString ticket, int operatorID, QString serviceName);

    int getID() const;
    QDateTime getTBegin() const;
    QDateTime getTAccept() const;
    QDateTime getTEnd() const;
    QString getTicket() const;
    int getOperatorID() const;
    QString getServiceName() const;

private:
    int ID;
    QDateTime tBegin;
    QDateTime tAccept;
    QDateTime tEnd;
    QString ticket;
    int operatorID;
    QString serviceName;


};

#endif // TASK_H
