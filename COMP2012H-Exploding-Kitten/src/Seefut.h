#ifndef SEEFUT_H
#deifne SEEFUT_H

#include "Cards.h"

class Seefut: public Cards{
	public:
		Seefut(Player player){	};
		virtual void action() const override{
			 send_player_notif(player,SEE_THE_FUT);
			 //peek deck?
		}
};

#endif

