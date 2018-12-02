#include "gamewindow.h"
#include "ui_game_window.h"
#include "notifywindow.h"
#include <QFontDatabase>
#include <QFont>
#include <QVector>
#include <QMessageBox>
#include <QSizePolicy>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

/*
 *  GameWindow::GameWindow(QWidget *parent, Client* client, int playerNum, QString name, QMap<QString,int> playerNames)
 *  @funct:  creates the GameWindow object and initializes the GUI
 *  @param:  parent: nullptr, client: the client pointer, playerNum: number of players, name: the player's name: playerNames: map of the player names and their order (for GUI purposes)
 *  @return: N/A
 */
GameWindow::GameWindow(QWidget *parent, Client* client, int playerNum, QString name, QMap<QString,int> playerNames) :
    QDialog(parent),
    ui(new Ui::game_window),
    notifyWindow(nullptr),
    handLayout(new QHBoxLayout(this)),
    cardSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed)),
    cardFont(QFont(QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/resource/resource/font/Twisted System.otf")).at(0))),
    textFont(QFont(QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/resource/resource/font/Candy Beans.otf")).at(0))),
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
    ui->currentCardLabel->setAlignment(Qt::AlignCenter);
    setCardStyle(ui->currentCardLabel, "DEFUSE");
    cardFont.setPointSize(30);
    textFont.setPointSize(25);
    ui->endTurnBtn->setStyleSheet("color: #B4091C; background-color: white; border: 4px solid #B4091C; border-radius: 20px");
    ui->endTurnBtn->setFont(cardFont);
    setCardStyle(ui->endTurnBtn, "END TURN");

    ui->currentPlayerLabel->setAlignment(Qt::AlignCenter);
    ui->currentPlayerLabel->setFont(textFont);
    ui->currentPlayerLabel->setStyleSheet("color: white;");
    ui->deckLabel->setAlignment(Qt::AlignCenter);
    ui->preMoveLabel->setFont(textFont);
    ui->preMoveLabel->setStyleSheet("color: white;");
    ui->preMoveLabel->setWordWrap(true);
    ui->preMoveLabel->setAlignment(Qt::AlignCenter);
    textFont.setPointSize(15);
    ui->deckLabel->setFont(textFont);
    ui->deckLabel->setStyleSheet("color: white;");
    cardFont.setPointSize(40);

    for (int i=0;i<playerNum;i++) {

        Player *newPlayer = createNewPlayer(playerNames.key(i,"Player" + QString::number(i)));
        playerLabel.push_back(newPlayer);
        if (playerNames.key(i,"Player" + QString::number(i))!=playerName) ui->playerListLayout->addLayout(newPlayer->layout);
    }
    this->show();
    connect(ui->endTurnBtn, &QPushButton::clicked, this, &GameWindow::endTurnBtnHandler);
    connect(client, &Client::receiveJson, this, &GameWindow::clientJsonReceived);
}

/*
 *  GameWindow::~GameWindow()
 *  @funct:  GameWindow destructor, deletes the dynamically created objects
 *  @param:  N/A
 *  @return: N/A
 */
GameWindow::~GameWindow()
{
    playerLabel.clear();
    playerLabel.squeeze();
    delete notifyWindow;
    delete handLayout;
    delete ui;
}

/*
 *  void GameWindow::endTurnBtnHandler()
 *  @funct:  triggered after the endTurnBtn has been pressed, sends this information to the server (and GameLogic) to be handled
 *  @param:  N/A
 *  @return: N/A
 */
void GameWindow::endTurnBtnHandler(){
    QJsonObject endTurnMsg;
    endTurnMsg["type"] = "endTurn";
    client->sendJson(endTurnMsg);
}

/*
 *  void GameWindow::playCardBtnHandler(int currentCard, QString currentCardString)
 *  @funct:  triggered after a card has been pressed, sends this information to the server (and GameLogic) to be handled
 *  @param:  currentCard: index of the card played, currentCardString: name of the card played
 *  @return: N/A
 */
void GameWindow::playCardBtnHandler(int currentCard, QString currentCardString){
    qDebug() << "Card Played:" + QString::number(currentCard);
    if (currentCardString == "DEFUSE")
        return;
    QJsonObject playCardMsg;
    playCardMsg["type"] = "playCard";
    playCardMsg["card"] = currentCard;
    client->sendJson(playCardMsg);
}

/*
 *  GameWindow::Player* GameWindow::createNewPlayer(QString name)
 *  @funct:  creates the GUI for opponents
 *  @param:  name: name of the opponent
 *  @return: GameWindow::Player*: to be added to playerLabel
 */
GameWindow::Player* GameWindow::createNewPlayer(QString name) {
    Player *player = new Player;
    player->layout = new QVBoxLayout();

    player->icon = new QLabel();
    player->icon->setStyleSheet("background-image:url(\":/resource/resource/image/Kitten.png\"); background-position: center; background-repeat: no-repeat;");
    player->layout->addWidget(player->icon);

    player->name = new QLabel(name);
    player->name->setAlignment(Qt::AlignCenter);
    player->name->setFont(textFont);
    player->name->setStyleSheet("color: white;");
    player->layout->addWidget(player->name);

    player->card = new QLabel("3 cards");
    player->card->setAlignment(Qt::AlignCenter);
    player->card->setFont(textFont);
    player->card->setStyleSheet("color: white; font-weight: bold; font-size: 15px;");
    player->layout->addWidget(player->card);

    player->layout->setStretch(0, 5);
    player->layout->setStretch(1, 1);
    player->layout->setStretch(2, 1);

    return player;
}


/*
 *  void GameWindow::setPlayerCard(Player *player, int cardNum)
 *  @funct:  handles and updates the GUI for opponents
 *  @param:  player: the opposing player, cardNum: the number of cards in their hand
 *  @return: N/A
 */
void GameWindow::setPlayerCard(Player *player, int cardNum) {
    player->card->setText(QString::number(cardNum) + " Cards");
}

void GameWindow::setPlayerDead(Player *player) {
    player->icon->setStyleSheet("background-image:url(\":/resource/resource/image/Exploding_Kitten.png\"); background-position: center; background-repeat: no-repeat;");
    player->name->setStyleSheet("color: #C8C8C8");
    player->card->setStyleSheet("color: #C8C8C8");
}

void GameWindow::setCurrentCard(QString cardType) {
    setCardStyle(ui->currentCardLabel, cardType);
    ui->currentCardLabel->setText(cardType == "SEE_THE_FUTURE" ? "SEE\nTHE\nFUTURE" : cardType);
    ui->currentCardLabel->setFont(cardFont);
}

/*
 *  void GameWindow::newCard(QString cardType)
 *  @funct:  creates the GUI for the player's cards in hand
 *  @param:  cardType: the name of the card to be added to the UI
 *  @return: N/A
 */
void GameWindow::newCard(QString cardType) {
    QPushButton *newCard = new QPushButton(cardType == "SEE_THE_FUTURE" ? "SEE\nTHE\nFUTURE" : cardType);
    connect(newCard, &QPushButton::clicked, this, std::bind(&GameWindow::playCardBtnHandler, this, handLayout->count(), cardType));
    newCard->setSizePolicy(cardSizePolicy);
    newCard->setFixedWidth(125);
    newCard->setFixedHeight(150);
    newCard->setFont(cardFont);
    setCardStyle(newCard, cardType);
    handLayout->addWidget(newCard);
}

/*
 *  void GameWindow::setCardStyle(QWidget *widget, QString cardType)
 *  @funct:  styling for the card GUI
 *  @param:  widget: pointer to the card to be changed, cardType: the desired type/style
 *  @return: N/A
 */
void GameWindow::setCardStyle(QWidget *widget, QString cardType) {
    if (cardType == "DEFUSE")
        widget->setStyleSheet("color: #86C336; background-color: white; border: 7px solid #86C336; border-radius: 20px");
    else if (cardType == "ATTACK")
        widget->setStyleSheet("color: #F15B28; background-color: white; border: 7px solid #F15B28; border-radius: 20px");
    else if (cardType == "SKIP")
        widget->setStyleSheet("color: #2890D1; background-color: white; border: 7px solid #2890D1; border-radius: 20px");
    else if (cardType == "SEE\nTHE\nFUTURE" || cardType == "SEE_THE_FUTURE")
        widget->setStyleSheet("color: #ED167A; background-color: white; border: 7px solid #ED167A; border-radius: 20px");
    else if (cardType == "SHUFFLE")
        widget->setStyleSheet("color: #726357; background-color: white; border: 7px solid #726357; border-radius: 20px");
    else if (cardType == "STEAL")
        widget->setStyleSheet("color: #646464; background-color: white; border: 7px solid #646464; border-radius: 20px");
}

/*
 *  void GameWindow::clearLayout(QLayout *layout)
 *  @funct:  resets the player's hand GUI
 *  @param:  layout: pointer to the player's hand widget
 *  @return: N/A
 */
void GameWindow::clearLayout(QLayout *layout) {
    while (QLayoutItem *item = layout->takeAt(0)) {
        if (QWidget *widget = item->widget())
            widget->deleteLater();
    }
}

/*
 *  void GameWindow::clientJsonReceived(const QJsonObject &json)
 *  @funct:  handler for the client when receiving information from server, updates the GUI accordingly
 *  @param:  json: QJsonObject contianing the GUI information
 *  @return: N/A
 */
void GameWindow::clientJsonReceived(const QJsonObject &json) {
    qDebug("Game Window Client Receive Json");
    const QString type = json.value(QString("type")).toString();
    if (type == "updateUi") {
        ui->deckLabel->setText(QString::number(json.value(QString("deckSize")).toInt()) + " Cards Left");
        const QJsonObject playerHand = json.value(QString("playerHand")).toObject();
        const QJsonObject playerAlive = json.value(QString("playerAlive")).toObject();
        setCurrentCard(json.value("prevCard").toString());
        if (json.value("currentPlayer").toString() == playerName) {
            ui->currentPlayerLabel->setText("Current Player:\n You");
        }else {
            ui->currentPlayerLabel->setText("Current Player:\n" + json.value("currentPlayer").toString());
        }
        ui->preMoveLabel->setText(json.value("prevMove").toString());
        if (json.value(QString("seeTheFutureFlag")).toBool()) {
            if (json.value(QString("currentPlayer")).toString() == playerName) {
                const QJsonArray seeTheFutureCards = json.value(QString("seeTheFuture")).toArray();
                delete notifyWindow;
                notifyWindow = new NotifyWindow(this, NotifyWindow::SHOW_SEE_THE_FUTURE, &seeTheFutureCards);
            }
        }
        if (json.value("drewExplodingKitten").toBool()) {
            const QString explodingPlayer = json.value("explodingPlayer").toString();
            if (explodingPlayer==playerName) {
                if (json.value("successfulDefuse").toBool()) {
                    QMessageBox::information(nullptr, "Exploding Kitten!", "You drew an Exploding Kitten! \n Luckily you have a Defuse to defuse it!");
                }else {
                    delete notifyWindow;
                    notifyWindow = new NotifyWindow(this, NotifyWindow::SHOW_EXPLODING_KITTEN);
                }
            }else {
                if (json.value("successfulDefuse").toBool()) {
                    QMessageBox::information(nullptr, "Exploding Kitten!", explodingPlayer + " drew an Exploding Kitten! \n Unfortunately, he had a Defuse to defuse it!");
                }else {
                    QMessageBox::information(nullptr, "Exploding Kitten!", explodingPlayer + " drew an Exploding Kitten and exploded!");
                    setPlayerDead(playerLabel[playerOrder.value(explodingPlayer)]);
                }
            }
        }
        const QString winner = json.value("winner").toString();
        if (winner != "") {
            if (winner == playerName) {
                QMessageBox::information(nullptr, "Exploding Kitten!", "YOU WIN!");
            } else {
                QMessageBox::information(nullptr, "Exploding Kitten!", winner + " has won the game!");
            }
        }
        for (QString player : playerHand.keys()) {
            if (player == playerName) {
                clearLayout(handLayout);
                for (QJsonValue card : playerHand.value(player).toArray()) {
                    newCard(card.toString());
                }
            }
            if (playerAlive.value(player).toBool()) {
                setPlayerCard(playerLabel[playerOrder.value(player)],playerHand.value(player).toArray().size());
            } else {
                playerLabel[playerOrder.value(player)]->card->setText("EXPLODED");
            }
        }
    }
}

