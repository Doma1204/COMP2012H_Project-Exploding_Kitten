#include "serverworker.h"

ServerWorker::ServerWorker(QObject *parent):
    QObject(parent),
    serverSocket(new QTcpSocket(this))
{
    qDebug("New ServerWorker");
}

bool ServerWorker::setSocketDescriptor(qintptr socketDescriptor) {
    return serverSocket->setSocketDescriptor(socketDescriptor);
}

QString ServerWorker::getUserName() const {
    return userName;
}

void ServerWorker::setUserName(const QString &userName) {
    this->userName = userName;
}
