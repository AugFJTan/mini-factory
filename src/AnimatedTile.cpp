#include "AnimatedTile.h"

AnimatedTile::AnimatedTile(TileType type, AnimationID animation_id) :
	Tile(type), m_animation_id(animation_id) {
}

AnimationID AnimatedTile::getAnimationID() {
	return m_animation_id;
}
