#ifndef SHELL_H
#define SHELL_H

#include "ennemy.h"

class Shell : public Ennemy
{
public:
	enum class Color {
		RED, GREEN
	};

	Shell(World* pWorld);

	void spawn(float x, float y, Color color);
	float get_vel();

	void update(float dt);
	void render(SDL_Renderer* pRenderer);
private:
	void update_physics(float dt);

	AnimComponent* m_animator;
	SDL_Rect m_rect;

	bool m_falling;
	float m_gravity;
	float m_vel;
	const float MAX_VEL = 700.f;
};

#endif