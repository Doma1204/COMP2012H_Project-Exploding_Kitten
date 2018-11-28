#include "server.h"
#include "serverworker.h"

#include <QNetworkInterface>
#include <QMessageBox>
#include <QtDebug>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

Server::Server(QObject *parent):
    QTcpServer(parent)
{
    if (!listen()) {
        qDebug("Unable to start the server");
        close();
        return;
    }
    port = serverPort();

    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
            ip = ipAddressesList.at(i).toString();
            break;
        }
    }

    if (ip.isEmpty())
        ip = QHostAddress(QHostAddress::LocalHost).toString();
}

QString Server::getIP() const {return ip;}
quint16 Server::getPort() const {return port;}

void Server::incomingConnection(qintptr socketDesriptor) {
    qDebug("New Player");
    ServerWorker *worker = new ServerWorker(this);
    if (!worker->setSocketDescriptor(socketDesriptor)) {
        worker->deleteLater();
        return;
    }

//    QJsonObject requestNameMsg;
//    requestNameMsg["type"] = "requestName";
//    sendJson(worker, requestNameMsg);

//    if (!clients.isEmpty()) {
//        QJsonObject playerNamesMsg;
//        QJsonArray playerNames;
//        for (ServerWorker *worker : clients)
//            playerNames.append(worker->getUserName());
//        playerNamesMsg["type"] = "playerList";
//        playerNamesMsg["playerNames"] = playerNames;
//        sendJson(worker, playerNamesMsg);
//    }

    connect(worker, &ServerWorker::disconnectedFromClient, this, std::bind(&Server::userDisconnected, this, worker));
    connect(worker, &ServerWorker::jsonReceived, this, std::bind(&Server::jsonReceived, this, worker, std::placeholders::_1));
    clients.append(worker);
}

void Server::jsonReceived(ServerWorker *sender, const QJsonObject &json) {
    Q_ASSERT(sender);

    qDebug("Server Receive a Client Json");
    const QString type = json.value(QString("type")).toString();
    if (type == "playerName") {
        qDebug("player name");
        const QString playerName = json.value("playerName").toString();
        qDebug() << playerName;
        sender->setPlayerName(playerName);
        QJsonObject newPlayerMsg;
        newPlayerMsg["type"] = "newPlayer";
        newPlayerMsg["playerName"] = playerName;
//        sendJson(sender, newPlayerMsg);
        broadcast(newPlayerMsg);
    }

    emit receiveJson(sender, json);
}

void Server::userDisconnected(ServerWorker *sender) {
    clients.removeAll(sender);
    const QString userName = sender->getPlayerName();
    if (!userName.isEmpty()) {
        QJsonObject disconnectedMessage;
        disconnectedMessage["type"] = QString("playerDisconnected");
        disconnectedMessage["username"] = userName;
        broadcast(disconnectedMessage, nullptr);
    }
    sender->deleteLater();
}

void Server::sendJson(ServerWorker *worker, const QJsonObject &json) {
    Q_ASSERT(worker);
    worker->sendJson(json);
}

void Server::broadcast(const QJsonObject &json, ServerWorker *exclude) {
    qDebug("Broadcast Message");
    for (ServerWorker *worker : clients) {
        Q_ASSERT(worker);
        if (worker == exclude)
            continue;
        sendJson(worker, json);
    }
}
