#ifndef MINI_FACTORY_PATHFINDING_H_
#define MINI_FACTORY_PATHFINDING_H_

#include "Mini_Factory.h"
#include "AnimatedTile.h"
#include <vector>
#include <queue>

struct Node {
	SDL_Point pos;
	TileType type;
	bool visited;
	Node* previous;
	Node* next;
};

class Path {
public:
	Path();
	void setTileType(SDL_Point pos, TileType type);
	void traverse(std::vector<uPtr<AnimatedTile>>& map);
	std::vector<Node*> getPaths();

private:
	Node m_nodes[MAP_WIDTH * MAP_HEIGHT];
};

#endif
