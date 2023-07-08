#include "particle.h"
#include "world.h"

Particle::Particle(World* pWorld) : LevelObject(pWorld)
{
	animator = add_component<AnimComponent>();
	rect = { 0,0,0,0 };
}

void Particle::update(float dt)
{
	animator->update(dt, &rect);

	if (animator->is_finished())
		get_world()->delete_gameobject(this);
}

void Particle::render(SDL_Renderer* pRenderer)
{
	SDL_RenderCopyF(pRenderer, TextureManager::instance().get_texture("particles"), &rect, &get_transf());
}

void Particle::spawn(float x, float y, float w, float h)
{
	set_pos(x, y);
	set_size(w, h);
}