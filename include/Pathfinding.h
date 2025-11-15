#ifndef MINI_FACTORY_PATHFINDING_H_
#define MINI_FACTORY_PATHFINDING_H_

#include "Mini_Factory.h"
#include "Tile.h"
#include <vector>
#include <queue>

struct Node {
	SDL_Point pos;
	bool visited;
	Node* previous;
	Node* next;
};

class Path {
public:
	Path();
	void traverse(Tile map[]);
	std::vector<Node*> getPaths();

private:
	Node m_nodes[MAP_WIDTH * MAP_HEIGHT];
	void traversePath(Tile map[], Node* start);
};

#endif
