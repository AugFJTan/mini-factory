#include "Pathfinding.h"
#include "Util.h"

Path::Path() {
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			SDL_Point pos = {x, y};
			m_nodes[to_index(pos)] = {pos, EMPTY, false, nullptr};
		}
	}
}

void Path::setTileType(SDL_Point pos, TileType type) {
	m_nodes[to_index(pos)].type = type;
}

void Path::traverse(std::vector<uPtr<AnimatedTile>>& map) {
	std::queue<Node*> traversable;
	std::queue<Node*> queue;

	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			Node* node = &m_nodes[to_index({x, y})];
			if (node->type == BELT)
				traversable.push(node);
		}
	}

	// TODO Differentiate between different paths
	while(traversable.size() > 0) {
		Node* current = traversable.front();
		traversable.pop();

		if (current->visited)
			continue;

		queue.push(current);

		while(queue.size() > 0) {
			current = queue.front();
			queue.pop();

			current->visited = true;
			AnimationID current_anim = map[to_index(current->pos)]->getAnimationID();

			SDL_Point neighbours[4] = {
				{current->pos.x, current->pos.y-1},
				{current->pos.x, current->pos.y+1},
				{current->pos.x-1, current->pos.y},
				{current->pos.x+1, current->pos.y}
			};

			for (int i = 0; i < 4; i++) {
				SDL_Point adjacent = neighbours[i];
				if (valid_pos(adjacent)) {
					if (m_nodes[to_index(adjacent)].type == BELT) {
						Node* next = &m_nodes[to_index(adjacent)];
						if (next->visited)
							continue;
						AnimationID next_anim = map[to_index(next->pos)]->getAnimationID();
						if (belt_forward_connected(current->pos, current_anim, next->pos, next_anim)) {
							printf("(%i, %i) is forward connected to (%i, %i)\n", current->pos.x, current->pos.y, next->pos.x, next->pos.y);
							current->next = next;
							next->previous = current;
							queue.push(next);
						}
						/*else if (next != start && belt_backward_connected(current->pos, current_anim, next->pos, next_anim)) {
							printf("(%i, %i) is backward connected to (%i, %i)\n", current->pos.x, current->pos.y, next->pos.x, next->pos.y);
							current->previous = next;
							next->next = current;
							queue.push(next);
						}*/
					}
				}
			}
		}
	}
}

std::vector<Node*> Path::getPaths() {
	std::vector<Node*> paths;
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			Node* node = &m_nodes[to_index({x, y})];
			if (node->previous == nullptr && node->next != nullptr)
				paths.push_back(node);
		}
	}
	std::cout << "Found " << paths.size() << " paths" << std::endl;
	return paths;
}
