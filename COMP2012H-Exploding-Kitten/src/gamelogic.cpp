#include "gamelogic.h"
#include <QJsonDocument>
#include <QMessageBox>


/*
 *  GameLogic::GameLogic(Server* ser)
 *  @funct:  constructor of GameLogic, initializes starting conditions of the game
 *  @param:  ser: pointer to the server object
 *  @return: N/A
 */
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
        playerAlive[worker->getPlayerName()] = QJsonValue(true); //Initializing playerAlive
        QJsonArray hand;
        playerHand[worker->getPlayerName()] = hand; //Initializing playerHand
    }
    //Macro for initializing the deck
    #define INIT_DECK(a,b) \
        for (int i=0;i<(b);i++) { \
            deck.push_back(a); \
        }
    INITIAL_DECK(INIT_DECK)
    #undef INIT_DECK

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(deck.begin(), deck.end(), std::default_random_engine(seed)); //Shuffling the deck

    for (QString player: playerHand.keys()) {
        for (int j=0;j<INITIAL_HAND_SIZE;j++) {
            addToPlayerHand(drawCard(),player); //Drawing the cards for each player for their initial hand
        }
        addToPlayerHand(DEFUSE,player); //Guarenteed Defuse at the start
    }

    for (int i=1;i<playerAliveNum();i++) {
        deck.push_back(EXPLODING_KITTEN); //adding Exploding Kitten cards to the deck
    }
    currentPlayer = playerAlive.begin().key();      //initializing currentPlayer
    prevMove = currentPlayer + "'s turn to move.";  //initializing prevMove for GUI purposes
    shuffle(deck.begin(), deck.end(), std::default_random_engine(seed)); //shuffling the deck (after adding the Exploding Kittens)
    connect(server, &Server::receiveJson, this, &GameLogic::receiveJson);
    updateAllUi(); //sending the initial UI information
}


/*
 *  GameLogic::CARD_TYPE GameLogic::drawCard()
 *  @funct:  draws and returns the top card of the deck
 *  @param:  N/A
 *  @return: the CARD_TYPE enum corresponding to the card drawn
 */
GameLogic::CARD_TYPE GameLogic::drawCard() {
    CARD_TYPE temp = deck.back();
    deck.pop_back();
    return temp;
}


/*
 *  void GameLogic::addToPlayerHand(CARD_TYPE card, QString playerName)
 *  @funct:  adds the desired card to the target player's hand
 *  @param:  card: the desired card, playerName: target player
 *  @return: N/A
 */
void GameLogic::addToPlayerHand(CARD_TYPE card, QString playerName){
    if (card == EXPLODING_KITTEN) {
        drewExplodingKitten = true;
        explodingPlayer = playerName;
        if (defuse(playerName)) {
            //player successfully defuses the Exploding Kitten
            prevMove = playerName + " drew an Exploding Kitten but successfully defused it!";
            successfulDefuse = true;
            if (!deck.size()) deck.push_back(EXPLODING_KITTEN);
            else deck.insert(rand()%deck.size(),EXPLODING_KITTEN);
        } else {
            //player does not defuse the Exploding Kitten (they lose)
            successfulDefuse = false;
            prevMove = playerName + " drew an Exploding Kitten and exploded!";
            playerAlive[playerName] = QJsonValue(false);
        }
    } else {
        //otherwise just add the card to their hand
        QJsonArray temp = playerHand[playerName].toArray();
        temp.push_back(cardName[card]);
        playerHand[playerName] = temp;
    }
}

/*
 *  bool GameLogic::defuse(QString playerName)
 *  @funct:  checks the target player's hand for Defuses, if they have one, play it
 *  @param:  playerName: target player
 *  @return: true: successful defuse, false: unsuccessful defuse
 */
bool GameLogic::defuse(QString playerName) {
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

/*
 *  void GameLogic::playerPlayCard(QString card)
 *  @funct:  handles the card effects after the current player plays it
 *  @param:  card: name of the card played
 *  @return: N/A
 */
void GameLogic::playerPlayCard(QString card) {
    if (card == "SEE_THE_FUTURE") {
        prevMove = currentPlayer + " played See The Future and viewed the top three cards of the deck.";
        seeTheFutureFlag = true;
    }else if (card == "ATTACK") {
        qDebug()<<"ENTERING ATTACK"<<attacked;
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
        prevMove = currentPlayer + " played Shuffle and shuffled the deck.";
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));
    } else if (card == "STEAL") {
        stealCard(currentPlayer);
    }
    prevCard = card;
}

/*
 *  void GameLogic::stealCard(QString stealer)
 *  @funct:  handles the Steal card interactions, steal from the next player
 *  @param:  stealer: the player who played the Steal card
 *  @return: N/A
 */
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

/*
 *  void GameLogic::endTurn()
 *  @funct:  ends the current player's turn, drawing the card (if applicable), and passing the turn to the next player
 *  @param:  N/A
 *  @return: N/A
 */
void GameLogic::endTurn(){
    if (skipped) {
        skipped = false;
        if (attacked == ATTACKED) {
            attacked = NONE;
        }else {
            setNextPlayer();
        }
        prevMove = currentPlayer + " played a skip and passed his turn.";

    }
    else {
        switch(attacked) {
            case ATTACKER:
                attacked = ATTACKED;
                prevMove = currentPlayer + " attacked and passed his turn.";
                setNextPlayer();
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
                setNextPlayer();
                break;
        }
    }

}

/*
 *  void GameLogic::setNextPlayer()
 *  @funct:  finds the next player still alive and sets them to be the currentPlayer
 *  @param:  N/A
 *  @return: N/A
 */
void GameLogic::setNextPlayer() {
    if (playerAliveNum() == 1) return;
    QJsonObject::iterator it = playerAlive.find(currentPlayer);
    do {
        if (++it == playerAlive.end()) it = playerAlive.begin();
    }while (it != playerAlive.find(currentPlayer) && !(it.value().toBool()));
    currentPlayer = it.key();
}

/*
 *  void GameLogic::updateAllUi()
 *  @funct:  broadcasts a QJson Object of all of the UI information to the clients
 *  @param:  N/A
 *  @return: N/A
 */
void GameLogic::updateAllUi(){
    if (gameEnded) return;
    QJsonObject gameUiInfo;
    gameUiInfo["type"] = "updateUi";
    gameUiInfo["deckSize"] = (int) deck.size();
    gameUiInfo["playerHand"] = playerHand;
    gameUiInfo["playerAlive"] = playerAlive;
    gameUiInfo["currentPlayer"] = currentPlayer;
    gameUiInfo["prevMove"] = prevMove;
    gameUiInfo["prevCard"] = prevCard;
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

/*
 *  int GameLogic::playerAliveNum()
 *  @funct:  counts the number of players still alive
 *  @param:  N/A
 *  @return: int: number of alive players
 */
int GameLogic::playerAliveNum(){
    int temp = 0;
    for (QJsonValue alive : playerAlive) {
        if (alive.toBool()) temp++;
    }
    return temp;
}

/*
 *  void GameLogic::receiveJson(ServerWorker *sender, const QJsonObject &json)
 *  @funct:  recieve handler for when the server receives the player's move choices from the clients
 *  @param:  sender: the client who sent the information, json: the QJson object containing the information
 *  @return: N/A
 */
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
