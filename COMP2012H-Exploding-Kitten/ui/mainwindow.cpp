#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "server.h"
#include "client.h"

#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
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
    playerCountLabel(new QLabel(this)),
    serverNameDetailLabel(new QLabel(this)),
    serverPortDetailLabel(new QLabel(this)),
    server(nullptr),
    client(nullptr),
    playerName(""),
    isHost(false),
    isConnect(false)
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
    playerCountLabel->setGeometry(10, 5, 200, 20);
    playerList->setGeometry(10, 25, 200, 230);
    serverNameDetailLabel->setGeometry(220, 0, 200, 50);
    serverPortDetailLabel->setGeometry(220, 50, 200, 50);
    leaveBtn->setGeometry(275, 270, 125, 30);

    serverNameDetailLabel->setText(QString("Server Name:\n") + ip);
    serverPortDetailLabel->setText(QString("Server Port:\n") + QString::number(port));
    playerCountLabel->setText(QString("Player Count:"));

    playerCountLabel->show();
    playerList->show();
    serverNameDetailLabel->show();
    serverPortDetailLabel->show();
    leaveBtn->show();

    leaveBtn->disconnect();
    connect(leaveBtn, &QPushButton::clicked, this, &MainWindow::leaveRoom);

    if (isHost) {
        startBtn->setGeometry(150, 270, 125, 30);
        startBtn->show();
        connect(startBtn, &QPushButton::clicked, server, &Server::startGameBroadcast);
    }
}

void MainWindow::deleteRoomWindow() {
    playerList->clear();
    playerList->hide();
    playerCountLabel->hide();
    serverNameDetailLabel->hide();
    serverPortDetailLabel->hide();
    leaveBtn->hide();
    startBtn->hide();
}

void MainWindow::destroyRoom() {
    if (server != nullptr) {
        server->stopServer();
        server->deleteLater();
        server = nullptr;
    }
    if (client != nullptr) {
        client->disconnectFromHost();
        client->deleteLater();
        client = nullptr;
    }
}

void MainWindow::joinRoom() {
    client = new Client(this);
    client->connectToServer(QHostAddress(ip), port);
    connect(client, &Client::receiveJson, this, &MainWindow::clientJsonReceived);
    connect(client, &Client::connected, this, &MainWindow::sendPlayerName);
    connect(client, &Client::connected, this, [this]() {isConnect = true;});
    connect(client, &Client::disconnected, this, &MainWindow::forceLeaveRoom);
}

void MainWindow::create_room_handler() {
    qDebug("Create Room");
    playerName = nameLineEdit->text().simplified();
    if (playerName.isEmpty()) {
        QMessageBox::information(this, QString("Empty Name"), QString("Name cannot be empty"));
        return;
    }

    server = new Server(this);
    ip = server->getIP();
    port = server->getPort();
    isHost = true;
    joinRoom();

    deleteCreateRoomWindow();
    setRoomWindow();
}

void MainWindow::join_room_handler() {
    qDebug("Join Room");
    ip = serverNameLineEdit->text().simplified();
    if (ip.isEmpty()) {
        QMessageBox::information(this, QString("Empty Server Name"), QString("Server name cannot be empty"));
        return;
    }

    port = serverPortLineEdit->text().simplified().toUShort();
    if (!port) {
        QMessageBox::information(this, QString("Invalid Port"), QString("Invvalid Port"));
        return;
    }

    playerName = nameLineEdit->text().simplified();
    if (playerName.isEmpty()) {
        QMessageBox::information(this, QString("Empty Name"), QString("Name cannot be empty"));
        return;
    }

    isHost = false;
    joinRoom();

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
    } else if (type == "startGame") {
        qDebug("Game Started");
        startGame();
    }
}

void MainWindow::addPlayer(const QString &playerName) {
    QListWidgetItem *newPlayer = new QListWidgetItem(playerName, playerList);
    newPlayer->setTextAlignment(Qt::AlignCenter);
    playerCountLabel->setText(QString("Player Count: ") + QString::number(playerList->count()));
}

void MainWindow::removePlayer(const QString &playerName) {
    QList<QListWidgetItem*> players = playerList->findItems(playerName, Qt::MatchExactly);
    for (QListWidgetItem *player : players) {
        qDebug() << player->text();
    }
    delete players.takeFirst();
    playerCountLabel->setText(QString("Player Count: ") + QString::number(playerList->count()));
}

void MainWindow::leaveRoom() {
    QMessageBox::StandardButton reply = QMessageBox::question(this, QString("Leave Room"), QString("Are you sure to leave the room?"));
    if (reply == QMessageBox::Yes) {
        isConnect = false;
        deleteRoomWindow();
        destroyRoom();
        setRequestRoomWindow();
    }
}

void MainWindow::forceLeaveRoom() {
    if (!isConnect)
        return;
    QMessageBox::information(this, QString("Server Disconnected"), QString("The host server is disconnected."));
    isConnect = false;
    if (gameWindow) delete gameWindow;
    deleteRoomWindow();
    destroyRoom();
    setRequestRoomWindow();
    this->show();
}

void MainWindow::startGame(){
    if (isHost) game = new GameLogic(server);
    QVector<QString> allPlayerNames;
    QMap<QString,int> playerNameMap;
    int playerNum = playerList->row((playerList->findItems(playerName, Qt::MatchExactly))[0]);
    for(int i = 0; i < playerList->count(); ++i){
        if (i < playerNum) {
            playerNameMap[playerList->item(i)->text()] = i-playerNum+playerList->count();
            qDebug() <<playerList->item(i)->text() << i-playerNum+playerList->count();
        }else {
            playerNameMap[playerList->item(i)->text()] = i-playerNum;
            qDebug() <<playerList->item(i)->text() << i-playerNum;
        }
    }

    gameWindow = new GameWindow(nullptr, client,playerList->count(), playerName,playerNameMap);
    this->hide();
}
