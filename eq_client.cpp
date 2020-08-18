#include "eq_client.h"

eq_client::eq_client()
{

}

eq_client::~eq_client()
{

}

bool eq_client::start(QString host, int port)
{
    if (!m_socket)
    {
        m_socket = new QTcpSocket();
        connect(m_socket, &QTcpSocket::readyRead, this, &eq_client::onReadyRead);
        connect(m_socket, &QAbstractSocket::disconnected,  this, &eq_client::onDisconnect);
    }
    m_socket->connectToHost(QHostAddress(host), port);
    if (!m_socket->waitForConnected(2000))
    {
        emit log_to_ui( "Error: " + m_socket->errorString());
        return false;

    } else {
        emit log_to_ui( QStringLiteral("Connecred to %1 on port %2").arg(m_socket->peerAddress().toString()).arg(m_socket->peerPort()));
        return true;
    }
}

void eq_client::stop()
{
    m_socket->disconnectFromHost();
}

void eq_client::getTasks()
{
    m_socket->write("#get_tasks");
}


void eq_client::send(QString message)
{
    m_socket->write(message.toUtf8().trimmed());
}

void eq_client::onReadyRead()
{
    QString message = m_socket->readAll();
    qDebug() << message;

    if (message.indexOf("#currentTasks") == 0) {
        message.replace("#currentTasks", "");
        QJsonDocument currentTasksJD = QJsonDocument::fromJson(message.toUtf8());
        QJsonArray currentTasks = currentTasksJD.array();
        emit tasksToTable(currentTasks);
        emit log_to_ui(message);
    }
}

void eq_client::onSocketError()
{
    QByteArray datas = m_socket->readAll();
    qDebug() << datas;
    emit log_to_ui(datas);
    m_socket->write(QByteArray("ok !\n"));
}

void eq_client::onDisconnect()
{
    emit log_to_ui("Disconnected from server");
}
