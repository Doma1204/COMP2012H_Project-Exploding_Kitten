#include "gamewindow.h"
#include "ui_game_window.h"
#include <QMessageBox>
GameWindow::GameWindow(QWidget *parent, Client* client, int playerNum, QString name) :
    QDialog(parent),
    ui(new Ui::game_window),
    client(client),
    playerName(name)
{
    ui->setupUi(this);
    //Game UI
    this->setWindowTitle(QString("Exploding Kittens"));
    endTurnBtn = new QPushButton(QString("End Turn"),this);
    playCardBtn = new QPushButton(QString("Play Card"),this);
    deckLabel = new QLabel(QString("Deck Size: 30 Cards"),this);
    currentPlayerLabel = new QLabel(QString("Current Player: Player 1"),this);
    botPlayerLabel = new QLabel(QString("Player 1: 5 cards"),this);
    leftPlayerLabel = new QLabel(QString("Player 2: 5 Cards"),this);
    topPlayerLabel = new QLabel(QString("Player 3: 5 Cards"),this);
    rightPlayerLabel = new QLabel(QString("Player 4: 5 Cards"),this);
    handList = new QListWidget(this);

    endTurnBtn->setGeometry(661,470,91,31);
    playCardBtn->setGeometry(661,510,91,31);
    deckLabel->setGeometry(320,280,161,31);
    currentPlayerLabel->setGeometry(320,200,161,31);
    botPlayerLabel->setGeometry(330,430,51,41);
    leftPlayerLabel->setGeometry(50,30,121,41);
    topPlayerLabel->setGeometry(320,30,121,41);
    rightPlayerLabel->setGeometry(580,30,131,41);
    handList->setGeometry(60,460,561,91);
    handList->show();
    endTurnBtn->show();
    playCardBtn->show();
    deckLabel->show();
    currentPlayerLabel->show();
    botPlayerLabel->show();
    leftPlayerLabel->show();
    topPlayerLabel->show();
    rightPlayerLabel->show();
    handList->show();
    handList->setFlow(QListWidget::LeftToRight);
    this->show();
    connect(endTurnBtn, &QPushButton::clicked,this, &GameWindow::endTurnBtnHandler);
    connect(playCardBtn, &QPushButton::clicked,this,&GameWindow::playCardBtnHandler);
    connect(client, &Client::receiveJson, this, &GameWindow::clientJsonReceived);
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::setGameWindow(){
    endTurnBtn->setGeometry(661,470,91,31);
    playCardBtn->setGeometry(661,510,91,31);
    deckLabel->setGeometry(320,280,161,31);
    currentPlayerLabel->setGeometry(320,200,161,31);
    botPlayerLabel->setGeometry(330,430,51,41);
    leftPlayerLabel->setGeometry(50,30,121,41);
    topPlayerLabel->setGeometry(320,30,121,41);
    rightPlayerLabel->setGeometry(580,30,131,41);
    handList->setGeometry(60,460,561,91);
    handList->show();
    endTurnBtn->show();
    endTurnBtn->setAutoDefault(false);
    playCardBtn->show();
    deckLabel->show();
    currentPlayerLabel->show();
    botPlayerLabel->show();
    leftPlayerLabel->show();
    topPlayerLabel->show();
    rightPlayerLabel->show();
    handList->show();
    this->show();
}


void GameWindow::endTurnBtnHandler(){
    QJsonObject endTurnMsg;
    endTurnMsg["type"] = "endTurn";
    client->sendJson(endTurnMsg);
}
void GameWindow::playCardBtnHandler(){
    if (handList->currentItem() == nullptr) {
        QMessageBox::information(nullptr, QString("No Card Selected"), QString("Please select a card."));
        return;
    }QJsonObject playCardMsg;
    playCardMsg["type"] = "playCard";
    playCardMsg["card"] = handList->currentItem()->text();
    client->sendJson(playCardMsg);
}

void GameWindow::clientJsonReceived(const QJsonObject &json) {
    qDebug("Game Window Client Receive Json");
    const QString type = json.value(QString("type")).toString();
    if (type == "updateUi") {
        deckLabel->setText(QString("Cards Remaining: ") + QString(json.value(QString("deckSize")).toInt()));
        const QJsonObject playerHand = json.value(QString("playerHand")).toObject();
//        for (QJsonObject::const_iterator it = playerHand.begin(); it != playerHand.end();it++) {
//            QString player = it.key();
//            if (player == playerName) {

//            } else {

//            }
//        }
        for (QString player : playerHand.keys()) {
            if (player == playerName) {
                handList->clear();
                for (QJsonValue card : playerHand.value(player).toArray()) {
                    card.toString();
                    QListWidgetItem *newPlayer = new QListWidgetItem(card.toString(), handList);
                    newPlayer->setTextAlignment(Qt::AlignCenter);
                }
            }else {
                playerHand.value(player).toArray().size();
            }

        }
//        gameUiInfo["type"] = "updateUi";
//        gameUiInfo["deckSize"] = QString::number(deck.size());
//        gameUiInfo["playerHand"] = playerHand;
//        gameUiInfo["playerAlive"] = playerAlive;
    }
}

