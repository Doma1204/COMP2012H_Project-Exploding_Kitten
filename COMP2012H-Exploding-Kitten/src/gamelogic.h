#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <algorithm>
#include <random>
#include <chrono>
#include "server.h"
#include <QtDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QVector>
#include <QObject>
using namespace std;

//How many cards each player has at the start (not including the guarenteed defuse)
#define INITIAL_HAND_SIZE 4

//X Macros of the number of each cards in the initial deck (Exploding Kittens are added afterwards)
#define INITIAL_DECK(X) \
    X(DEFUSE,2) \
    X(EXPLODING_KITTEN,0)\
    X(ATTACK,6) \
    X(SKIP,6) \
    X(SEE_THE_FUTURE,5) \
    X(SHUFFLE,4) \
    X(STEAL, 4)

#define CARD_COUNT(A,B) 1+
#define DECK_COUNT(A,B) B+
#define CARD_NAME(A,B) #A,
#define CARD_ENUM(A,B) A,


class GameLogic : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(GameLogic)

    enum CARD_TYPE { INITIAL_DECK(CARD_ENUM) }; //enum to keep track of the types of cards
    enum ATTACK_STATE {NONE, ATTACKED, ATTACKER, ATTACK_SKIP}; //for managing the Attack card interactions

public:
    explicit GameLogic(Server* ser); //GameLogic constructor

    void addToPlayerHand(CARD_TYPE card, QString playerName); //adds the card to the desired player's hand
    CARD_TYPE drawCard(); //removes the top card of the deck and returns it
    void playerPlayCard(QString card); //for handling the card effects after the player plays it
    void endTurn(); //for handling end of turn actions (drawing and passing to the next player)
    bool defuse(QString playerName); //checks to see if the player has a Defuse card, returns true if yes
    void updateAllUi(); //sends GUI information to all of the clients
    int playerAliveNum(); //number of players remaining in the game
    void stealCard(QString stealer); //for handling the Steal card mechanics
    void setNextPlayer(); //setting the next player

private:

    static const int deck_size = INITIAL_DECK(DECK_COUNT) 0;
    const QString cardName[INITIAL_DECK(CARD_COUNT) 0] = {INITIAL_DECK(CARD_NAME)}; //array of the names of the cards
    Server* server; //pointer to the server object
    QString currentPlayer; //the name of the current player
    QString prevMove; //the previous move (to be displayed on the GUI)
    ATTACK_STATE attacked; //for handling the Attack card interactions
    bool skipped; //for handling the Skip card interaction
    bool gameEnded; //prevents the players from playing after the game has ended
    bool seeTheFutureFlag; //flag to display the cards on the GUI
    QVector<CARD_TYPE> deck; //vector that contains the deck of cards
    QJsonObject playerAlive; //key: playerName    value: alive or not (bool)
    QJsonObject playerHand;  //key: playerName    value: QJsonArray of the player's hand
    QString prevCard; //for displaying the center card of the GUI
    bool drewExplodingKitten; //flag for updating GUI
    QString explodingPlayer; //name of player who drew the Exploding Kitten (for updating GUI)
    bool successfulDefuse; //whether or not the defuse was successful (for updating GUI)

public slots:
    void receiveJson(ServerWorker *sender, const QJsonObject &json); //handler for when the server recieves info from clients.
};

#endif // GAMELOGIC_H
