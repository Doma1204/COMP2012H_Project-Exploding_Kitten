#include "serverworker.h"
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

ServerWorker::ServerWorker(QObject *parent):
    QObject(parent),
    serverSocket(new QTcpSocket(this))
{
    qDebug("New ServerWorker");
    connect(serverSocket, &QTcpSocket::readyRead, this, &ServerWorker::receiveJson);
    connect(serverSocket, &QTcpSocket::disconnected, this, &ServerWorker::disconnectFromClient);
}

bool ServerWorker::setSocketDescriptor(qintptr socketDescriptor) {
    return serverSocket->setSocketDescriptor(socketDescriptor);
}

QString ServerWorker::getPlayerName() const {
    return playerName;
}

void ServerWorker::setPlayerName(const QString &playerName) {
    this->playerName = playerName;
}

void ServerWorker::disconnectFromClient() {
    serverSocket->disconnectFromHost();
    emit disconnectedFromClient();
}

void ServerWorker::receiveJson() {
    qDebug("ServerWorker receive Json");
    QByteArray jsonData;
    QDataStream socketStream(serverSocket);
    socketStream.setVersion(QDataStream::Qt_5_11);
    while(true) {
        socketStream.startTransaction();
        socketStream >> jsonData;
        if (socketStream.commitTransaction()) {
            QJsonParseError parseError;
            const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
            if (parseError.error == QJsonParseError::NoError) {
                if (jsonDoc.isObject())
                    emit jsonReceived(jsonDoc.object());
                else
                    qDebug("Invalid message");
            } else {
                qDebug("Invalid message");
            }
        } else {
            break;
        }
    }
}

void ServerWorker::sendJson(const QJsonObject &json) {
    QDataStream socketStream(serverSocket);
    socketStream.setVersion(QDataStream::Qt_5_11);
    socketStream << QJsonDocument(json).toJson(QJsonDocument::Compact);
}
