#include "gamewindow.h"
#include "ui_game_window.h"
#include <QFontDatabase>
#include <QFont>
#include <QMessageBox>
#include <QSizePolicy>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

GameWindow::GameWindow(QWidget *parent, Client* client, int playerNum, QString name, QMap<QString,int> playerNames) :
    QDialog(parent),
    ui(new Ui::game_window),
    handLayout(new QHBoxLayout(this)),
    cardSizePolicy(new QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed)),
    cardFont(new QFont(QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/resource/resource/font/Twisted System.otf")).at(0))),
    textFont(new QFont(QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/resource/resource/font/Candy Beans.otf")).at(0))),
    client(client),
    playerOrder(playerNames),
    playerName(name)
{
    ui->setupUi(this);
    //Game UI
    this->setWindowTitle(QString("Exploding Kittens"));
    this->setStyleSheet("background-color: #C83232;");
    ui->scrollAreaWidgetContents->setLayout(handLayout);
    ui->scrollArea->setStyleSheet("border: none;");
    setCardStyle(ui->currentCardLabel, "DEFUSE");
    cardFont->setPointSize(30);
    textFont->setPointSize(25);
    ui->endTurnBtn->setStyleSheet("color: #B4091C; background-color: white; border: 4px solid #B4091C; border-radius: 20px");
    ui->endTurnBtn->setFont(*cardFont);
    setCardStyle(ui->endTurnBtn, "END TURN");
    ui->currentPlayerLabel->setAlignment(Qt::AlignCenter);
    ui->currentPlayerLabel->setFont(*textFont);
    ui->currentPlayerLabel->setStyleSheet("color: white;");



    cardFont->setPointSize(40);

    for (int i=0;i<playerNum;i++) {
        Player *newPlayer = createNewPlayer("Player" + QString::number(i));
        playerLabel.push_back(newPlayer);
        ui->playerListLayout->addLayout(newPlayer->layout);
    }

    this->show();

    //    recentMove = new QLabel("temp",this);

    connect(ui->endTurnBtn, &QPushButton::clicked, this, &GameWindow::endTurnBtnHandler);
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
void GameWindow::playCardBtnHandler(QPushButton *card, QString currentCard){
    delete card;
    QJsonObject playCardMsg;
    playCardMsg["type"] = "playCard";
    playCardMsg["card"] = currentCard;
    client->sendJson(playCardMsg);
}

GameWindow::Player* GameWindow::createNewPlayer(QString name) {
    Player *player = new Player;
    player->layout = new QVBoxLayout();

    player->icon = new QLabel();
    player->icon->setStyleSheet("background-image:url(\":/resource/resource/image/Kitten.png\"); background-position: center; background-repeat: no-repeat;");
    player->layout->addWidget(player->icon);

    player->name = new QLabel(name);
    player->name->setAlignment(Qt::AlignCenter);
    player->name->setFont(*textFont);
    player->name->setStyleSheet("color: white;");
    player->layout->addWidget(player->name);

    player->card = new QLabel("3 cards");
    player->card->setAlignment(Qt::AlignCenter);
    player->card->setStyleSheet("color: white; font-weight: bold; font-size: 15px;");
    player->layout->addWidget(player->card);

    player->layout->setStretch(0, 5);
    player->layout->setStretch(1, 1);
    player->layout->setStretch(2, 1);

    return player;
}

void GameWindow::setPlayerCard(Player *player, int cardNum) {
    player->card->setText(QString::number(cardNum) + " Cards");
}

void GameWindow::setPlayerExploding(Player *player) {
    // TODO: make the player icon to be exploding kitten
}

void GameWindow::setPlayerDead(Player *player) {
    // TODO: make the player icon to a dead icon and make the text color lighter
}

void GameWindow::newCard(QString cardType) {
    if (cardType == "SEE_THE_FUTURE")
        cardType = "SEE\nTHE\nFUTURE";
    QPushButton *newCard = new QPushButton(cardType);
    connect(newCard, &QPushButton::clicked, this, std::bind(&GameWindow::playCardBtnHandler, this, newCard, cardType));
    newCard->setSizePolicy(*cardSizePolicy);
    newCard->setFixedWidth(125);
    newCard->setFixedHeight(150);
    newCard->setFont(*cardFont);
    setCardStyle(newCard, cardType);
    handLayout->addWidget(newCard);
}

void GameWindow::setCardStyle(QWidget *widget, QString cardType) {
    if (cardType == "DEFUSE")
        widget->setStyleSheet("color: #86C336; background-color: white; border: 7px solid #86C336; border-radius: 20px");
    else if (cardType == "ATTACK")
        widget->setStyleSheet("color: #F15B28; background-color: white; border: 7px solid #F15B28; border-radius: 20px");
    else if (cardType == "SKIP")
        widget->setStyleSheet("color: #2890D1; background-color: white; border: 7px solid #2890D1; border-radius: 20px");
    else if (cardType == "SEE\nTHE\nFUTURE")
        widget->setStyleSheet("color: #ED167A; background-color: white; border: 7px solid #ED167A; border-radius: 20px");
    else if (cardType == "SHUFFLE")
        widget->setStyleSheet("color: #726357; background-color: white; border: 7px solid #726357; border-radius: 20px");
}

void GameWindow::clearLayout(QLayout *layout) {
    while (QLayoutItem *item = layout->takeAt(0)) {
        if (QWidget *widget = item->widget())
            widget->deleteLater();
    }
}

void GameWindow::clientJsonReceived(const QJsonObject &json) {
    qDebug("Game Window Client Receive Json");
    const QString type = json.value(QString("type")).toString();
    if (type == "updateUi") {
        ui->deckLabel->setText(QString::number(json.value(QString("deckSize")).toInt()) + " Cards Left");
        const QJsonObject playerHand = json.value(QString("playerHand")).toObject();
        const QJsonObject playerAlive = json.value(QString("playerAlive")).toObject();
        for (QString player : playerHand.keys()) {
            if (player == playerName) {
                clearLayout(handLayout);
                for (QJsonValue card : playerHand.value(player).toArray()) {
                    newCard(card.toString());
                }
            }
//            if (playerAlive.value(player).toBool()) {
//                playerLabel[playerOrder.value(player)]->setText(player+": "+ QString::number(playerHand.value(player).toArray().size())+ " Cards");
//            } else {
//                playerLabel[playerOrder.value(player)]->setText(player+": Exploded");
//            }
        }
        ui->currentPlayerLabel->setText("Current Player:\n" + json.value("currentPlayer").toString());
//        recentMove->setText(json.value("prevMove").toString());
        if (json.value(QString("seeTheFutureFlag")).toBool()) {
            if (json.value(QString("currentPlayer")).toString() == playerName) {
                const QJsonArray seeTheFutureCards = json.value(QString("seeTheFuture")).toArray();
                QMessageBox::information(nullptr, "See The Future", "Top Card: "+ seeTheFutureCards[0].toString()
                                         +"\n Second Card: " + seeTheFutureCards[1].toString()
                                         +"\n Third Card: " + seeTheFutureCards[2].toString());
            }
        }
    }
}
