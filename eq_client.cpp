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

void eq_client::send(QString message)
{
    m_socket->write(message.toUtf8().trimmed());
}

QString getSubMessage(QString &message, const QString  controlStr)
{
    int controlStrIndex = message.indexOf(controlStr);
    int endOfCurrentMessage = message.indexOf("$", controlStrIndex);
    QString subMessage = message.mid(controlStrIndex, endOfCurrentMessage - controlStrIndex + 1);
    message.replace(subMessage, "");
    subMessage = subMessage.left(subMessage.lastIndexOf(QChar('$')));
    subMessage.replace(controlStr, "");
    return  subMessage;
}

void eq_client::onReadyRead()
{
    QString message = m_socket->readAll();
    qDebug() << message;
    emit log_to_ui(message);

    if ((message.indexOf("#freeTasks") > -1) &&  (message.indexOf("#yourcurrenttask") > -1)) {
        int y =0;
    }

    if (message.indexOf("#freeTasks") > -1) {        

        QJsonDocument freeTasksJD = QJsonDocument::fromJson(getSubMessage(message, "#freeTasks").toUtf8());
        QJsonArray freeTasks = freeTasksJD.array();
        emit tasksToTable(freeTasks);
    }

    if (message.indexOf("#yourcurrenttask") > -1) {

        QJsonDocument currentTaskJD = QJsonDocument::fromJson(getSubMessage(message, "#yourcurrenttask").toUtf8());
        QJsonObject currentTask = currentTaskJD.array()[0].toObject();


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
