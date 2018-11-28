#include "gamelogic.h"
GameLogic::GameLogic(bool server, int player_num):
    server(server),
    player_num(player_num),
    current_player(PLAYER_1),
    attacked(false),
    skipped(false),
    players_left(player_num)
{
    #define X(a,b) \
        for (int i=0;i<(b);i++) { \
            deck.push_back(a); \
        }
    INITIAL_DECK
    #undef X

    random_shuffle ( deck.begin(), deck.end() );
    for (int i=0;i<player_num;i++) {
        player_alive[i]=true;
        for (int j=0;j<INITIAL_HAND_SIZE;j++) {
            add_to_player_hand(draw_card(),(PLAYER_NUM) i);
        }
        send_player_hand((PLAYER_NUM)i);
    }


    for (int i=1;i<player_num;i++) {
        deck.push_back(EXPLODING_KITTEN);
    }
    random_shuffle ( deck.begin(), deck.end() );

}

CARD_TYPE GameLogic::draw_card() {
    CARD_TYPE temp = deck.back();
    deck.pop_back();
    return temp;
}

void GameLogic::add_to_player_hand(CARD_TYPE card, PLAYER_NUM player){
    if (card == EXPLODING_KITTEN) {

        //TODO: let player choose:
        if (explode(player)) {
           deck.insert(deck.begin()+rand()%deck.size(),EXPLODING_KITTEN);
        }
    } else {
        player_hand[player].push_back(card);
    }
}

bool GameLogic::explode(PLAYER_NUM player) {
    send_player_notif(player,EXPLODE);
    bool has_defuse = false;
    int defuse_loc = 0;
    for (int i =0;i<player_hand[player].size();i++) {
        if (player_hand[player].at(i) == DEFUSE) {
            defuse_loc = i;has_defuse = true;
        }
    }
    if (has_defuse) {
        player_hand[player].erase(player_hand[player].begin()+defuse_loc);
        send_player_notif(player,HAS_DEFUSE);
        return true;
    }else {
//TODO:
    send_player_notif(player,LOSE);
    players_left--;
        return false;
    }

}

void GameLogic::send_player_hand(PLAYER_NUM player){
    //TODO:: SEND TO PLAYER
}

void GameLogic::send_player_notif(PLAYER_NUM player,NOTIF_TYPE type){
    switch(type) {
        case SEE_THE_FUT:
            break;
        case LOSE:
            break;
        case WIN:
            break;
        case HAS_DEFUSE:
            break;
    }
    //TODO:: SEND TO PLAYER
}


void GameLogic::player_play_card(CARD_TYPE card, PLAYER_NUM player) {
    switch(card) {
        case ATTACK:
            attacked = true;
            end_turn();
            break;
        case SKIP:
            skipped = true;
            end_turn();
            break;
        case SEE_THE_FUTURE:
            send_player_notif(player,SEE_THE_FUT);
            break;
        case SHUFFLE:
            random_shuffle(deck.begin(), deck.end());
            break;
    }
}

void GameLogic::end_turn(){
    if (skipped) { skipped = false; }
    else if (attacked) {attacked = false;}
    else {
        add_to_player_hand(draw_card(),current_player);
    }
    //TODO: END GAME
    while (!player_alive[current_player]) {
        current_player = (PLAYER_NUM) ((current_player + 1) %player_num);
    }
    if (players_left == 1) {
        send_player_notif(current_player,WIN);
    }
}
