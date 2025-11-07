#include "AnimationFrames.h"

AnimationFrames::AnimationFrames(int num_frames, int fps) :
	m_num_frames(num_frames), m_fps(fps), m_last_frame(0), m_last_update(0) {
}

void AnimationFrames::update(Uint64 current) {
	float dt = (current - m_last_update) / 1000.0f;
	int frames_to_update = floor(dt / (1.0f / m_fps));

	if (frames_to_update > 0) {
		m_last_frame += frames_to_update;
		m_last_frame %= m_num_frames;
		m_last_update = current;
	}
}

int AnimationFrames::getLastFrame() {
	return m_last_frame;
}
