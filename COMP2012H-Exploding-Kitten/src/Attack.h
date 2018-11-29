#ifndef ATTACK_H
#deifne ATTACK_H

#include "Cards.h"
#include "gamelogic.h"

class Attack: public Cards{
	public:
		Attack(Player player){	};
		virtual void action() const override{
			 //forgot the function of attack pair of cards 
		}
};

#endif

