#ifndef COIN_H
#define COIN_H

#include "levelobject.h"

class Coin : public LevelObject
{
public:
	Coin(World* pWorld);

	void update(float dt) override;
	void render(SDL_Renderer* pRenderer) override;

	void on_destroy();

private:
	AnimComponent* m_animator;
	Vector4i m_rect;
};

#endif