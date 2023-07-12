#ifndef MAGIKOOPA_H
#define MAGIKOOPA_H

#include "ennemy.h"

class Magikoopa : public Ennemy
{
public:
	Magikoopa(World* pWorld);

	void update(float dt);
	void render(SDL_Renderer* pRenderer);

private:
	AnimComponent* m_animator;
	SDL_Rect m_rect;
	uint64_t m_time;
	uint64_t m_cast_delay;
	bool m_is_moving;
	Vector2f m_goal_pos;
	bool m_can_cast = false;
	uint32_t m_jump_count = 0;
};

#endif