#ifndef EXPLODE_H
#define EXPLODE_H

#include "Cards.h"

class Explode: public Cards{
	public:
		Explode(Player player){	};
		virtual void action() const override{
			send_player_notif(player,LOSE);
   			//players_left--;
		}
};

#endif

