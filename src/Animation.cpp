#include "Animation.h"

Animation::Animation(Spritesheet* spritesheet, AnimationFrames* animation_frames, int row, int col) :
	m_spritesheet(spritesheet), m_animation_frames(animation_frames), m_row(row), m_col(col) {
}

void Animation::render(SDL_Renderer* renderer, SDL_Rect* dst) {
	int frame_row = m_row;
	int frame_col = m_col + m_animation_frames->getLastFrame();

	if (frame_col >= m_spritesheet->cols) {
		frame_row += 1;
	}

	SDL_Rect frame;
	frame.x = (frame_col % m_spritesheet->cols) * m_spritesheet->length;
	frame.y = frame_row * m_spritesheet->length;
	frame.w = m_spritesheet->length;
	frame.h = m_spritesheet->length;

	SDL_RenderCopy(renderer, m_spritesheet->texture, &frame, dst);
}
