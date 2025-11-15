#include "ItemPath.h"
#include "Util.h"

ItemPath::ItemPath() {
}

void ItemPath::addPoints(bool first, bool lane_a, Node* node, int length, std::vector<SDL_Point>& offsets) {
	int start_idx = first ? 0 : 1;
	for (int i = start_idx; i < offsets.size(); i++) {
		SDL_Point offset = offsets[i];
		SDL_Point point = {node->pos.x * length + offset.x, node->pos.y * length + offset.y};
		if (lane_a)
			m_lane_a.push_back(point);
		else
			m_lane_b.push_back(point);
	}
}

void ItemPath::update(Tile map[], std::vector<std::vector<SDL_Point>>& belt_lane_offsets, Node* path) {
	Node* current = path;
	Tile start_tile = map[to_index(current->pos)];
	TileID previous_tile_id = start_tile.getID();

	int length = 32;

	bool first = true;
	bool lane_a = true, lane_b = false;

	while(current != nullptr) {
		Tile current_tile = map[to_index(current->pos)];
		TileID current_tile_id = current_tile.getID();
		switch(current_tile_id) {
			case BELT_UP:
				addPoints(first, lane_a, current, length, belt_lane_offsets[LANE_A_UP]);
				addPoints(first, lane_b, current, length, belt_lane_offsets[LANE_B_UP]);
				break;
			case BELT_DOWN:
				addPoints(first, lane_a, current, length, belt_lane_offsets[LANE_A_DOWN]);
				addPoints(first, lane_b, current, length, belt_lane_offsets[LANE_B_DOWN]);
				break;
			case BELT_LEFT:
				addPoints(first, lane_a, current, length, belt_lane_offsets[LANE_A_LEFT]);
				addPoints(first, lane_b, current, length, belt_lane_offsets[LANE_B_LEFT]);
				break;
			case BELT_RIGHT:
				addPoints(first, lane_a, current, length, belt_lane_offsets[LANE_A_RIGHT]);
				addPoints(first, lane_b, current, length, belt_lane_offsets[LANE_B_RIGHT]);
				break;
			case BELT_UP_LEFT:
				addPoints(first, lane_a, current, length, belt_lane_offsets[LANE_A_UP_LEFT]);
				addPoints(first, lane_b, current, length, belt_lane_offsets[LANE_B_UP_LEFT]);
				break;
			case BELT_UP_RIGHT:
				addPoints(first, lane_a, current, length, belt_lane_offsets[LANE_A_UP_RIGHT]);
				addPoints(first, lane_b, current, length, belt_lane_offsets[LANE_B_UP_RIGHT]);
				break;
			case BELT_DOWN_LEFT:
				addPoints(first, lane_a, current, length, belt_lane_offsets[LANE_A_DOWN_LEFT]);
				addPoints(first, lane_b, current, length, belt_lane_offsets[LANE_B_DOWN_LEFT]);
				break;
			case BELT_DOWN_RIGHT:
				addPoints(first, lane_a, current, length, belt_lane_offsets[LANE_A_DOWN_RIGHT]);
				addPoints(first, lane_b, current, length, belt_lane_offsets[LANE_B_DOWN_RIGHT]);
				break;
			case BELT_LEFT_UP:
				addPoints(first, lane_a, current, length, belt_lane_offsets[LANE_A_LEFT_UP]);
				addPoints(first, lane_b, current, length, belt_lane_offsets[LANE_B_LEFT_UP]);
				break;
			case BELT_LEFT_DOWN:
				addPoints(first, lane_a, current, length, belt_lane_offsets[LANE_A_LEFT_DOWN]);
				addPoints(first, lane_b, current, length, belt_lane_offsets[LANE_B_LEFT_DOWN]);
				break;
			case BELT_RIGHT_UP:
				addPoints(first, lane_a, current, length, belt_lane_offsets[LANE_A_RIGHT_UP]);
				addPoints(first, lane_b, current, length, belt_lane_offsets[LANE_B_RIGHT_UP]);
				break;
			case BELT_RIGHT_DOWN:
				addPoints(first, lane_a, current, length, belt_lane_offsets[LANE_A_RIGHT_DOWN]);
				addPoints(first, lane_b, current, length, belt_lane_offsets[LANE_B_RIGHT_DOWN]);
				break;
			default:
				break;
		}
		first = false;
		previous_tile_id = current_tile_id;
		current = current->next;
	}
}

int ItemPath::getTotalDistance() {
	int total_distance = 0;
	for (int i = 0; i < m_lane_a.size()-1; i++) {
		total_distance += calc_distance(m_lane_a[i], m_lane_a[i+1]);
	}
	return total_distance;
}

void ItemPath::drawLane(std::vector<SDL_Point>& lane, SDL_Renderer* renderer) {
	int scale = 2;
	SDL_Point previous = lane[0];
	for (int i = 1; i < lane.size(); i++) {
		SDL_Point current = lane[i];
		SDL_Point start = {previous.x * scale, previous.y * scale};
		SDL_Point end = {current.x * scale, current.y * scale};
		SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
		previous = current;
	}
}

void ItemPath::drawLaneA(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 0xff);
	drawLane(m_lane_a, renderer);
}

void ItemPath::drawLaneB(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0xff, 0xff);
	drawLane(m_lane_b, renderer);
}

void ItemPath::drawItemLaneA(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect *item_rect, int distance) {
	drawItem(m_lane_a, renderer, texture, item_rect, distance);
}

void ItemPath::drawItemLaneB(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect *item_rect, int distance) {
	drawItem(m_lane_b, renderer, texture, item_rect, distance);
}

void ItemPath::drawItem(std::vector<SDL_Point>& lane, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect *item_rect, int distance) {
	int scale = 2;

	int index = -1;
	int offset = 0;
	for (int i = 0; i < lane.size()-1; i++) {
		offset += calc_distance(lane[i], lane[i+1]);
		if (offset >= distance) {
			index = i;
			break;
		}
	}
	if (index == -1)
		return;

	SDL_Point current = lane[index];
	SDL_Point next = lane[index+1];

	int dx = 0, dy = 0;
	int adjust = calc_distance(current, next) - (offset - distance);
	if (current.y == next.y) {
		if (current.x < next.x)  // right
			dx = adjust;
		else  // left
			dx = -adjust;
	}
	if (current.x == next.x) {
		if (current.y < next.y)  // down
			dy = adjust;
		else  // up
			dy = -adjust;
	}

	SDL_Rect dst;
	dst.x = current.x + dx - (item_rect->w / 2);
	dst.y = current.y + dy - (item_rect->h / 2);
	dst.w = item_rect->w;
	dst.h = item_rect->h;

	// printf("(%i, %i)\n", dst.x, dst.y);

	dst.x *= scale;
	dst.y *= scale;
	dst.w *= scale;
	dst.h *= scale;

	SDL_RenderCopy(renderer, texture, item_rect, &dst);
}
