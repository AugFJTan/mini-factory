#include "Mini_Factory.h"
#include "Animation.h"
#include "AnimationFrames.h"
#include "AnimationID.h"
#include "Belt.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

SDL_Rect world_to_screen(SDL_Point pos, int scale, int length) {
	SDL_Rect rect;
	rect.x = pos.x * scale * length;
	rect.y = pos.y * scale * length;
	rect.w = scale * length;
	rect.h = scale * length;
	return rect;
}

int to_index(SDL_Point pos) {
	return pos.y * MAP_WIDTH + pos.x;
}

void parse_animation_file(Spritesheet* spritesheet, std::map<std::string, AnimationID>& animation_lookup,
	sPtr<AnimationFrames>& animation_frames, std::vector<uPtr<Animation>>& animations) {
	std::ifstream file;
	file.open("../config/anim_belt.txt");

	std::string data;
	bool first_line = true;
	int frames, fps;
	std::string anim_id;
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
			ss >> anim_id >> x >> y;
			SDL_Point pos = {x, y};
			animations.push_back(std::make_unique<Animation>(spritesheet, animation_frames, pos));
			animation_lookup[anim_id] = static_cast<AnimationID>(n);
			std::cout << anim_id << " @ (" << x << ", " << y << ")" << std::endl;
			n++;
		}
	}
}

void parse_layout_file(std::vector<uPtr<AnimatedTile>>& map, std::map<std::string, AnimationID>& animation_lookup) {
	std::ifstream file;
	file.open("../config/layout.txt");

	std::string data;
	std::string anim_id;
	int x, y;

	while (std::getline(file, data)) {
		std::istringstream ss(data);
		ss >> anim_id >> x >> y;
		SDL_Point pos = {x, y};
		map[to_index(pos)] = std::make_unique<Belt>(animation_lookup[anim_id]);
		std::cout << anim_id << " " << "(" << x << ", " << y << ")" << std::endl;
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
	spritesheet.length = 32;

	sPtr<AnimationFrames> belt_anim_frames;
	std::vector<uPtr<Animation>> belt_animations;
	std::map<std::string, AnimationID> animation_lookup;
	std::vector<uPtr<AnimatedTile>> map(MAP_WIDTH * MAP_HEIGHT);

	parse_animation_file(&spritesheet, animation_lookup, belt_anim_frames, belt_animations);
	parse_layout_file(map, animation_lookup);

	int scale = 2;
	int length = spritesheet.length;

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
				Belt *belt = dynamic_cast<Belt*>(map[to_index(pos)].get());
				if (belt == nullptr)
					continue;
				SDL_Rect dst = world_to_screen(pos, scale, length);
				belt_animations[belt->getAnimationID()]->render(renderer, &dst);
			}
		}

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
