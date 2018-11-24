#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "player.h"
#include "card.h"
#include "vector"

using namespace std;

class GameLogic
{
public:
    GameLogic(int player_num);
    ~GameLogic();
    Player* player_list[4];
    Player* current_player;
    vector<CARD_TYPE> deck;
    int players_left;
    void draw_card();
    bool set_current_player(PLAYER_NUM next_player);
    void pass_turn();
    void player_explode();
    void play_card(PLAYER_NUM player, CARD_TYPE card);
    void defuse_bomb();
};

#endif // GAMELOGIC_H
