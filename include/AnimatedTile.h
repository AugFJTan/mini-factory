#ifndef MINI_FACTORY_ANIMATED_TILE_H_
#define MINI_FACTORY_ANIMATED_TILE_H_

#include "Tile.h"
#include "AnimationID.h"

class AnimatedTile : public Tile {
public:
	AnimatedTile(TileType type, AnimationID animation_id);
	virtual ~AnimatedTile() = default;
	AnimationID getAnimationID();

private:
	AnimationID m_animation_id;
};

#endif
