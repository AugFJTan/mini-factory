#include "Mini_Factory.h"
#include "Animation.h"
#include "AnimationFrames.h"
#include "TileID.h"
#include "Pathfinding.h"
#include "ItemPath.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "Util.h"

void parse_animation_file(Spritesheet* spritesheet, std::map<std::string, TileID>& tile_lookup,
	sPtr<AnimationFrames>& animation_frames, std::vector<uPtr<Animation>>& animations) {
	std::ifstream file;
	file.open("../config/anim_belt.txt");

	std::string data;
	bool first_line = true;
	int frames, fps;
	std::string tile_id;
	int x, y;
	int n = 0;

	while (std::getline(file, data)) {
		if (data == "")
			continue;
		if (data[0] == '#')
			continue;
		std::istringstream ss(data);
		if (first_line) {
			ss >> frames >> fps;
			animation_frames = std::make_shared<AnimationFrames>(frames, fps);
			std::cout << "Frames = " << frames << ", FPS = " << fps << std::endl;
			first_line = false;
		} else {
			ss >> tile_id >> x >> y;
			SDL_Rect rect = {x, y, 32, 32};
			animations.push_back(std::make_unique<Animation>(spritesheet, animation_frames, rect));
			tile_lookup[tile_id] = static_cast<TileID>(n++);
			std::cout << tile_id << " @ (" << x << ", " << y << ")" << std::endl;
		}
	}
}

void parse_layout_file(Tile map[], std::map<std::string, TileID>& tile_lookup, Path& path) {
	std::ifstream file;
	file.open("../config/layout.txt");

	std::string data;
	std::string tile_id;
	int x, y;

	while (std::getline(file, data)) {
		std::istringstream ss(data);
		ss >> tile_id >> x >> y;
		SDL_Point pos = {x, y};
		Tile belt(BELT, tile_lookup[tile_id]);
		map[to_index(pos)] = belt;
		std::cout << tile_id << " " << "(" << x << ", " << y << ")" << std::endl;
	}
}

void parse_item_lanes_file(std::vector<std::vector<SDL_Point>>& belt_lane_offsets) {
	std::ifstream file;
	file.open("../config/item_lanes.txt");

	std::string data;
	std::string lane_id, points;

	while (std::getline(file, data)) {
		std::istringstream ss(data);
		ss >> lane_id >> points;
		std::cout << "Lane: " << lane_id << " Pos: " << points << std::endl;
		size_t idx = 0, lbracket = 0, rbracket = 0;
		std::vector<SDL_Point> offsets;
		while(idx < points.length()) {
			lbracket = points.find('(', idx);
			rbracket = points.find(')', idx);
			std::string coord = points.substr(lbracket+1, rbracket-lbracket-1);
			size_t comma = coord.find(',');
			SDL_Point pos;
			pos.x = std::stoi(coord.substr(0, comma));
			pos.y = std::stoi(coord.substr(comma+1));
			offsets.push_back(pos);
			idx = rbracket + 2;
		}
		belt_lane_offsets.push_back(offsets);
	}
}

int main(int argc, char* args[]) {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Could not initialize SDL2: %s\n", SDL_GetError());
		return 1;
	}

	window = SDL_CreateWindow("Mini Factory",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);
	if (window == NULL) {
		fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		fprintf(stderr, "Could not create renderer: %s\n", SDL_GetError());
		return 1;
	}

	bool quit = false;
	SDL_Event event;

	SDL_Texture* texture = IMG_LoadTexture(renderer, "../assets/automation_spritesheet.png");
	if (texture == NULL) {
		fprintf(stderr, "Could not create texture: %s\n", SDL_GetError());
		return 1;
	}

	Spritesheet spritesheet;
	spritesheet.texture = texture;
	spritesheet.cols = 10;
	spritesheet.rows = 118;
	spritesheet.cell_length = 32;

	sPtr<AnimationFrames> belt_anim_frames;
	std::vector<uPtr<Animation>> belt_animations;
	std::map<std::string, TileID> tile_lookup;
	Tile map[MAP_WIDTH * MAP_HEIGHT];
	Path path;

	parse_animation_file(&spritesheet, tile_lookup, belt_anim_frames, belt_animations);
	parse_layout_file(map, tile_lookup, path);

	path.traverse(map);
	std::vector<Node*> belt_paths = path.getPaths();

	std::vector<std::vector<SDL_Point>> belt_lane_offsets;
	parse_item_lanes_file(belt_lane_offsets);

	std::vector<ItemPath> item_paths;
	for (int i = 0; i < belt_paths.size(); i++) {
		ItemPath item_path;
		item_path.update(map, belt_lane_offsets, belt_paths[i]);
		item_paths.push_back(item_path);
	}

	int total_distance = item_paths[0].getTotalDistance();

	int scale = 2;
	int length = spritesheet.cell_length;

	SDL_Rect item_rect = {3 * length, 2 * length, 12, 11};
	float distance = 0;
	Uint64 previous = SDL_GetTicks64();
	int pixels_per_sec = 45;

	while (!quit) {
		while (SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					quit = true;
					break;
				default:
					break;
			}
		}

		Uint64 start = SDL_GetTicks64();

		// Do stuff
		SDL_SetRenderDrawColor(renderer, 0x7f, 0x7f, 0x7f, 0xff);  // Background
		SDL_RenderClear(renderer);

		belt_anim_frames->update(start);

		for (int y = 0; y < MAP_HEIGHT; y++) {
			for (int x = 0; x < MAP_WIDTH; x++) {
				SDL_Point pos = {x, y};
				Tile belt = map[to_index(pos)];
				if (belt.getType() != BELT)
					continue;
				belt_animations[belt.getID()]->render(renderer, scale, pos);
			}
		}

		for (int i = 0; i < belt_paths.size(); i++) {
			Node* current = belt_paths[i];
			while (current->next != nullptr) {
				SDL_Point a = current->pos;
				SDL_Point b = current->next->pos;
				a.x = (a.x * length + length / 2) * scale;
				a.y = (a.y * length + length / 2) * scale;
				b.x = (b.x * length + length / 2) * scale;
				b.y = (b.y * length + length / 2) * scale;
				SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
				SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
				current = current->next;
			}
		}

		for (int i = 0; i < item_paths.size(); i++) {
			item_paths[i].drawLaneA(renderer, scale);
			item_paths[i].drawLaneB(renderer, scale);
		}

		if (distance > total_distance)
			distance = 0;

		distance += (start - previous) / (1000.f / pixels_per_sec);
		previous = start;

		item_paths[0].drawItemLaneA(renderer, spritesheet.texture, scale, &item_rect, (int)distance);
		item_paths[0].drawItemLaneB(renderer, spritesheet.texture, scale, &item_rect, (int)distance);

		SDL_RenderPresent(renderer);

		Uint64 end = SDL_GetTicks64();

		float elapsedMS = end - start;

		// Cap to 60 FPS
		SDL_Delay(floor(16.666f - elapsedMS));
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
