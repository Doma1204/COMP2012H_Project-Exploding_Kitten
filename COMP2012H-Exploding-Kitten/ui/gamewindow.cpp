#include "gamewindow.h"
#include "ui_game_window.h"
#include <QMessageBox>
GameWindow::GameWindow(QWidget *parent, Client* client, int playerNum, QString name, QMap<QString,int> playerNames) :
    QDialog(parent),
    ui(new Ui::game_window),
    client(client),
    playerOrder(playerNames),
    playerName(name)
{
    ui->setupUi(this);
    //Game UI
    this->setWindowTitle(QString("Exploding Kittens"));
    endTurnBtn = new QPushButton(QString("End Turn"),this);
    playCardBtn = new QPushButton(QString("Play Card"),this);
    deckLabel = new QLabel(QString("Deck Size: 30 Cards"),this);
    currentPlayerLabel = new QLabel(QString("Current Player: "),this);
    handList = new QListWidget(this);
    recentMove = new QLabel("temp",this);
    for (int i=0;i<playerNum;i++) {
        playerLabel.push_back(new QLabel(QString("Player "),this));
    }
    playerLabel[0]->setGeometry(330,430,121,41);
    switch(playerNum) {
        case 2:
            playerLabel[1]->setGeometry(320,30,121,41);
            break;
        case 3:
            playerLabel[1]->setGeometry(185,30,121,41);
            playerLabel[2]->setGeometry(450,30,121,41);
            break;
        case 4:
            playerLabel[1]->setGeometry(50,30,121,41);
            playerLabel[2]->setGeometry(320,30,121,41);
            playerLabel[3]->setGeometry(580,30,131,41);
            break;
    }
    endTurnBtn->setGeometry(661,470,91,31);
    playCardBtn->setGeometry(661,510,91,31);
    deckLabel->setGeometry(320,280,161,31);
    currentPlayerLabel->setGeometry(320,200,161,31);
    handList->setGeometry(60,460,561,91);
    recentMove->setGeometry(380, 250,161,31);
    handList->show();
    endTurnBtn->show();
    playCardBtn->show();
    deckLabel->show();
    currentPlayerLabel->show();
    recentMove->show();
    for (QLabel* label : playerLabel) {
        label->show();
    }
    handList->show();
    handList->setFlow(QListWidget::LeftToRight);
    this->show();
    for (int i=0;i<playerNum;i++) {
        playerLabel[i]->setText(playerOrder.key(i,"Error")+": 5 Cards");
    }
    connect(endTurnBtn, &QPushButton::clicked,this, &GameWindow::endTurnBtnHandler);
    connect(playCardBtn, &QPushButton::clicked,this,&GameWindow::playCardBtnHandler);
    connect(client, &Client::receiveJson, this, &GameWindow::clientJsonReceived);
}

GameWindow::~GameWindow()
{
    delete ui;
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
    playCardMsg["card"] = handList->row(handList->currentItem());
    client->sendJson(playCardMsg);
}

void GameWindow::clientJsonReceived(const QJsonObject &json) {
    qDebug("Game Window Client Receive Json");
    const QString type = json.value(QString("type")).toString();
    if (type == "updateUi") {
        deckLabel->setText(QString("Deck: ") + QString::number(json.value(QString("deckSize")).toInt())+QString(" Cards Remaining"));
        const QJsonObject playerHand = json.value(QString("playerHand")).toObject();
        const QJsonObject playerAlive = json.value(QString("playerAlive")).toObject();
        for (QString player : playerHand.keys()) {
            if (player == playerName) {
                handList->clear();
                for (QJsonValue card : playerHand.value(player).toArray()) {
                    card.toString();
                    QListWidgetItem *newPlayer = new QListWidgetItem(card.toString(), handList);
                    newPlayer->setTextAlignment(Qt::AlignCenter);
                }
            }
            if (playerAlive.value(player).toBool()) {
                playerLabel[playerOrder.value(player)]->setText(player+": "+ QString::number(playerHand.value(player).toArray().size())+ " Cards");
            } else {
                playerLabel[playerOrder.value(player)]->setText(player+": Exploded");
            }
        }
        currentPlayerLabel->setText(QString("Current Player: ")+json.value(QString("currentPlayer")).toString());
        recentMove->setText(json.value("prevMove").toString());
    }
}

