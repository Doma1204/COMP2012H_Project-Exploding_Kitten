#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "player.h"
#include "card.h"
#include "vector"
#include "algorithm"

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

class GameLogic
{
public:
    bool server;
    int player_num;
    GameLogic(bool server, int player_num);
    ~GameLogic();
    PLAYER_NUM current_player;
    bool attacked;
    bool skipped;
    bool player_alive[4] = {false};
    vector<CARD_TYPE> deck;
    vector<CARD_TYPE> player_hand[4];
    int players_left;

    void add_to_player_hand(CARD_TYPE card, PLAYER_NUM player);
    CARD_TYPE draw_card();
    void player_play_card(CARD_TYPE card, PLAYER_NUM player);
    void end_turn();
    bool explode(PLAYER_NUM player);


    void send_player_hand(PLAYER_NUM player);
    void send_player_notif(PLAYER_NUM player, NOTIF_TYPE type);
    void get_player_move(PLAYER_NUM player);
    void update_all_ui();

};

#endif // GAMELOGIC_H
