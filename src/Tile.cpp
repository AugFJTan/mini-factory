#include "Tile.h"

Tile::Tile(TileType type, SDL_Point pos) : m_type(type), m_pos(pos) {
}

TileType Tile::getType() {
	return m_type;
}

SDL_Point Tile::getPos() {
	return m_pos;
}
