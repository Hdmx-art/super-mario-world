#ifndef MYSTER_H
#define MYSTER_H

#include "levelobject.h"

class Coin;

class MysterBlock : public LevelObject
{
public:
	MysterBlock(World* pWorld);

	void update(float dt) override;
	void render(SDL_Renderer* pRenderer) override;

	void on_hit();

private:

	AnimComponent* m_anim;
	SDL_Rect m_rect;
	Coin *m_coin;

	Vector2f m_init_pos;

	bool m_hit;
	bool m_coin_spawned;
	bool m_finished;
	float m_grav;
};

#endif