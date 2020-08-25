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

        QList<Task *> freeTasks = this->jsonStrToTaskList(getSubMessage(message, "#freeTasks"));
        emit tasksToTable(freeTasks);
    }

    if (message.indexOf("#yourcurrenttask") > -1) {

        QJsonDocument currentTaskJD = QJsonDocument::fromJson(getSubMessage(message, "#yourcurrenttask").toUtf8());
        QJsonObject currentTask = currentTaskJD.object();
        emit enableCurrentTask(currentTask);


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

QList<Task *> eq_client::jsonStrToTaskList(const QString message)
{
    QJsonDocument tasksJD = QJsonDocument::fromJson(message.toUtf8());
    QJsonArray tasks = tasksJD.array();
    QList<Task *> res;
    foreach (const QJsonValue &task, tasks) {
        QJsonObject task_obj = task.toObject();

        QDateTime tBeginDT = QDateTime::fromString(task_obj["tBegin"].toString(), "yyyy-MM-ddThh:mm:ss");
        tBeginDT.setTimeSpec(Qt::UTC);
        QDateTime tAcceptDT = QDateTime::fromString(task_obj["tAccept"].toString(), "yyyy-MM-ddThh:mm:ss");
        tAcceptDT.setTimeSpec(Qt::UTC);
        QDateTime tEndDT = QDateTime::fromString(task_obj["tEnd"].toString(), "yyyy-MM-ddThh:mm:ss");
        tEndDT.setTimeSpec(Qt::UTC);

        Task *sameTask = new Task(task_obj["ID"].toInt(),
                tBeginDT,
                tAcceptDT,
                tEndDT,
                task_obj["ticket"].toString(),
                task_obj["operatorID"].toInt(),
                task_obj["serviceName"].toString());
        res.append(sameTask);
    }
    return res;
}
