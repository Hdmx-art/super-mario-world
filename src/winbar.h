#ifndef WINBAR_H
#define WINBAR_H

#include "levelobject.h"

class World;

class WinBar : public LevelObject
{
public:
	WinBar(World* pWorld);
	
	void update(float dt);
	void render(SDL_Renderer* pRenderer);
	void set_init_pos(float x, float y);

private:
	SDL_Rect m_rect;
	Vector2f m_init_pos;
	float m_vel = 200.f;
};

#endif