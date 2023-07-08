#ifndef MUSHROOM_H
#define MUSHROOM_H

#include "item.h"

class World;

class Mushroom : public Item
{
public:
	Mushroom(World* pWorld);

	void update(float dt);
	void render(SDL_Renderer* pRenderer);

	void on_hit(Mario* pMario);

private:
	void update_physics(float dt);

	SDL_Rect m_rect;
	float m_vel;
	bool m_falling;
	float m_gravity;
};

#endif