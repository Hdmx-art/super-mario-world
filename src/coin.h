#ifndef COIN_H
#define COIN_H

#include "item.h"

class Coin : public Item
{
public:
	Coin(World* pWorld);

	void update(float dt) override;
	void render(SDL_Renderer* pRenderer) override;

	void on_hit(Mario* pMario);

private:
	AnimComponent* m_animator;
	Vector4i m_rect;
};

#endif