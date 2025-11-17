#ifndef MINI_FACTORY_ANIMATION_H_
#define MINI_FACTORY_ANIMATION_H_

#include "Mini_Factory.h"
#include "AnimationFrames.h"

struct Spritesheet {
	SDL_Texture* texture;
	int rows;
	int cols;
	int cell_length;
};

class Animation {
public:
	Animation(Spritesheet* spritesheet, sPtr<AnimationFrames>& animation_frames, SDL_Rect rect);
	virtual void render(SDL_Renderer* renderer, int scale, SDL_Point pos);

protected:
	Spritesheet* m_spritesheet;
	sPtr<AnimationFrames> m_animation_frames;

private:
	SDL_Rect m_rect;
};

#endif
