#include "Magikoopa.h"
#include "texturemanager.h"
#include "world.h"
#include "magicbullet.h"
#include "game.h"
#include <cstdlib>

Magikoopa::Magikoopa(World* pWorld) : Ennemy(pWorld)
{
	set_size(96.f, 116.f);
	m_animator = add_component<AnimComponent>();

	m_animator->add_animation("cast", { {26,31}, {352,500}, 3, 1.f, 0, false });
	m_animator->add_animation("idle", { {26,31}, {352+26*2,500}, 1, 1.f, 0, true });
	m_animator->set_animation("idle");

	m_goal_pos.x = rand() % (Game::instance().get_window_size().x - 500) + 200;
	m_goal_pos.y = rand() % (Game::instance().get_window_size().y - 500) + 50;

	m_time = SDL_GetTicks64();
	m_cast_delay = 5000;
}

void Magikoopa::update(float dt)
{
	if (SDL_GetTicks64() > m_time + m_cast_delay && m_can_cast) {
		m_animator->set_animation("cast");
		m_time = SDL_GetTicks64();
		m_can_cast = false;
	}

	for (auto go = get_world()->begin(); go != get_world()->end(); ++go) {
		if (dynamic_cast<Mario*>(go->get())) {

			Mario* mario = dynamic_cast<Mario*>(go->get());

			if (mario->get_pos().x < get_pos().x)
				set_flip(SDL_FLIP_HORIZONTAL);
			else
				set_flip(SDL_FLIP_NONE);
			break;
		}
	}

	if (m_animator->is_finished() && m_animator->get_current_animation() == "cast") {
		MagicBullet* bullet = get_world()->add_gameobject<MagicBullet>();
		bullet->set_pos(get_pos().x, get_pos().y);
		bullet->cast();
		m_animator->set_animation("idle");
		
		m_goal_pos.x = rand() % (Game::instance().get_window_size().x - 500) + 200;
		m_goal_pos.y = rand() % (Game::instance().get_window_size().y - 500) + 50;
	}

	if (m_animator->get_current_animation() == "idle")
	{
		Vector2f vec = { m_goal_pos.x - get_pos().x, m_goal_pos.y - get_pos().y };

		move((vec.x / get_length(vec)) * 200.f, (vec.y / get_length(vec)) * 200.f, dt);
		
		if(abs(vec.x) <= 2.f && abs(vec.y) <= 2.f)
			m_can_cast = true;
	}

	m_animator->update(dt, &m_rect);
}

void Magikoopa::render(SDL_Renderer* pRenderer)
{
	SDL_RenderCopyExF(pRenderer, TextureManager::instance().get_texture("enemies"),
		&m_rect, &get_transf(), 0.f, nullptr, get_flip());
}
