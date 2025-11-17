#include "MachineAnimation.h"

MachineAnimation::MachineAnimation(Spritesheet* spritesheet, sPtr<AnimationFrames>& animation_frames, SDL_Rect rect, SDL_Point offset, ParticleEffect particle_effect) :
	Animation(spritesheet, animation_frames, rect), m_offset(offset), m_particle_effect(particle_effect) {
}

void MachineAnimation::render(SDL_Renderer* renderer, int scale, SDL_Point pos) {
	Animation::render(renderer, scale, {pos.x + m_offset.x, pos.y + m_offset.y});

	if (m_animation_frames->getLastFrame() >= m_particle_effect.start_frame &&
		m_animation_frames->getLastFrame() <= m_particle_effect.end_frame) {
		SDL_Rect src;
		src.x = (m_particle_effect.rect.x + m_animation_frames->getLastFrame() - m_particle_effect.start_frame) * m_spritesheet->cell_length;
		src.y = m_particle_effect.rect.y * m_spritesheet->cell_length;
		src.w = m_particle_effect.rect.w * m_spritesheet->cell_length;
		src.h = m_particle_effect.rect.h * m_spritesheet->cell_length;

		SDL_Rect dst;
		dst.x = (pos.x + m_particle_effect.offset.x) * m_spritesheet->cell_length * scale;
		dst.y = (pos.y + m_particle_effect.offset.y) * m_spritesheet->cell_length * scale;
		dst.w = m_particle_effect.rect.w * m_spritesheet->cell_length * scale;
		dst.h = m_particle_effect.rect.h * m_spritesheet->cell_length * scale;

		SDL_RenderCopy(renderer, m_spritesheet->texture, &src, &dst);
	}
}
