#include "client.h"
#include <QTcpSocket>

Client::Client(QObject *parent):
    QObject(parent),
    clientSocket(new QTcpSocket(this))
{
    // Forward the connected and disconnected signals
    connect(clientSocket, &QTcpSocket::connected, this, &Client::connected);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Client::disconnected);
    // connect readyRead() to the slot that will take care of reading the data in
//    connect(clientSocket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    // Forward the error signal, QOverload is necessary as error() is overloaded, see the Qt docs
//    connect(clientSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Client::error);
    // Reset the loggedIn variable when we disconnec. Since the operation is trivial we use a lambda instead of creating another slot
//    connect(clientSocket, &QTcpSocket::disconnected, this, [this]()->void{loggedIn = false;});
}

void Client::connectToServer(const QHostAddress &address, quint16 port) {
    clientSocket->connectToHost(address, port);
}

void Client::disconnectFromHost() {
    clientSocket->disconnectFromHost();
}
