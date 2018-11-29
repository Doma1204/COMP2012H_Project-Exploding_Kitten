#include "gamelogic.h"

GameLogic::GameLogic(Server* ser) :
    attacked(false),
    skipped(false),
    server(ser)
{
    for (ServerWorker *worker : server->clients) {
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

    random_shuffle ( deck.begin(), deck.end() );

    for (QString player: playerHand.keys()) {
        for (int j=0;j<INITIAL_HAND_SIZE;j++) {
            addToPlayerHand(drawCard(),player);
        }
    }

    for (int i=1;i<playersLeft;i++) {
        deck.push_back(EXPLODING_KITTEN);
    }
    random_shuffle ( deck.begin(), deck.end() );
    connect(server, &Server::receiveJson, this, &GameLogic::receiveJson);
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
        playerHand[playerName].toArray().push_back(cardName[card]);
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
        //END GAME
    } else {
        QJsonObject::iterator it = playerAlive.find(currentPlayer);
        do {
            if (++it == playerAlive.end()) it = playerAlive.begin();
        }while (it != playerAlive.find(currentPlayer) && !(it.value().toBool()));
        currentPlayer = it.key();
    }
}

void GameLogic::updateAllUi(){
    QJsonObject gameUiInfo;

    gameUiInfo["type"] = "updateUi";
    gameUiInfo["deckSize"] = (int) deck.size();
    gameUiInfo["playerHand"] = playerHand;
    gameUiInfo["playerAlive"] = playerAlive;
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
            playerPlayCard(json.value("card").toString());
        }
    }
}
