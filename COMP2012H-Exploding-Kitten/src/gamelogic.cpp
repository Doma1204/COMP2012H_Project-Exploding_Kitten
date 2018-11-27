#include "gamelogic.h"


GameLogic::GameLogic(bool server) :
    server(server)
{

}

GameLogic::GameLogic(int player_num) :
    players_left(player_num)
{
    player_list[PLAYER_1] = new Player(PLAYER_1);
    player_list[PLAYER_2] = new Player(PLAYER_2);
    if (player_num>2) player_list[PLAYER_3] = new Player(PLAYER_3);
    else player_list[PLAYER_3] = nullptr;
    if (player_num>3) player_list[PLAYER_4] = new Player(PLAYER_4);
    else player_list[PLAYER_4] = nullptr;

    current_player = player_list[PLAYER_1];
}

GameLogic::~GameLogic() {
    for (int i =0; i<4;i++) {
        delete player_list[i];
    }
}

bool GameLogic::set_current_player(PLAYER_NUM next_player) {
    if (!player_list[next_player]) {current_player = player_list[next_player]; return true;}
    return false;
}


void GameLogic::draw_card() {
    if (deck.back() == EXPLODING_KITTEN) {
        bool has_defuse = false;
        for (auto const& card: current_player->hand) {
            if (card == DEFUSE) has_defuse = true;
        }
        if (has_defuse) {
            defuse_bomb();
            //TODO: remove defuse
        }else {
            player_explode();
        }
    }
    current_player->hand.push_back(deck.back());
    deck.pop_back();
}

void GameLogic::defuse_bomb() {
    //TODO::
}

void GameLogic::play_card(PLAYER_NUM player, CARD_TYPE card) {
    //TODO::
    switch(card) {
        case ATTACK:
            attacked = true;
            break;
        case SKIP:
            skipped = true;
        break;
    case SEE_THE_FUTURE:
        see_the_future(player);
        break;
    case SHUFFLE:
        shuffle();
        break;
//    case NOPE:
//        nope();
//        break;
    }

}
void GameLogic::see_the_future(PLAYER_NUM player) {
    //TODO:
}

void GameLogic::shuffle() {

}

void GameLogic::player_explode() {
    PLAYER_NUM curr_player_num = current_player->player_num;
    pass_turn();
    delete player_list[curr_player_num];
    if (--players_left == 1) {
        //TODO:WIN GAME
    }
}

void GameLogic::end_turn(){
if (!skipped) {
    draw_card();
    skipped = false;
}
if (!attacked){
    pass_turn();
    attacked = false;
}
}

void GameLogic::pass_turn() {
    PLAYER_NUM next_player = (PLAYER_NUM)((current_player->player_num+1)%4);
    if (!set_current_player(next_player)) {
        next_player = (PLAYER_NUM)((current_player->player_num+2)%4);
        if (!set_current_player(next_player)) {
            next_player = (PLAYER_NUM)((current_player->player_num+3)%4);
        }
    }

}
