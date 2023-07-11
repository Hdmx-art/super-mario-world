#include "magicbullet.h"
#include "texturemanager.h"
#include "mario.h"
#include "world.h"
#include "game.h"

MagicBullet::MagicBullet(World* pWorld) : Ennemy(pWorld)
{
	m_animator = add_component<AnimComponent>();

	set_size(64.f, 64.f);
	m_animator->add_animation("rotate", { {26,26}, { 342,544 }, 4, 0.7f,0,true });
	m_animator->set_animation("rotate");

	set_origin(get_size().x / 2.f, get_size().y / 2.f);
}

void MagicBullet::update(float dt)
{
	rotate(100.f * dt);
	m_animator->update(dt, &m_rect);
	
	move(-m_vel.x, -m_vel.y, dt);
	
	if (get_pos().x + get_size().x < 0.f)
		get_world()->delete_gameobject(this);
	else if (get_pos().x >= Game::instance().get_window_size().x) 
		get_world()->delete_gameobject(this);
	else if(get_pos().y < 0.f)
		get_world()->delete_gameobject(this);
	else if(get_pos().y + get_size().y >= Game::instance().get_window_size().y)
		get_world()->delete_gameobject(this);
}

void MagicBullet::render(SDL_Renderer* pRenderer)
{
	SDL_RenderCopyExF(pRenderer, TextureManager::instance().get_texture("enemies"), &m_rect, &get_transf(),
		get_rotation(), &get_origin(), get_flip());
}

void MagicBullet::cast()
{
	for (auto go = get_world()->begin(); go != get_world()->end(); ++go) {
		if (dynamic_cast<Mario*>(go->get())) {

			Mario* mario = dynamic_cast<Mario*>(go->get());

			Vector2f vec = { get_pos().x - mario->get_pos().x, get_pos().y - mario->get_pos().y };

			m_vel.x = (vec.x / get_length(vec)) * 150.f;
			m_vel.y = (vec.y / get_length(vec)) * 150.f;

			break;
		}
	}
}
