#include "gamelogic.h"
#include <QJsonDocument>
#include <QMessageBox>
int myrand(int i) {return rand()% i;}
GameLogic::GameLogic(Server* ser) :
    server(ser),
    attacked(NONE),
    skipped(false),
    gameEnded(false),
    seeTheFutureFlag(false),
    drewExplodingKitten(false),
    explodingPlayer(""),
    successfulDefuse(false)
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

    random_shuffle( deck.begin(), deck.end(),myrand);

    for (QString player: playerHand.keys()) {
        for (int j=0;j<INITIAL_HAND_SIZE;j++) {
            addToPlayerHand(drawCard(),player);
        }
        addToPlayerHand(DEFUSE,player);
    }

    for (int i=1;i<playerAliveNum();i++) {
        deck.push_back(EXPLODING_KITTEN);
    }
    currentPlayer = playerAlive.begin().key();
    prevMove = currentPlayer + "'s turn to move.";
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
        qDebug()<< "add to hand";
        //TODO: let player choose:
        drewExplodingKitten = true;
        explodingPlayer = playerName;
        if (defuse(playerName)) {
            qDebug()<< "add exploding";
            prevMove = playerName + " drew an Exploding Kitten but successfully defused it!";
            successfulDefuse = true;
            if (!deck.size()) deck.push_back(EXPLODING_KITTEN);
            else deck.insert(rand()%deck.size(),EXPLODING_KITTEN);
            qDebug()<< "finished adding";
        } else {
            successfulDefuse = false;
            prevMove = playerName + " drew an Exploding Kitten and exploded!";
            playerAlive[playerName] = QJsonValue(false);
        }
    } else {
        QJsonArray temp = playerHand[playerName].toArray();
        temp.push_back(cardName[card]);
        playerHand[playerName] = temp;
    }
}

bool GameLogic::defuse(QString playerName) {
    qDebug() << "enter defuse";
    bool has_defuse = false;
    QJsonArray temp = playerHand[playerName].toArray();
    for (int i = 0; i < temp.count();i++) {
        if (temp.at(i).toString() == "DEFUSE") {
            temp.removeAt(i);
            has_defuse = true;
            break;
        }
    }
    playerHand[playerName] = temp;
    return has_defuse;
}


void GameLogic::playerPlayCard(QString card) {
    if (card == "SEE_THE_FUTURE") {
        prevMove = currentPlayer + " played see the future and viewed the top three cards of the deck.";
        seeTheFutureFlag = true;
    }else if (card == "ATTACK") {
        if (attacked == ATTACKED) {
            attacked = ATTACK_SKIP;
        }else {
            attacked = ATTACKER;
        }
        endTurn();
    }else if (card == "SKIP") {
        skipped = true;
        endTurn();
    }else if (card == "SHUFFLE") {
        prevMove = currentPlayer + " shuffled the deck.";
        random_shuffle( deck.begin(), deck.end(),myrand );
    } else if (card == "STEAL") {
        stealCard(currentPlayer);
    }
}

void GameLogic::stealCard(QString stealer) {
    QJsonObject::iterator it = playerAlive.find(currentPlayer);
    do {
        if (++it == playerAlive.end()) it = playerAlive.begin();
    }while (it != playerAlive.find(currentPlayer) && !(it.value().toBool()));
    QJsonArray stealerHand = playerHand[stealer].toArray();
    QJsonArray targetHand = playerHand[it.key()].toArray();
    if (!targetHand.size()) {
        prevMove = stealer + " tried to steal from " + it.key() + " but his hand was empty!";
        return;
    }
    prevMove = stealer + " stole a card from " + it.key() + "!";
    stealerHand.append(targetHand.takeAt(rand()%targetHand.size()));
    playerHand[stealer] = stealerHand;
    playerHand[it.key()] = targetHand;
}

void GameLogic::endTurn(){
    if (skipped) {
        skipped = false;
        if (attacked == ATTACKED) {
            attacked = NONE;
        }
        prevMove = currentPlayer + " played a skip and passed his turn.";
    }
    else {
        switch(attacked) {
            case ATTACKER:
                attacked = ATTACKED;
                prevMove = currentPlayer + " attacked and passed his turn.";
                break;
            case ATTACKED:
                prevMove = currentPlayer + " drew a card.";
                attacked = NONE;
                addToPlayerHand(drawCard(),currentPlayer);
                break;
            case ATTACK_SKIP:
                prevMove = currentPlayer + " played an Attack.";
                attacked = NONE;
                break;
            case NONE:
                prevMove = currentPlayer + " drew a card and passed.";
                addToPlayerHand(drawCard(),currentPlayer);
                break;
        }
    }
    if (playerAliveNum() == 1) return;
    if (attacked == ATTACKED) return;
    QJsonObject::iterator it = playerAlive.find(currentPlayer);
    do {
        if (++it == playerAlive.end()) it = playerAlive.begin();
    }while (it != playerAlive.find(currentPlayer) && !(it.value().toBool()));
    currentPlayer = it.key();
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
    QJsonArray seeTheFutureArr;
    if (deck.size()>=3) {
        seeTheFutureArr.append(cardName[*(deck.end()-1)]);
        seeTheFutureArr.append(cardName[*(deck.end()-2)]);
        seeTheFutureArr.append(cardName[*(deck.end()-3)]);
    } else if (deck.size() == 2) {
        seeTheFutureArr.append(cardName[*(deck.end()-1)]);
        seeTheFutureArr.append(cardName[*(deck.end()-2)]);
        seeTheFutureArr.append("No Card");
    } else if (deck.size() == 1) {
        seeTheFutureArr.append(cardName[*(deck.end()-1)]);
        seeTheFutureArr.append("No Card");
        seeTheFutureArr.append("No Card");
    } else {
        seeTheFutureArr.append("No Card");
        seeTheFutureArr.append("No Card");
        seeTheFutureArr.append("No Card");
    }
    gameUiInfo["seeTheFuture"] = seeTheFutureArr;
    gameUiInfo["seeTheFutureFlag"] = QJsonValue(seeTheFutureFlag);
    seeTheFutureFlag = false;
    gameUiInfo["prevMove"] = prevMove;
    gameUiInfo["drewExplodingKitten"] = QJsonValue(drewExplodingKitten);
    drewExplodingKitten = false;
    gameUiInfo["explodingPlayer"] = explodingPlayer;
    gameUiInfo["successfulDefuse"] = successfulDefuse;
    if (playerAliveNum() == 1) {
        QJsonObject::iterator it = playerAlive.find(currentPlayer);
        do {
            if (++it == playerAlive.end()) it = playerAlive.begin();
        }while (it != playerAlive.find(currentPlayer) && !(it.value().toBool()));
        gameUiInfo["winner"] =it.key();
        gameEnded = true;
    }
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
