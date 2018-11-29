#ifndef CARDS_H_
#define CARDS_H_

#include "player.h"
#include "gamelogic.h"

class Cards{
	public:
		const Player player;
		Cards(void):player (player) {	};
		virtual ~Cards(void)  = default;
		virtual void action() = 0;
};

#endif

