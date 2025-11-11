#ifndef MINI_FACTORY_UTIL_H_
#define MINI_FACTORY_UTIL_H_

#include "Mini_Factory.h"
#include "AnimationID.h"

int to_index(SDL_Point pos);
bool valid_pos(SDL_Point pos);
bool belt_forward_connected(SDL_Point pos_a, AnimationID anim_a, SDL_Point pos_b, AnimationID anim_b);

#endif
