#include "AnimatedTile.h"

AnimatedTile::AnimatedTile(TileType type, SDL_Point pos, AnimationID animation_id) :
	Tile(type, pos), m_animation_id(animation_id) {
}

AnimationID AnimatedTile::getAnimationID() {
	return m_animation_id;
}
