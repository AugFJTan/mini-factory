#include "AnimatedTile.h"

AnimatedTile::AnimatedTile(TileType type, TileID animation_id) :
	Tile(type), m_animation_id(animation_id) {
}

TileID AnimatedTile::getAnimationID() {
	return m_animation_id;
}
