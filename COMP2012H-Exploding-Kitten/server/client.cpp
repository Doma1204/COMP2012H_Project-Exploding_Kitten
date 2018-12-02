#include "client.h"
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDataStream>

Client::Client(QObject *parent):
    QObject(parent),
    clientSocket(new QTcpSocket(this))
{
    connect(clientSocket, &QTcpSocket::connected, this, &Client::connected);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(clientSocket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    // Forward the error signal, QOverload is necessary as error() is overloaded, see the Qt docs
//    connect(clientSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Client::error);
}

void Client::connectToServer(const QHostAddress &address, quint16 port) {
    clientSocket->connectToHost(address, port);
}

void Client::disconnectFromHost() {
    qDebug("client Disconnect");
    clientSocket->disconnectFromHost();
}

void Client::sendJson(const QJsonObject &json) {
    QDataStream clientStream(clientSocket);
    clientStream.setVersion(QDataStream::Qt_5_11);
    clientStream << QJsonDocument(json).toJson(QJsonDocument::Compact);
}

void Client::jsonReceived(const QJsonObject &json) {
    emit receiveJson(json);
}

void Client::onReadyRead() {
    qDebug("Client ready read");
    QByteArray jsonData;
    QDataStream socketStream(clientSocket);
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
