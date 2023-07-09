#ifndef MAGICBULLET_H
#define MAGICBULLET_H

#include "ennemy.h"

class MagicBullet : public Ennemy
{
public:
	MagicBullet(World* pWorld);

	void update(float dt);
	void render(SDL_Renderer* pRenderer);

	void cast();

private:
	AnimComponent* m_animator;
	SDL_Rect m_rect;
	Vector2f m_vel;
};

#endif