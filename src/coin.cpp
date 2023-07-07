#include "coin.h"
#include "world.h"

Coin::Coin(World* pWorld) : LevelObject(pWorld)
{
	m_animator = add_component<AnimComponent>();

	set_size(64.f, 64.f);

	m_rect = { 0,0,16,16 };
	
	m_animator->add_animation("rotate", { {16,16},{0,0}, 4, 0.4f, 0, true });
	m_animator->add_animation("destroyed", { {16,28},{0,0}, 10, 0.4f, 0, false });
	m_animator->set_animation("rotate");
}

void Coin::update(float dt)
{
	m_animator->update(dt, &m_rect);
	
	if (m_animator->get_current_animation() == "destroyed") {
		if (m_animator->is_finished()) {
			get_world()->delete_gameobject(this);
		}
	}
}

void Coin::render(SDL_Renderer* pRenderer)
{
	if (m_animator->get_current_animation() == "rotate") {
		SDL_CHECK(SDL_RenderCopyExF(
			pRenderer,
			TextureManager::instance().get_texture("tiles"),
			&m_rect,
			&get_transf(),
			0.f,
			&get_origin(),
			get_flip()
		));
	}
	else if (m_animator->get_current_animation() == "destroyed") {
		SDL_CHECK(SDL_RenderCopyExF(
			pRenderer,
			TextureManager::instance().get_texture("particles"),
			&m_rect,
			&get_transf(),
			0.f,
			&get_origin(),
			get_flip()
		));
	}
}

void Coin::on_destroy()
{
	if (m_animator->get_current_animation() != "destroyed") {
		m_animator->set_animation("destroyed");
		set_size(32.f, 48.f);
		move((TILE_SIZE - get_size().x) / 2.f, 0.f, 1.f);
	}
}
