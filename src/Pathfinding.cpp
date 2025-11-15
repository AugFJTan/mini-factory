#include "Pathfinding.h"
#include "Util.h"

Path::Path() {
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			SDL_Point pos = {x, y};
			m_nodes[to_index(pos)] = {pos, false, nullptr};
		}
	}
}

void Path::traverse(Tile map[]) {
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			SDL_Point pos = {x, y};
			if (map[to_index(pos)].getType() != BELT)
				continue;
			Node* node = &m_nodes[to_index(pos)];
			if (!node->visited)
				traversePath(map, node);
		}
	}
}

void Path::traversePath(Tile map[], Node* start) {
	std::queue<Node*> queue;

	Node* current = start;
	queue.push(current);

	while(queue.size() > 0) {
		current = queue.front();
		queue.pop();

		current->visited = true;
		Tile current_tile = map[to_index(current->pos)];
		TileID current_tile_id = current_tile.getID();

		SDL_Point neighbours[4] = {
			{current->pos.x, current->pos.y-1},
			{current->pos.x, current->pos.y+1},
			{current->pos.x-1, current->pos.y},
			{current->pos.x+1, current->pos.y}
		};

		for (int i = 0; i < 4; i++) {
			SDL_Point adjacent = neighbours[i];
			if (valid_pos(adjacent)) {
				if (map[to_index(adjacent)].getType() == BELT) {
					Node* next = &m_nodes[to_index(adjacent)];
					if (next == start)  // Prevent loop
						continue;
					Tile next_tile = map[to_index(next->pos)];
					TileID next_tile_id = next_tile.getID();
					if (belt_forward_connected(current->pos, current_tile_id, next->pos, next_tile_id)) {
						printf("(%i, %i) is forward connected to (%i, %i)\n", current->pos.x, current->pos.y, next->pos.x, next->pos.y);
						current->next = next;
						next->previous = current;
						queue.push(next);
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
