#ifndef MINI_FACTORY_ANIMATED_TILE_H_
#define MINI_FACTORY_ANIMATED_TILE_H_

#include "Tile.h"
#include "TileID.h"

class AnimatedTile : public Tile {
public:
	AnimatedTile(TileType type, TileID animation_id);
	TileID getAnimationID();

private:
	TileID m_animation_id;
};

#endif
