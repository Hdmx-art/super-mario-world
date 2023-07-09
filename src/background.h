#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "levelobject.h"

class Background : public LevelObject
{
public:
	Background(World* pWorld);

	void set_rect(const Vector2i& pos);

	virtual void update(float dt);
	virtual void render(SDL_Renderer* pRenderer);

private:
	SDL_Rect m_rect;
};

#endif