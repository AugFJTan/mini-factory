#include "Animation.h"

Animation::Animation(Spritesheet* spritesheet, sPtr<AnimationFrames>& animation_frames, SDL_Rect rect) :
	m_spritesheet(spritesheet), m_animation_frames(animation_frames), m_rect(rect) {
}

void Animation::render(SDL_Renderer* renderer, int scale, SDL_Point pos) {
	int last_frame = m_animation_frames->getLastFrame();
	int cells_per_row = (m_spritesheet->cols / m_rect.w) * m_rect.w;

	SDL_Rect src;
	src.x = ((m_rect.x + last_frame * m_rect.w) % cells_per_row) * m_spritesheet->cell_length;
	src.y = (m_rect.y + (m_rect.x + last_frame * m_rect.w) / cells_per_row * m_rect.h) * m_spritesheet->cell_length;
	src.w = m_rect.w * m_spritesheet->cell_length;
	src.h = m_rect.h * m_spritesheet->cell_length;

	SDL_Rect dst;
	dst.x = pos.x * m_spritesheet->cell_length * scale;
	dst.y = pos.y * m_spritesheet->cell_length * scale;
	dst.w = m_rect.w * m_spritesheet->cell_length * scale;
	dst.h = m_rect.h * m_spritesheet->cell_length * scale;

	SDL_RenderCopy(renderer, m_spritesheet->texture, &src, &dst);
}
