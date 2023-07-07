#include "mysterblock.h"
#include "coin.h"
#include "world.h"

MysterBlock::MysterBlock(World* pWorld) : LevelObject(pWorld)
{
	m_anim = add_component<AnimComponent>();

	set_size(64.f, 64.f);
	m_rect = { 48,64,16,16 };
	set_collide_type(LevelObject::COLLIDE_TYPE::TOTAL);
	
	m_grav = 0.f;
	m_hit = false;
	m_finished = false;
	m_coin_spawned = false;

	m_anim->add_animation("tick", { {16,16},{0,64},4,0.5f,0,true });
	m_anim->add_animation("finished", { {16,16},{80,64},1,0.3f,0,false });
	m_anim->set_animation("tick");
}

void MysterBlock::update(float dt)
{
	m_anim->update(dt, &m_rect);

	if (m_finished) 
		m_anim->set_animation("finished");
	else 
		m_anim->set_animation("tick");
	

	if (m_hit) {

		if (!m_coin_spawned) {
			m_coin = get_world()->add_gameobject<Coin>();
			m_coin->set_pos(get_pos().x, get_pos().y - m_coin->get_size().y);
		}

		m_grav -= 7500.f * dt;

		if (m_grav <= -1000.f) {
			m_grav *= -1.f;
		}

		if (get_pos().y <= m_init_pos.y) {
			move(0, m_grav, dt);
			m_coin->move(0.f, m_grav, dt);
		}
		else {
			set_y(m_init_pos.y);
			m_hit = false;
			m_finished = true;
			m_coin->on_destroy();
		}

		m_coin_spawned = true;
	}
}

void MysterBlock::render(SDL_Renderer* pRenderer)
{
	SDL_RenderCopyF(pRenderer, TextureManager::instance().get_texture("tiles"), &m_rect, &get_transf());
}

void MysterBlock::on_hit()
{
	if (m_finished == false) {
		m_hit = true;
		m_init_pos = get_pos();
		m_grav = 0.f;
	}
}