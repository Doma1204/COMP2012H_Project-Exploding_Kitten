#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "player.h"
#include "card.h"
#include "vector"
#include "algorithm"
#include "server.h"
#include <QtDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QVector>
#include <QObject>
using namespace std;

enum NOTIF_TYPE{
    LOSE,
    WIN,
    SEE_THE_FUT,
    EXPLODE,
    HAS_DEFUSE
};

#define INITIAL_HAND_SIZE 3

#define INITIAL_DECK \
    X(DEFUSE,2) \
    X(ATTACK,3) \
    X(SKIP,5) \
    X(SEE_THE_FUTURE,3) \
    X(SHUFFLE,4)

#define X(a,b) b+
static const int deck_size = INITIAL_DECK 0;
#undef X

#define CARDS(X) \
    X(DEFUSE) \
    X(EXPLODING_KITTEN)\
    X(ATTACK)\
    X(SKIP) \
    X(SEE_THE_FUTURE)\
    X(SHUFFLE) \
    X(NOPE)

#define CARD_COUNT(A) 1+
#define CARD_NAME(A) #A,
#define CARD_ENUM(A) A,

enum CARD_TYPE {
  CARDS(CARD_ENUM)
};


class GameLogic : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(GameLogic)
public:
    explicit GameLogic(Server* ser);
    const QString cardName[CARDS(CARD_COUNT) 0] = {CARDS(CARD_NAME)};
    Server* server;
    QString currentPlayer;
    bool attacked;
    bool skipped;
    bool player_alive[4] = {false};
    QVector<CARD_TYPE> deck;
    QVector<QString> playerNames;
    QJsonObject playerAlive;
    QJsonObject playerHand;
    QVector<QString> playerLeft;
    int playersLeft;

    void addToPlayerHand(CARD_TYPE card, QString playerName);
    CARD_TYPE drawCard();
    void playerPlayCard(QString card);
    void endTurn();
    bool defuse(QString playerName);
    void updateAllUi();
    int playerAliveNum();
public slots:
    void receiveJson(ServerWorker *sender, const QJsonObject &json);
};

#endif // GAMELOGIC_H
