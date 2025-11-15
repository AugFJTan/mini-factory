#ifndef MINI_FACTORY_TILE_H_
#define MINI_FACTORY_TILE_H_

#include "Mini_Factory.h"
#include "TileID.h"

typedef enum TileType {
	EMPTY,
	BELT,
	BUILDING
} TileType;

class Tile {
public:
	Tile();
	Tile(TileType type, TileID id);
	TileType getType();
	TileID getID();

private:
	TileType m_type;
	TileID m_id;
};

#endif
