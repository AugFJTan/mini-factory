#ifndef MINI_FACTORY_ANIMATION_H_
#define MINI_FACTORY_ANIMATION_H_

#include "Mini_Factory.h"
#include "AnimationFrames.h"

struct Spritesheet {
	SDL_Texture* texture;
	int rows;
	int cols;
	int length;
};

class Animation {
public:
	Animation(Spritesheet* spritesheet, sPtr<AnimationFrames>& animation_frames, SDL_Point pos);
	void render(SDL_Renderer* renderer, SDL_Rect* dst);

private:
	Spritesheet* m_spritesheet;
	sPtr<AnimationFrames> m_animation_frames;
	SDL_Point m_pos;
};

#endif
