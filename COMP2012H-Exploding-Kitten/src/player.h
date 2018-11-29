#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "vector"
using namespace std;


enum PLAYER_NUM{
    PLAYER_1,
    PLAYER_2,
    PLAYER_3,
    PLAYER_4
};


class Player
{
public:

    Player(PLAYER_NUM player);
    ~Player();
//    vector<CARD_TYPE> hand;
    const PLAYER_NUM player_num;

};

#endif // PLAYER_H
