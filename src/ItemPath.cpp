#include "ItemPath.h"
#include "Util.h"

#define BELT_LENGTH 32
#define BELT_TOP_OFFSET 10
#define BELT_BOTTOM_OFFSET 30
#define BELT_LEFT_OFFSET 10
#define BELT_RIGHT_OFFSET 20

ItemPath::ItemPath() {
}

void ItemPath::addPoints(bool first, bool lane_a, Node* node, int length, std::vector<SDL_Point>& offsets) {
	int start_idx = first ? 0 : 1;
	for (int i = 0; i < offsets.size(); i++) {
		SDL_Point offset = offsets[i];
		SDL_Point point = {node->pos.x * length + offset.x, node->pos.y * length + offset.y};
		if (lane_a)
			m_lane_a.push_back(point);
		else
			m_lane_b.push_back(point);
	}
}

void ItemPath::update(std::vector<uPtr<AnimatedTile>>& map, std::vector<std::vector<SDL_Point>>& belt_lane_offsets, Node* path) {
	Node* current = path;
	AnimationID start_anim = map[to_index(current->pos)]->getAnimationID();
	AnimationID previous_anim = start_anim;

	int length = 32;
	/*int top_offset = 10;
	int bottom_offset = 20;
	int left_offset = 10;
	int right_offset = 20;*/

	bool first = true;
	bool lane_a = true, lane_b = false;

	while(current != nullptr) {
		AnimationID current_anim = map[to_index(current->pos)]->getAnimationID();
		switch(current_anim) {
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
		previous_anim = current_anim;
		current = current->next;
	}
}

void ItemPath::drawLaneA(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 0xff);
	int scale = 2;
	SDL_Point previous = m_lane_a[0];
	for (int i = 1; i < m_lane_a.size(); i++) {
		SDL_Point current = m_lane_a[i];
		SDL_Point start = {previous.x * scale, previous.y * scale};
		SDL_Point end = {current.x * scale, current.y * scale};
		SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
		previous = current;
	}
}

void ItemPath::drawLaneB(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0xff, 0xff);
	int scale = 2;
	SDL_Point previous = m_lane_b[0];
	for (int i = 1; i < m_lane_b.size(); i++) {
		SDL_Point current = m_lane_b[i];
		SDL_Point start = {previous.x * scale, previous.y * scale};
		SDL_Point end = {current.x * scale, current.y * scale};
		SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
		previous = current;
	}
}
