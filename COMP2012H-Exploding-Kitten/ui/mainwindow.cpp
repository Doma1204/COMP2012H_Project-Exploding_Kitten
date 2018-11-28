#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "server.h"
#include "client.h"

#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QList>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    serverNameLabel(new QLabel(QString("Server Name:"), this)),
    serverPortLabel(new QLabel(QString("Server Port:"), this)),
    nameLabel(new QLabel(QString("Name:"), this)),
    server(nullptr),
    client(nullptr),
    playerName(""),
    isHost(false)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Exploding Kittens"));
    createRoomBtn = new QPushButton(QString("Create Room") ,this);
    joinRoomBtn = new QPushButton(QString("Join Room"), this);
    backBtn = new QPushButton(QString("Back"), this);
    leaveBtn = new QPushButton(QString("Leave"), this);
    startBtn = new QPushButton(QString("Start"), this);

    serverNameLineEdit = new QLineEdit(this);
    serverPortLineEdit = new QLineEdit(this);
    nameLineEdit = new QLineEdit(this);

    playerList = new QListWidget(this);

    createRoomBtn->hide();
    joinRoomBtn->hide();
    backBtn->hide();
    leaveBtn->hide();
    startBtn->hide();
    serverNameLabel->hide();
    serverPortLabel->hide();
    nameLabel->hide();
    serverNameLineEdit->hide();
    serverPortLineEdit->hide();
    nameLineEdit->hide();
    playerList->hide();

    setRequestRoomWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setRequestRoomWindow() {
    createRoomBtn->setGeometry(125, 170, 150, 30);
    joinRoomBtn->setGeometry(125, 200, 150, 30);

    //    createRoomBtn->setStyleSheet(QString("QPushButton{background:white}"));
    //    joinRoomBtn->setStyleSheet(QString("QPushButton{background:white}"));

    createRoomBtn->show();
    joinRoomBtn->show();

    createRoomBtn->disconnect();
    joinRoomBtn->disconnect();
    connect(createRoomBtn, &QPushButton::clicked, this, [this]() {deleteRequestRoomWindow(); setCreateRoomWindow();});
    connect(joinRoomBtn, &QPushButton::clicked, this, [this]() {deleteRequestRoomWindow(); setJoinRoomWindow();});
}

void MainWindow::deleteRequestRoomWindow() {
    createRoomBtn->hide();
    joinRoomBtn->hide();
}

void MainWindow::setCreateRoomWindow() {
    nameLabel->setGeometry(75, 85, 50, 50);
    nameLineEdit->setGeometry(125, 100, 200, 20);
    createRoomBtn->setGeometry(275, 270, 125, 30);
    backBtn->setGeometry(150, 270, 125, 30);

    nameLabel->show();
    nameLineEdit->show();
    createRoomBtn->show();
    backBtn->show();

    createRoomBtn->disconnect();
    backBtn->disconnect();
    connect(backBtn, &QPushButton::clicked, this, [this]() {deleteCreateRoomWindow(); setRequestRoomWindow();});
    connect(createRoomBtn, &QPushButton::clicked, this, &MainWindow::create_room_handler);
}

void MainWindow::deleteCreateRoomWindow() {
    nameLabel->hide();
    nameLineEdit->hide();
    createRoomBtn->hide();
    backBtn->hide();
}

void MainWindow::setJoinRoomWindow() {
    serverNameLabel->setGeometry(50, 85, 100, 50);
    serverPortLabel->setGeometry(60, 105, 100, 50);
    nameLabel->setGeometry(95, 125, 50, 50);
    serverNameLineEdit->setGeometry(140, 100, 200, 20);
    serverPortLineEdit->setGeometry(140, 120, 200, 20);
    nameLineEdit->setGeometry(140, 140, 200, 20);
    joinRoomBtn->setGeometry(275, 270, 125, 30);
    backBtn->setGeometry(150, 270, 125, 30);

    serverNameLabel->show();
    serverNameLineEdit->show();
    serverPortLabel->show();
    serverPortLineEdit->show();
    nameLabel->show();
    nameLineEdit->show();
    joinRoomBtn->show();
    backBtn->show();

    serverNameLineEdit->setText(QString("10.89.219.100"));
    nameLineEdit->setText(QString("Joseph"));

    joinRoomBtn->disconnect();
    backBtn->disconnect();
    connect(backBtn, &QPushButton::clicked, this, [this]() {deleteJoinRoomWindow(); setRequestRoomWindow();});
    connect(joinRoomBtn, &QPushButton::clicked, this, &MainWindow::join_room_handler);
}

void MainWindow::deleteJoinRoomWindow() {
    serverNameLabel->hide();
    serverNameLineEdit->hide();
    serverPortLabel->hide();
    serverPortLineEdit->hide();
    nameLabel->hide();
    nameLineEdit->hide();
    joinRoomBtn->hide();
    backBtn->hide();
}

void MainWindow::setRoomWindow() {
    playerList->setGeometry(10, 10, 200, 250);
    leaveBtn->setGeometry(275, 270, 125, 30);

    playerList->show();
    leaveBtn->show();

    leaveBtn->disconnect();
    connect(leaveBtn, &QPushButton::clicked, this, [this]() {deleteRoomWindow(); destroyRoom(); setRequestRoomWindow();});

    if (isHost) {
        startBtn->setGeometry(150, 270, 125, 30);
        startBtn->show();
        startBtn->disconnect();
    }
}

void MainWindow::deleteRoomWindow() {
    playerList->clear();
    playerList->hide();
    leaveBtn->hide();
    startBtn->hide();
}

void MainWindow::destroyRoom() {
    if (client != nullptr) {
        client->disconnectFromHost();
        client->deleteLater();
        client = nullptr;
    }
    if (server != nullptr) {
        server->deleteLater();
        server = nullptr;
    }
}

void MainWindow::joinRoom(QString ip, quint16 port) {
    client = new Client(this);
    client->connectToServer(QHostAddress(ip), port);
    connect(client, &Client::receiveJson, this, &MainWindow::clientJsonReceived);
    connect(client, &Client::connected, this, &MainWindow::sendPlayerName);
}

void MainWindow::create_room_handler() {
    qDebug("Create Room");
    server = new Server(this);
    connect(server, &Server::receiveJson, this, &MainWindow::jsonReceived);

    playerName = nameLineEdit->text();
    isHost = true;
    joinRoom(server->getIP(), server->getPort());
    qDebug() << server->getIP();
    qDebug() << server->getPort();

    deleteCreateRoomWindow();
    setRoomWindow();
}
void MainWindow::join_room_handler() {
    qDebug("Join Room");
    const QString ip = serverNameLineEdit->text();
    const quint16 port = serverPortLineEdit->text().toUShort();
    playerName = nameLineEdit->text();
    isHost = false;
    joinRoom(ip, port);

    deleteJoinRoomWindow();
    setRoomWindow();
}

void MainWindow::sendPlayerName() {
    qDebug("Send New Player Name");
    QJsonObject playerNameMsg;
    playerNameMsg["type"] = "playerName";
    playerNameMsg["playerName"] = playerName;
    client->sendJson(playerNameMsg);
}

void MainWindow::clientJsonReceived(const QJsonObject &json) {
    qDebug("Client Receive Json");
    const QString type = json.value(QString("type")).toString();
    if (type == "requestName") {
        sendPlayerName();
    } else if (type == "newPlayer") {
        qDebug("Receive New Player");
        addPlayer(json.value(QString("playerName")).toString());
    } else if (type == "playerList") {
        for(QJsonValue playerName : json.value(QString("playerNames")).toArray())
            addPlayer(playerName.toString());
    } else if (type == "playerDisconnected") {
        qDebug("Player Disconnected");
        removePlayer(json.value(QString("playerName")).toString());
    }
}

void MainWindow::jsonReceived(ServerWorker *sender, const QJsonObject &json) {
//    qDebug("Receive Json");
//    const QString type = json.value(QString("type")).toString();
//    if (type == "requestName") {
//        sendPlayerName();
//    } else if (type == "newPlayer") {
//        qDebug("Receive New Player");
//        addPlayer(json.value(QString("playerName")).toString());
//    } else if (type == "playerList") {
//        for(QJsonValue playerName : json.value(QString("playerNames")).toArray())
//            addPlayer(playerName.toString());
//    } else if (type == "playerDisconnected") {

//    }
}

void MainWindow::addPlayer(const QString &playerName) {
    QListWidgetItem *newPlayer = new QListWidgetItem(playerName, playerList);
    newPlayer->setTextAlignment(Qt::AlignCenter);
}

void MainWindow::removePlayer(const QString &playerName) {
    QList<QListWidgetItem*> players = playerList->findItems(playerName, Qt::MatchExactly);
    playerList->removeItemWidget(players.takeFirst());
}
