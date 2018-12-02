#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "server.h"
#include "client.h"

#include <QFont>
#include <QFontDatabase>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QList>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

/*
 *  MainWindow::MainWindow(QWidget *parent)
 *  @funct:  creates the GameWindow object and initializes the GUI
 *  @param:  parent: nullptr
 *  @return: N/A
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    game(nullptr),
    server(nullptr),
    client(nullptr),
    playerName(""),
    ip(""),
    port(0),
    isHost(false),
    isConnect(false)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Exploding Kittens"));
    this->centralWidget()->setStyleSheet("background-image:url(\":/resource/resource/image/ExplodingKittenBackground.png\"); background-position: center;");

    QFont *startWindowBtnFont = new QFont(QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/resource/resource/font/FEASFBRG.TTF")).at(0));
    startWindowBtnFont->setPointSize(35);

    QFont *roomWindowFont = new QFont(QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/resource/resource/font/Candy Beans.otf")).at(0));
    roomWindowFont->setPointSize(15);

    // Start Window
    ui->createRoomBtn->setFont(*startWindowBtnFont);
    ui->createRoomBtn->setStyleSheet("border: none; color: #fcdb20;");
    ui->createRoomBtn->hide();

    ui->joinRoomBtn->setFont(*startWindowBtnFont);
    ui->joinRoomBtn->setStyleSheet("border: none; color: #fcdb20");
    ui->joinRoomBtn->hide();

    startWindowBtnFont->setPointSize(20);
    ui->instructionBtn->setFont(*startWindowBtnFont);
    ui->instructionBtn->setStyleSheet("border: none; color: #fcdb20");
    ui->instructionBtn->hide();

    //Instruction Window
    ui->instructionBackBtn->setFont(*startWindowBtnFont);
    ui->instructionBackBtn->setStyleSheet("border: none; color: #fcdb20");
    ui->instructionBackBtn->hide();

    startWindowBtnFont->setPointSize(35);
    ui->instructionLabel->setFont(*startWindowBtnFont);
    ui->instructionLabel->setStyleSheet("border: none; color: #fcdb20");
    ui->instructionLabel->hide();
    roomWindowFont->setPointSize(12);
    ui->instructionTextLabel->setFont(*roomWindowFont);
     ui->instructionTextLabel->setWordWrap(true);
    ui->instructionTextLabel->setText(
                "Exploding Kittens is a 2-5 player online card game. \n"
                "Each player take turns playing card and drawing from the deck. \n"
                "Each card has a different function:\n"
                "Skip: Ends the player’s turn without drawing\n"
                "Attack: Next player must take two turns\n"
                "Shuffle: Shuffles the deck\n"
                "See the Future: View the top three cards of the deck\n"
                "Steal: Steal a card from the next player’s hand\n"
                "Defuse: Can only be used if the player draws an Exploding Kitten, the Exploding Kitten will be placed back into the deck randomly\n"
                "If the player draws the Exploding Kitten but does not have a Defuse, he loses!\n"
                );
    ui->instructionTextLabel->setStyleSheet("border: none; color: white");
    ui->instructionTextLabel->hide();

    //Room Window
    roomWindowFont->setPointSize(15);
    ui->serverNameLabel->setFont(*roomWindowFont);
    ui->serverNameLabel->setStyleSheet("border:none; color: white;");
    ui->serverNameLabel->hide();

    ui->serverPortLabel->setFont(*roomWindowFont);
    ui->serverPortLabel->setStyleSheet("border:none; color: white;");
    ui->serverPortLabel->hide();

    ui->nameLabel->setFont(*roomWindowFont);
    ui->nameLabel->setStyleSheet("border:none; color: white;");
    ui->nameLabel->hide();

    ui->playerCountLabel->setFont(*roomWindowFont);
    ui->playerCountLabel->setStyleSheet("border:none; color: white;");
    ui->playerCountLabel->hide();

    ui->serverNameDetailLabel->setFont(*roomWindowFont);
    ui->serverNameDetailLabel->setStyleSheet("border:none; color: white;");
    ui->serverNameDetailLabel->hide();

    ui->serverPortDetailLabel->setFont(*roomWindowFont);
    ui->serverPortDetailLabel->setStyleSheet("border:none; color: white;");
    ui->serverPortDetailLabel->hide();

    ui->serverNameLineEdit->hide();
    ui->serverPortLineEdit->hide();
    ui->nameLineEdit->hide();

    ui->playerList->setFont(*roomWindowFont);
    ui->playerList->setStyleSheet("color: white");
    ui->playerList->hide();

    roomWindowFont->setPointSize(20);

    connect(ui->createRoomBtn, &QPushButton::clicked, this, [this]() {deleteRequestRoomWindow(); setCreateRoomWindow();});
    connect(ui->joinRoomBtn, &QPushButton::clicked, this, [this]() {deleteRequestRoomWindow(); setJoinRoomWindow();});
    connect(ui->instructionBtn, &QPushButton::clicked, this, [this]() {deleteRequestRoomWindow(); setInstructionWindow();});
    connect(ui->instructionBackBtn, &QPushButton::clicked,this,[this](){deleteInstructionWindow();setRequestRoomWindow();});
    ui->createBtn->setFont(*roomWindowFont);
    ui->createBtn->setStyleSheet("border: none; color: white;");
    connect(ui->createBtn, &QPushButton::clicked, this, &MainWindow::create_room_handler);
    ui->createBtn->hide();

    ui->backBtn->setFont(*roomWindowFont);
    ui->backBtn->setStyleSheet("border: none; color: white;");
    ui->backBtn->hide();

    ui->joinBtn->setFont(*roomWindowFont);
    ui->joinBtn->setStyleSheet("border: none; color: white;");
    connect(ui->joinBtn, &QPushButton::clicked, this, &MainWindow::join_room_handler);
    ui->joinBtn->hide();

    ui->startBtn->setFont(*roomWindowFont);
    ui->startBtn->setStyleSheet("border: none; color: white;");
    ui->startBtn->hide();

    ui->leaveBtn->setFont(*roomWindowFont);
    ui->leaveBtn->setStyleSheet("border: none; color: white;");
    connect(ui->leaveBtn, &QPushButton::clicked, this, &MainWindow::leaveRoom);
    ui->leaveBtn->hide();

    setRequestRoomWindow();
    delete startWindowBtnFont;
    delete roomWindowFont;
}

/*
 *  MainWindow::~MainWindow()
 *  @funct:  MainWindow destructor for the dynamically created objects
 *  @param:  N/A
 *  @return: N/A
 */
MainWindow::~MainWindow()
{
    ui->playerList->clear();
    delete gameWindow;
    delete game;
    delete server;
    delete client;
    delete ui;
}

/*
 *  void MainWindow::set[...]Window()/void MainWindow::delete[...]Window()
 *  @funct:  sets and resets the GUI for different windows
 *  @param:  N/A
 *  @return: N/A
 */
void MainWindow::setInstructionWindow(){
    ui->instructionBackBtn->show();
    ui->instructionLabel->show();
    ui->instructionTextLabel->show();
}
void MainWindow::deleteInstructionWindow(){
    ui->instructionBackBtn->hide();
    ui->instructionLabel->hide();
    ui->instructionTextLabel->hide();
}


void MainWindow::setRequestRoomWindow() {
    ui->createRoomBtn->show();
    ui->joinRoomBtn->show();
    ui->instructionBtn->show();
}
void MainWindow::deleteRequestRoomWindow() {
    ui->createRoomBtn->hide();
    ui->joinRoomBtn->hide();
    ui->instructionBtn->hide();
}

void MainWindow::setCreateRoomWindow() {
    ui->nameLabel->setGeometry(75, 130, 50, 20);
    ui->nameLineEdit->setGeometry(125, 130, 200, 20);
    ui->nameLineEdit->setFocus();

    ui->nameLabel->show();
    ui->nameLineEdit->show();
    ui->createBtn->show();
    ui->backBtn->show();

    ui->backBtn->disconnect();
    connect(ui->backBtn, &QPushButton::clicked, this, [this]() {deleteCreateRoomWindow(); setRequestRoomWindow();});
}
void MainWindow::deleteCreateRoomWindow() {
    ui->nameLabel->hide();
    ui->nameLineEdit->hide();
    ui->createBtn->hide();
    ui->backBtn->hide();
}

void MainWindow::setJoinRoomWindow() {
    ui->nameLabel->setGeometry(100, 180, 50, 20);
    ui->nameLineEdit->setGeometry(150, 180, 200, 20);

    ui->serverNameLineEdit->setFocus();

    ui->serverNameLabel->show();
    ui->serverPortLabel->show();
    ui->serverNameLineEdit->show();
    ui->serverPortLineEdit->show();
    ui->nameLabel->show();
    ui->nameLineEdit->show();
    ui->joinBtn->show();
    ui->backBtn->show();

    ui->backBtn->disconnect();
    connect(ui->backBtn, &QPushButton::clicked, this, [this]() {deleteJoinRoomWindow(); setRequestRoomWindow();});
}
void MainWindow::deleteJoinRoomWindow() {
    ui->serverNameLabel->hide();
    ui->serverPortLabel->hide();
    ui->serverNameLineEdit->hide();
    ui->serverPortLineEdit->hide();
    ui->nameLabel->hide();
    ui->nameLineEdit->hide();
    ui->joinBtn->hide();
    ui->backBtn->hide();
}

void MainWindow::setRoomWindow() {
    ui->serverNameDetailLabel->setText("Server Name:\n" + ip);
    ui->serverPortDetailLabel->setText("Server Port:\n" + QString::number(port));
    ui->playerCountLabel->setText(QString("Player Count:"));;

    ui->playerCountLabel->show();
    ui->serverNameDetailLabel->show();
    ui->serverPortDetailLabel->show();
    ui->playerList->show();
    ui->leaveBtn->show();

    if (isHost) {
        ui->startBtn->disconnect();
        connect(ui->startBtn, &QPushButton::clicked, server, &Server::startGameBroadcast);
        ui->startBtn->show();
    }
}
void MainWindow::deleteRoomWindow() {
    ui->playerList->clear();
    ui->playerList->hide();
    ui->playerCountLabel->hide();
    ui->serverNameDetailLabel->hide();
    ui->serverPortDetailLabel->hide();
    ui->leaveBtn->hide();
    ui->startBtn->hide();
}

/*
 *  void MainWindow::destroyRoom()
 *  @funct:  deletes the room.
 *  @param:  N/A
 *  @return: N/A
 */
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

/*
 *  void MainWindow::joinRoom()
 *  @funct:  joins the room, connecting the client to the server
 *  @param:  N/A
 *  @return: N/A
 */
void MainWindow::joinRoom() {
    client = new Client(this);
    client->connectToServer(QHostAddress(ip), port);
    connect(client, &Client::receiveJson, this, &MainWindow::clientJsonReceived);
    connect(client, &Client::connected, this, &MainWindow::sendPlayerName);
    connect(client, &Client::disconnected, this, &MainWindow::forceLeaveRoom);
}

/*
 *  void MainWindow::leaveRoom()
 *  @funct:  for leaving the room
 *  @param:  N/A
 *  @return: N/A
 */
void MainWindow::leaveRoom() {
    QMessageBox::StandardButton reply = QMessageBox::question(this, QString("Leave Room"), QString("Are you sure to leave the room?"));
    if (reply == QMessageBox::Yes) {
        isConnect = false;
        deleteRoomWindow();
        destroyRoom();
        setRequestRoomWindow();
    }
}

void MainWindow::leaveRoomNoPrompt() {
    if (gameWindow) delete gameWindow;
    isConnect = false;
    deleteRoomWindow();
    destroyRoom();
    setRequestRoomWindow();
}

/*
 *  void MainWindow::forceLeaveRoom()
 *  @funct:  if the host exists the room, all others will be forced out of the room
 *  @param:  N/A
 *  @return: N/A
 */
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

/*
 *  void MainWindow::addPlayer(const QString &playerName)
 *  @funct:  adding the player to the room
 *  @param:  playerName: name of the player to be added
 *  @return: N/A
 */
void MainWindow::addPlayer(const QString &playerName) {
    QListWidgetItem *newPlayer = new QListWidgetItem(playerName, ui->playerList);
    newPlayer->setTextAlignment(Qt::AlignCenter);
    newPlayer->setFlags(newPlayer->flags() & ~Qt::ItemIsSelectable);
    ui->playerCountLabel->setText(QString("Player Count: ") + QString::number(ui->playerList->count()));
}

/*
 *  void MainWindow::removePlayer(const QString &playerName)
 *  @funct:  removes the player to the room
 *  @param:  playerName: name of the player to be removed
 *  @return: N/A
 */
void MainWindow::removePlayer(const QString &playerName) {
    QList<QListWidgetItem*> players = ui->playerList->findItems(playerName, Qt::MatchExactly);
    delete players.takeFirst();
    ui->playerCountLabel->setText(QString("Player Count: ") + QString::number(ui->playerList->count()));
}

/*
 *  void MainWindow::startGame()
 *  @funct:  starts the game, creating the GameWindow (and GameLogic for the host)
 *  @param:  N/A
 *  @return: N/A
 */
void MainWindow::startGame(){
    if (isHost) game = new GameLogic(server);
    QVector<QString> allPlayerNames;
    QMap<QString,int> playerNameMap;
    int playerNum = ui->playerList->row((ui->playerList->findItems(playerName, Qt::MatchExactly))[0]);
    for(int i = 0; i < ui->playerList->count(); ++i){
        if (i < playerNum) {
            playerNameMap[ui->playerList->item(i)->text()] = i-playerNum+ui->playerList->count();
            qDebug() <<ui->playerList->item(i)->text() << i-playerNum+ui->playerList->count();
        }else {
            playerNameMap[ui->playerList->item(i)->text()] = i-playerNum;
            qDebug() <<ui->playerList->item(i)->text() << i-playerNum;
        }
    }

    gameWindow = new GameWindow(nullptr, client,ui->playerList->count(), playerName,playerNameMap);
    connect(gameWindow,&GameWindow::close,this,&MainWindow::leaveRoomNoPrompt);
    this->hide();
}

/*
 *  void MainWindow::create_room_handler()
 *  @funct:  handler for the createRoomBtn
 *  @param:  N/A
 *  @return: N/A
 */
void MainWindow::create_room_handler() {
    playerName = ui->nameLineEdit->text().simplified();
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

/*
 *  void MainWindow::join_room_handler()
 *  @funct:  handler for the joinRoomBtn
 *  @param:  N/A
 *  @return: N/A
 */
void MainWindow::join_room_handler() {
    ip = ui->serverNameLineEdit->text().simplified();
    if (ip.isEmpty()) {
        QMessageBox::information(this, QString("Empty Server Name"), QString("Server name cannot be empty"));
        return;
    }

    port = ui->serverPortLineEdit->text().simplified().toUShort();
    if (!port) {
        QMessageBox::information(this, QString("Invalid Port"), QString("Invalid Port"));
        return;
    }

    playerName = ui->nameLineEdit->text().simplified();
    if (playerName.isEmpty()) {
        QMessageBox::information(this, QString("Empty Name"), QString("Name cannot be empty"));
        return;
    }

    isHost = false;
    joinRoom();
}

/*
 *  void MainWindow::sendPlayerName()
 *  @funct:  sends the player's name to the server
 *  @param:  N/A
 *  @return: N/A
 */
void MainWindow::sendPlayerName() {
    QJsonObject playerNameMsg;
    playerNameMsg["type"] = "playerName";
    playerNameMsg["playerName"] = playerName;
    client->sendJson(playerNameMsg);
}

/*
 *  void MainWindow::clientJsonReceived(const QJsonObject &json)
 *  @funct:  QJson handler for the room
 *  @param:  N/A
 *  @return: N/A
 */
void MainWindow::clientJsonReceived(const QJsonObject &json) {
    qDebug() << json;
    const QString type = json.value(QString("type")).toString();
    if (type == "newPlayer") {
        addPlayer(json.value(QString("playerName")).toString());
    } else if (type == "playerList") {
        for(QJsonValue playerName : json.value(QString("playerNames")).toArray())
            addPlayer(playerName.toString());
        isConnect = true;
        deleteJoinRoomWindow();
        setRoomWindow();
    } else if (type == "playerDisconnected") {
        removePlayer(json.value(QString("playerName")).toString());
    } else if (type == "startGame") {
        startGame();
    } else if (type == "playerFull") {
        QMessageBox::information(this, "Room Full", "The Room is Full");
        destroyRoom();
    } else if (type == "nameRepeat") {
        QMessageBox::information(this, "Name Repeated", "The Name has been used by another player");
        destroyRoom();
    }
}

