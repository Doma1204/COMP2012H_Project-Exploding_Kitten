#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "player.h"
#include "card.h"
#include "vector"

using namespace std;

class GameLogic
{
public:
    bool server;
    GameLogic(int player_num);
    GameLogic(bool server);
    ~GameLogic();
    Player* player_list[4];
    Player* current_player;
    bool attacked;
    bool skipped;
    vector<CARD_TYPE> deck;
    int players_left;
    void draw_card();
    bool set_current_player(PLAYER_NUM next_player);
    void pass_turn();
    void player_explode();
    void play_card(PLAYER_NUM player, CARD_TYPE card);
    void defuse_bomb();
    void end_turn();
    void see_the_future(PLAYER_NUM player);
    void shuffle();
};

#endif // GAMELOGIC_H
