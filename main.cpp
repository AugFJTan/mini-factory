#include "Mini_Factory.h"
#include "Animation.h"
#include "AnimationFrames.h"
#include "AnimationID.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

SDL_Rect world_to_screen(int x, int y, int scale, int length) {
	SDL_Rect rect;
	rect.x = x * scale * length;
	rect.y = y * scale * length;
	rect.w = scale * length;
	rect.h = scale * length;
	return rect;
}

void parse_file(Spritesheet* spritesheet, sPtr<AnimationFrames>& animation_frames, std::vector<uPtr<Animation>>& animations) {
	std::ifstream file;
	file.open("../config/anim_belt.txt");

	std::string data;
	bool first_line = true;
	int frames, fps;
	std::string anim_id;
	int row, col;

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
			ss >> anim_id >> row >> col;
			animations.push_back(std::make_unique<Animation>(spritesheet, animation_frames, row, col));
			std::cout << anim_id << " @ (" << row << ", " << col << ")" << std::endl;
		}
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

	parse_file(&spritesheet, belt_anim_frames, belt_animations);

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

		SDL_Rect dst;
		for (int i = 0; i < 3; i++) {
			dst = world_to_screen(2+i, 1, scale, length);
			belt_animations[BELT_RIGHT]->render(renderer, &dst);
		}

		for (int i = 0; i < 3; i++) {
			dst = world_to_screen(5, 2+i, scale, length);
			belt_animations[BELT_DOWN]->render(renderer, &dst);
		}

		for (int i = 0; i < 3; i++) {
			dst = world_to_screen(2+i, 5, scale, length);
			belt_animations[BELT_LEFT]->render(renderer, &dst);
		}

		for (int i = 0; i < 3; i++) {
			dst = world_to_screen(1, 2+i, scale, length);
			belt_animations[BELT_UP]->render(renderer, &dst);
		}

		dst = world_to_screen(5, 1, scale, length);
		belt_animations[BELT_Q1_CW]->render(renderer, &dst);

		dst = world_to_screen(5, 5, scale, length);
		belt_animations[BELT_Q2_CW]->render(renderer, &dst);

		dst = world_to_screen(1, 5, scale, length);
		belt_animations[BELT_Q3_CW]->render(renderer, &dst);

		dst = world_to_screen(1, 1, scale, length);
		belt_animations[BELT_Q4_CW]->render(renderer, &dst);

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
