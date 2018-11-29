#ifndef SKIP_H
#deifne SKIP_H

#include "Cards.h"
#include "gamelogic.h"

class Skip: public Cards{
	public:
		Skip(Player player){	};
		virtual void action() const override{
			 send_player_notif(player,SEE_THE_FUT);
			 current_player = (PLAYER_NUM) ((current_player + 1) %player_num);
		}
};

#endif

