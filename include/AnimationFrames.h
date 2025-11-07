#ifndef MINI_FACTORY_ANIMATION_FRAMES_H_
#define MINI_FACTORY_ANIMATION_FRAMES_H_

#include "Mini_Factory.h"

class AnimationFrames {
public:
	AnimationFrames(int num_frames, int fps);
	void update(Uint64 current);
	int getLastFrame();

private:
	int m_num_frames;
	int m_fps;
	int m_last_frame;
	Uint64 m_last_update;
};

#endif
