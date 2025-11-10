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
	Tile(TileType type);
	virtual ~Tile() = default;
	TileType getType();

private:
	TileType m_type;
};

#endif
