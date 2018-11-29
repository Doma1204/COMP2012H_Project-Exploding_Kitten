#ifndef SHUFFLE_H
#deifne SHUFFLE_H

#include "Cards.h"

class Shuffle: public Cards{
	public:
		Shuffle(Player player){	};
		virtual void action() const override{
			random_shuffle(deck.begin(), deck.end());
		}
};

#endif

