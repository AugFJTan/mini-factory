#ifndef MINI_FACTORY_BELT_H_
#define MINI_FACTORY_BELT_H_

#include "AnimatedTile.h"

class Belt : public AnimatedTile {
public:
	Belt(SDL_Point pos, AnimationID animation_id);
};

#endif
