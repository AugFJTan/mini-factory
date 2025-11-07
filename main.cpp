#include "Mini_Factory.h"
#include "Animation.h"
#include "AnimationFrames.h"

SDL_Rect world_to_screen(int x, int y, int scale, int length) {
	SDL_Rect rect;
	rect.x = x * scale * length;
	rect.y = y * scale * length;
	rect.w = scale * length;
	rect.h = scale * length;
	return rect;
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

	AnimationFrames belt_anim_frames(4, 12);

	Spritesheet spritesheet;
	spritesheet.texture = texture;
	spritesheet.cols = 10;
	spritesheet.rows = 118;
	spritesheet.length = 32;

	Animation belt_up_anim(&spritesheet, &belt_anim_frames, 6, 2);
	Animation belt_down_anim(&spritesheet, &belt_anim_frames, 5, 8);
	Animation belt_left_anim(&spritesheet, &belt_anim_frames, 5, 4);
	Animation belt_right_anim(&spritesheet, &belt_anim_frames, 5, 0);

	Animation belt_q1_cw_anim(&spritesheet, &belt_anim_frames, 12, 2);
	Animation belt_q2_cw_anim(&spritesheet, &belt_anim_frames, 10, 6);
	Animation belt_q3_cw_anim(&spritesheet, &belt_anim_frames, 9, 8);
	Animation belt_q4_cw_anim(&spritesheet, &belt_anim_frames, 11, 4);

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

		belt_anim_frames.update(start);

		SDL_Rect dst;
		for (int i = 0; i < 3; i++) {
			dst = world_to_screen(2+i, 1, scale, length);
			belt_right_anim.render(renderer, &dst);
		}

		for (int i = 0; i < 3; i++) {
			dst = world_to_screen(5, 2+i, scale, length);
			belt_down_anim.render(renderer, &dst);
		}

		for (int i = 0; i < 3; i++) {
			dst = world_to_screen(2+i, 5, scale, length);
			belt_left_anim.render(renderer, &dst);
		}

		for (int i = 0; i < 3; i++) {
			dst = world_to_screen(1, 2+i, scale, length);
			belt_up_anim.render(renderer, &dst);
		}

		dst = world_to_screen(5, 1, scale, length);
		belt_q1_cw_anim.render(renderer, &dst);

		dst = world_to_screen(5, 5, scale, length);
		belt_q2_cw_anim.render(renderer, &dst);

		dst = world_to_screen(1, 5, scale, length);
		belt_q3_cw_anim.render(renderer, &dst);

		dst = world_to_screen(1, 1, scale, length);
		belt_q4_cw_anim.render(renderer, &dst);

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
