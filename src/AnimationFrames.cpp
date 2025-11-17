#include "AnimationFrames.h"

AnimationFrames::AnimationFrames(int num_frames, int fps, bool loop) :
	m_num_frames(num_frames), m_fps(fps), m_state(loop ? LOOP : IDLE), m_last_frame(0), m_last_update(0) {
}

void AnimationFrames::update(Uint64 current) {
	if (m_state == IDLE) {
		m_last_update = current;
		return;
	}

	float dt = (current - m_last_update) / 1000.0f;
	int frames_to_update = floor(dt / (1.0f / m_fps));

	if (frames_to_update > 0) {
		m_last_frame += frames_to_update;
		if (m_state == PLAY_ONCE) {
			if (m_last_frame >= m_num_frames) {
				m_last_frame = 0;
				m_state = IDLE;
			}
		}
		m_last_frame %= m_num_frames;
		m_last_update = current;
	}
}

void AnimationFrames::playOnce() {
	if (m_state == LOOP)
		return;
	if (m_state == IDLE)
		m_state = PLAY_ONCE;
}

int AnimationFrames::getLastFrame() {
	return m_last_frame;
}
