#ifndef MINI_FACTORY_ANIMATION_FRAMES_H_
#define MINI_FACTORY_ANIMATION_FRAMES_H_

#include "Mini_Factory.h"

typedef enum AnimationState {
	IDLE,
	PLAY_ONCE,
	LOOP
} AnimationState;

class AnimationFrames {
public:
	AnimationFrames(int num_frames, int fps, bool loop);
	void update(Uint64 current);
	void playOnce();
	int getLastFrame();

private:
	int m_num_frames;
	int m_fps;
	AnimationState m_state;
	int m_last_frame;
	Uint64 m_last_update;
};

#endif
