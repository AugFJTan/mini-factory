#include "Util.h"

int to_index(SDL_Point pos) {
	return pos.y * MAP_WIDTH + pos.x;
}

bool valid_pos(SDL_Point pos) {
	return (pos.x >= 0 && pos.x <= MAP_WIDTH-1) &&
		(pos.y >= 0 && pos.y <= MAP_HEIGHT-1);
}

int calc_distance(SDL_Point a, SDL_Point b) {
	return abs(a.x - b.x) + abs(a.y - b.y);
}

bool belt_forward_connected(SDL_Point pos_a, TileID tile_a, SDL_Point pos_b, TileID tile_b) {
	bool result = false;
	switch(tile_a) {
		case BELT_UP:
		case BELT_LEFT_UP:
		case BELT_RIGHT_UP:
			result = (pos_b.x == pos_a.x && pos_b.y == pos_a.y-1) &&
				(tile_b == BELT_UP || tile_b == BELT_UP_LEFT || tile_b == BELT_UP_RIGHT);
			break;
		case BELT_DOWN:
		case BELT_LEFT_DOWN:
		case BELT_RIGHT_DOWN:
			result = (pos_b.x == pos_a.x && pos_b.y == pos_a.y+1) &&
				(tile_b == BELT_DOWN || tile_b == BELT_DOWN_LEFT || tile_b == BELT_DOWN_RIGHT);
			break;
		case BELT_LEFT:
		case BELT_UP_LEFT:
		case BELT_DOWN_LEFT:
			result = (pos_b.x == pos_a.x-1 && pos_b.y == pos_a.y) &&
				(tile_b == BELT_LEFT || tile_b == BELT_LEFT_UP || tile_b == BELT_LEFT_DOWN);
			break;
		case BELT_RIGHT:
		case BELT_UP_RIGHT:
		case BELT_DOWN_RIGHT:
			result = (pos_b.x == pos_a.x+1 && pos_b.y == pos_a.y) &&
				(tile_b == BELT_RIGHT || tile_b == BELT_RIGHT_UP || tile_b == BELT_RIGHT_DOWN);
			break;
		default:
			break;
	}
	return result;
}
