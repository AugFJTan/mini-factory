#include "Mini_Factory.h"
#include "Animation.h"
#include "MachineAnimation.h"
#include "AnimationFrames.h"
#include "TileID.h"
#include "Pathfinding.h"
#include "ItemPath.h"

#include "FileUtil.h"
#include "Util.h"

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
	sPtr<AnimationFrames> tesla_anim_frames;
	std::vector<uPtr<Animation>> animations;
	std::map<std::string, TileID> tile_lookup;
	Tile map[MAP_WIDTH * MAP_HEIGHT];
	Path path;

	parse_animation_file("../config/anim_belt.txt", BELT_UP, &spritesheet, tile_lookup, belt_anim_frames, animations);
	parse_animation_file("../config/anim_tesla.txt", MACHINE_TESLA, &spritesheet, tile_lookup, tesla_anim_frames, animations);
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

	int tesla_pause = 0;

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

		tesla_pause += start - previous;
		if (tesla_pause > 1000) {
			tesla_pause = 0;
			tesla_anim_frames->playOnce();
		}

		belt_anim_frames->update(start);
		tesla_anim_frames->update(start);

		for (int y = 0; y < MAP_HEIGHT; y++) {
			for (int x = 0; x < MAP_WIDTH; x++) {
				SDL_Point pos = {x, y};
				Tile belt = map[to_index(pos)];
				if (belt.getType() != BELT)
					continue;
				animations[belt.getID()]->render(renderer, scale, pos);
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

		SDL_Point pos = {5, 4};
		animations[MACHINE_TESLA]->render(renderer, scale, pos);

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
