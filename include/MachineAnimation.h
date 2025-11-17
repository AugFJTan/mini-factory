#ifndef MINI_FACTORY_MACHINE_ANIMATION_H_
#define MINI_FACTORY_MACHINE_ANIMATION_H_

#include "Mini_Factory.h"
#include "Animation.h"

struct ParticleEffect {
	int start_frame;
	int end_frame;
	SDL_Rect rect;
	SDL_Point offset;
};

class MachineAnimation : public Animation {
public:
	MachineAnimation(Spritesheet* spritesheet, sPtr<AnimationFrames>& animation_frames, SDL_Rect rect, SDL_Point offset, ParticleEffect particle_effect);
	void render(SDL_Renderer* renderer, int scale, SDL_Point pos) override;

private:
	SDL_Point m_offset;
	ParticleEffect m_particle_effect;
};

#endif
