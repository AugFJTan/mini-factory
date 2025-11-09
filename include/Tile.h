#ifndef MINI_FACTORY_TILE_H_
#define MINI_FACTORY_TILE_H_

#include "Mini_Factory.h"

typedef enum TileType {
	EMPTY,
	BELT,
	BUILDING
} TileType;

class Tile {
public:
	Tile(TileType type, SDL_Point pos);
	TileType getType();
	SDL_Point getPos();

private:
	TileType m_type;
	SDL_Point m_pos;
};

#endif
