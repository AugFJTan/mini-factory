#include "Animation.h"

Animation::Animation(Spritesheet* spritesheet, sPtr<AnimationFrames>& animation_frames, SDL_Point pos) :
	m_spritesheet(spritesheet), m_animation_frames(animation_frames), m_pos(pos) {
}

void Animation::render(SDL_Renderer* renderer, SDL_Rect* dst) {
	int sprite_x = m_pos.x + m_animation_frames->getLastFrame();
	int sprite_y = m_pos.y;

	if (sprite_x >= m_spritesheet->cols) {
		sprite_y += 1;
	}

	SDL_Rect frame;
	frame.x = (sprite_x % m_spritesheet->cols) * m_spritesheet->length;
	frame.y = sprite_y * m_spritesheet->length;
	frame.w = m_spritesheet->length;
	frame.h = m_spritesheet->length;

	SDL_RenderCopy(renderer, m_spritesheet->texture, &frame, dst);
}
