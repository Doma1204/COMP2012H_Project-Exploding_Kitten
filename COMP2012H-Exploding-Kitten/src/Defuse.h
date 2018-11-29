#ifndef DEFUSE_H
#deifne DEFUSE_H

#include "Cards.h"

class Defuse: public Cards{
	public:
		Defuse(Player player){	};
		virtual void action() const override{
			player_hand[player].erase(player_hand[player].begin()+defuse_loc);
      		send_player_notif(player,HAS_DEFUSE);
		}
};

#endif

