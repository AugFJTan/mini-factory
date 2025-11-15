#include "Animation.h"

Animation::Animation(Spritesheet* spritesheet, sPtr<AnimationFrames>& animation_frames, SDL_Rect rect) :
	m_spritesheet(spritesheet), m_animation_frames(animation_frames), m_rect(rect) {
}

void Animation::render(SDL_Renderer* renderer, int scale, SDL_Point pos) {
	int sprite_x = m_rect.x + m_animation_frames->getLastFrame();
	int sprite_y = m_rect.y;

	if (sprite_x >= m_spritesheet->cols) {
		sprite_y += 1;
	}

	SDL_Rect frame;
	frame.x = (sprite_x % m_spritesheet->cols) * m_spritesheet->cell_length;
	frame.y = sprite_y * m_spritesheet->cell_length;
	frame.w = m_rect.w;
	frame.h = m_rect.h;

	SDL_Rect dst;
	dst.x = pos.x * m_spritesheet->cell_length * scale;
	dst.y = pos.y * m_spritesheet->cell_length * scale;
	dst.w = m_rect.w * scale;
	dst.h = m_rect.h * scale;

	SDL_RenderCopy(renderer, m_spritesheet->texture, &frame, &dst);
}
