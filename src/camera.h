#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
public:
	static Camera& instance() {
		return s_instance;
	}

	void set_offset(float x) {
		m_offset = x;
	}

	float get_offset() {
		return m_offset;
	}

private:
	float m_offset;
	static Camera s_instance;
	Camera() {
		m_offset = 0.f;
	}
};

#endif