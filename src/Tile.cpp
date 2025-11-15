#include "Tile.h"

Tile::Tile() : m_type(EMPTY), m_id(BELT_UP) {
}

Tile::Tile(TileType type, TileID id) : m_type(type), m_id(id) {
}

TileType Tile::getType() {
	return m_type;
}

TileID Tile::getID() {
	return m_id;
}
