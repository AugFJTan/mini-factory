#ifndef MINI_FACTORY_ITEM_PATH_H_
#define MINI_FACTORY_ITEM_PATH_H_

#include "Mini_Factory.h"
#include "Pathfinding.h"

typedef enum BeltLane {
	LANE_A_UP,
	LANE_A_DOWN,
	LANE_A_LEFT,
	LANE_A_RIGHT,
	LANE_A_UP_LEFT,
	LANE_A_UP_RIGHT,
	LANE_A_DOWN_LEFT,
	LANE_A_DOWN_RIGHT,
	LANE_A_LEFT_UP,
	LANE_A_LEFT_DOWN,
	LANE_A_RIGHT_UP,
	LANE_A_RIGHT_DOWN,
	LANE_B_UP,
	LANE_B_DOWN,
	LANE_B_LEFT,
	LANE_B_RIGHT,
	LANE_B_UP_LEFT,
	LANE_B_UP_RIGHT,
	LANE_B_DOWN_LEFT,
	LANE_B_DOWN_RIGHT,
	LANE_B_LEFT_UP,
	LANE_B_LEFT_DOWN,
	LANE_B_RIGHT_UP,
	LANE_B_RIGHT_DOWN,
} BeltPath;

class ItemPath {
public:
	ItemPath();
	void update(std::vector<uPtr<AnimatedTile>>& map, std::vector<std::vector<SDL_Point>>& belt_lane_offsets, Node* path);
	void drawLaneA(SDL_Renderer* renderer);
	void drawLaneB(SDL_Renderer* renderer);
	void drawItemLaneA(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect *item_frame, int distance);
	void drawItemLaneB(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect *item_frame, int distance);
	int getTotalDistance();

private:
	std::vector<SDL_Point> m_lane_a;
	std::vector<SDL_Point> m_lane_b;
	void addPoints(bool first, bool lane_a, Node* node, int length, std::vector<SDL_Point>& offsets);
	void drawItem(std::vector<SDL_Point>& lane, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect *item_frame, int distance);
	void drawLane(std::vector<SDL_Point>& lane, SDL_Renderer* renderer);
};

#endif
