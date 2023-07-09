#ifndef CAMERA_H
#define CAMERA_H

#include "rects.h"

struct SDL_Renderer;

class Camera
{
public:
	static Camera& instance() {
		return s_instance;
	}

	void set_offset(float x, float y) {
		m_offset = { x,y };
	}

	const Vector2f& get_offset() {
		return m_offset;
	}

	void debug_draw(SDL_Renderer* pRenderer);

private:
	Vector2f m_offset;
	static Camera s_instance;

	Camera() {
		m_offset = { 0.f, 0.f };
	}
};

#endif