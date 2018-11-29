#include "gamelogic.h"
#include <QJsonDocument>
#include <QMessageBox>
int myrand(int i) {return rand()% i;}
GameLogic::GameLogic(Server* ser) :
    server(ser),
    attacked(false),
    skipped(false),
    gameEnded(false)
{
    for (ServerWorker *worker : server->getClients()) {
        playerAlive[worker->getPlayerName()] = QJsonValue(true);
        QJsonArray hand;
        playerHand[worker->getPlayerName()] = hand;
    }

    #define X(a,b) \
        for (int i=0;i<(b);i++) { \
            deck.push_back(a); \
        }
    INITIAL_DECK
    #undef X

    random_shuffle( deck.begin(), deck.end(),myrand );

    for (QString player: playerHand.keys()) {
        for (int j=0;j<INITIAL_HAND_SIZE;j++) {
            addToPlayerHand(drawCard(),player);
        }
    }

    for (int i=1;i<playerAliveNum();i++) {
        deck.push_back(EXPLODING_KITTEN);
    }
    currentPlayer = playerAlive.begin().key();
    prevMove = currentPlayer + "'s Turn to move.";
    random_shuffle( deck.begin(), deck.end() ,myrand);
    connect(server, &Server::receiveJson, this, &GameLogic::receiveJson);
    updateAllUi();
}

CARD_TYPE GameLogic::drawCard() {
    CARD_TYPE temp = deck.back();
    deck.pop_back();
    return temp;
}

void GameLogic::addToPlayerHand(CARD_TYPE card, QString playerName){
    if (card == EXPLODING_KITTEN) {

        //TODO: let player choose:
        if (defuse(playerName)) {

            deck.insert(deck.begin()+rand()%deck.size(),EXPLODING_KITTEN);

        } else {
            playerAlive[playerName] = QJsonValue(false);
            //TODO::lose
        }
    } else {
        QJsonArray temp = playerHand[playerName].toArray();
        temp.push_back(cardName[card]);
        playerHand[playerName] = temp;
    }
}

bool GameLogic::defuse(QString playerName) {
    bool has_defuse = false;
    for (QJsonArray::iterator it = playerHand[playerName].toArray().begin(); it<playerHand[playerName].toArray().end();it++) {
        if (it->toString() == "DEFUSE") {
            playerHand[playerName].toArray().erase(it);
            has_defuse = true;
            break;
        }
    }
    return has_defuse;
}


void GameLogic::playerPlayCard(QString card) {

}


void GameLogic::endTurn(){
    if (skipped) { skipped = false; }
    else if (attacked) {attacked = false;}
    else {
        addToPlayerHand(drawCard(),currentPlayer);
    }
    if (playerAliveNum() == 1) {
        QJsonObject::iterator it = playerAlive.find(currentPlayer);
        do {
            if (++it == playerAlive.end()) it = playerAlive.begin();
        }while (it != playerAlive.find(currentPlayer) && !(it.value().toBool()));
        QMessageBox::information(nullptr, QString(" "),  it.key() + QString(" has won the game!"));
        gameEnded = true;
    } else {
        prevMove = currentPlayer + " drew a card and passed.";
        QJsonObject::iterator it = playerAlive.find(currentPlayer);
        do {
            if (++it == playerAlive.end()) it = playerAlive.begin();
        }while (it != playerAlive.find(currentPlayer) && !(it.value().toBool()));
        currentPlayer = it.key();
    }
}

void GameLogic::updateAllUi(){
    if (gameEnded) return;
    QJsonObject gameUiInfo;
    gameUiInfo["type"] = "updateUi";
    gameUiInfo["deckSize"] = (int) deck.size();
    gameUiInfo["playerHand"] = playerHand;
    gameUiInfo["playerAlive"] = playerAlive;
    gameUiInfo["currentPlayer"] = currentPlayer;
    gameUiInfo["prevMove"] = prevMove;
    QJsonDocument doc(gameUiInfo);
    QByteArray bytes = doc.toJson();
    qDebug() << bytes;
    server->broadcast(gameUiInfo);
}

int GameLogic::playerAliveNum(){
    int temp = 0;
    for (QJsonValue alive : playerAlive) {
        if (alive.toBool()) temp++;
    }
    return temp;
}

void GameLogic::receiveJson(ServerWorker *sender, const QJsonObject &json){
    const QString type = json.value(QString("type")).toString();
    if (sender->getPlayerName() == currentPlayer) {
        if (type == "endTurn") endTurn();
        if (type == "playCard") {
            QJsonArray temp = playerHand[currentPlayer].toArray();
            QString card = temp.takeAt(json.value("card").toInt()).toString();
            playerHand[currentPlayer] = temp;
            playerPlayCard(card);
        }
        updateAllUi();
    }
}
