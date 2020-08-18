#ifndef EQ_CLIENT_H
#define EQ_CLIENT_H

#include <QtNetwork>
#include <QTcpSocket>
#include <QObject>


class eq_client: public QObject
{
    Q_OBJECT

public:
    eq_client();
    ~eq_client();
    void send(QString message);
    bool start(QString host, int port);
    void stop();
    void getTasks();


private slots:
    void onReadyRead();
    void onSocketError();
    void onDisconnect();




signals:
    void log_to_ui(QString log_str);
    void tasksToTable(const QJsonArray tasks);

private:
    QTcpSocket  *m_socket = NULL;
};

#endif // EQ_CLIENT_H
