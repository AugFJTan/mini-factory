#include "MachineAnimation.h"

MachineAnimation::MachineAnimation(Spritesheet* spritesheet, sPtr<AnimationFrames>& animation_frames, SDL_Rect rect, SDL_Point offset,
	bool has_particle_effect, ParticleEffect particle_effect) : Animation(spritesheet, animation_frames, rect), m_offset(offset) {
	if (has_particle_effect) {
		m_particle_effect = std::make_unique<ParticleEffect>();
		m_particle_effect->start_frame = particle_effect.start_frame;
		m_particle_effect->end_frame = particle_effect.end_frame;
		m_particle_effect->rect = particle_effect.rect;
		m_particle_effect->offset = particle_effect.offset;
	}
}

void MachineAnimation::render(SDL_Renderer* renderer, int scale, SDL_Point pos) {
	Animation::render(renderer, scale, {pos.x + m_offset.x, pos.y + m_offset.y});

	if (m_particle_effect == nullptr)
		return;

	SDL_Rect rect = m_particle_effect->rect;
	int last_frame = m_animation_frames->getLastFrame();
	int cells_per_row = (m_spritesheet->cols / rect.w) * rect.w;

	if (last_frame >= m_particle_effect->start_frame &&
		last_frame <= m_particle_effect->end_frame) {
		SDL_Rect src;
		src.x = ((rect.x + (last_frame - m_particle_effect->start_frame) * rect.w) % cells_per_row) * m_spritesheet->cell_length;
		src.y = (rect.y + (rect.x + (last_frame - m_particle_effect->start_frame) * rect.w) / cells_per_row * rect.h) * m_spritesheet->cell_length;
		src.w = rect.w * m_spritesheet->cell_length;
		src.h = rect.h * m_spritesheet->cell_length;

		SDL_Rect dst;
		dst.x = (pos.x + m_particle_effect->offset.x) * m_spritesheet->cell_length * scale;
		dst.y = (pos.y + m_particle_effect->offset.y) * m_spritesheet->cell_length * scale;
		dst.w = rect.w * m_spritesheet->cell_length * scale;
		dst.h = rect.h * m_spritesheet->cell_length * scale;

		SDL_RenderCopy(renderer, m_spritesheet->texture, &src, &dst);
	}
}
