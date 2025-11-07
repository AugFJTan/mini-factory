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
	Tile(TileType type, SDL_FPoint pos);
	TileType getType();
	SDL_FPoint getPos();

private:
	TileType m_type;
	SDL_FPoint m_pos;
};

#endif
