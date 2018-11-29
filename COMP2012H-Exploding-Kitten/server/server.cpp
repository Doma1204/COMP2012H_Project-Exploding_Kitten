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
QVector<ServerWorker*> Server::getClients() const {return clients;}

void Server::incomingConnection(qintptr socketDesriptor) {
    qDebug("New Player");
    ServerWorker *worker = new ServerWorker(this);
    if (!worker->setSocketDescriptor(socketDesriptor)) {
        worker->deleteLater();
        return;
    }

    if (!clients.isEmpty()) {
        qDebug("Send player list");
        QJsonObject playerNamesMsg;
        QJsonArray playerNames;
        for (ServerWorker *worker : clients)
            playerNames.append(worker->getPlayerName());
        playerNamesMsg["type"] = "playerList";
        playerNamesMsg["playerNames"] = playerNames;
        sendJson(worker, playerNamesMsg);
    }

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

void Server::startGameBroadcast() {
    if (clients.size()>4){
        QMessageBox::information(nullptr, QString("Too Many Players"), QString("There are too many players."));
        return;
    }
    qDebug("Server Start Game Broadcast");
    QJsonObject startGameMsg;
    startGameMsg["type"] = "startGame";
    broadcast(startGameMsg);
}


void Server::userDisconnected(ServerWorker *sender) {
    clients.removeAll(sender);
    const QString player = sender->getPlayerName();
    if (!player.isEmpty()) {
        QJsonObject disconnectedMessage;
        disconnectedMessage["type"] = QString("playerDisconnected");
        disconnectedMessage["playerName"] = player;
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

void Server::stopServer() {
    for (ServerWorker *worker : clients)
        worker->disconnectFromClient();
    close();
}
