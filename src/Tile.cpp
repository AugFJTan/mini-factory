#include "Tile.h"

Tile::Tile(TileType type) : m_type(type) {
}

TileType Tile::getType() {
	return m_type;
}
