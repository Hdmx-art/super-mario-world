#ifndef PARTICLE_H
#define PARTICLE_H

#include "levelobject.h"

class World;

class Particle : public LevelObject
{
public:
	Particle(World* pWorld);

	void update(float dt);
	void render(SDL_Renderer* pRenderer);

	void spawn(float x, float y, float w, float h);

protected:
	AnimComponent* animator;
	SDL_Rect rect;
};

#endif