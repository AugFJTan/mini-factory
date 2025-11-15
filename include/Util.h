#ifndef MINI_FACTORY_UTIL_H_
#define MINI_FACTORY_UTIL_H_

#include "Mini_Factory.h"
#include "TileID.h"

int to_index(SDL_Point pos);
bool valid_pos(SDL_Point pos);
int calc_distance(SDL_Point a, SDL_Point b);
bool belt_forward_connected(SDL_Point pos_a, TileID anim_a, SDL_Point pos_b, TileID anim_b);

#endif
