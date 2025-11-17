#ifndef MINI_FACTORY_FILE_UTIL_H_
#define MINI_FACTORY_FILE_UTIL_H_

#include "Tile.h"
#include "TileID.h"
#include "Animation.h"
#include "MachineAnimation.h"
#include "Pathfinding.h"
#include "Util.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

void parse_animation_file(std::string filename, TileID start_tile_id, Spritesheet* spritesheet, std::map<std::string, TileID>& tile_lookup,
	sPtr<AnimationFrames>& animation_frames, std::vector<uPtr<Animation>>& animations);

ParticleEffect parse_particle_effect_file(std::string particle_effect_id);

void parse_layout_file(Tile map[], std::map<std::string, TileID>& tile_lookup, Path& path);

void parse_item_lanes_file(std::vector<std::vector<SDL_Point>>& belt_lane_offsets);

#endif
