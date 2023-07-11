#include "mushroom.h"
#include "texturemanager.h"
#include "world.h"
#include "tile.h"
#include "mario.h"
#include "soundmanager.h"
#include "hud.h"

Mushroom::Mushroom(World* pWorld) : Item(pWorld)
{
	m_rect = { 32,224, 16,16 };
	set_size(64.f, 64.f);
	m_vel = 200.f;

	m_falling = true;
	m_gravity = 0.f;
}

void Mushroom::update(float dt)
{
	for (auto go = get_world()->begin(); go != get_world()->end(); ++go) {
		BasicTile* tile = dynamic_cast<BasicTile*>(go->get());

		if (tile) {
			if (tile->get_collide_type() != LevelObject::COLLIDE_TYPE::NONE && tile->is_drawable()) {
				if (SDL_HasIntersectionF(&get_transf(), &tile->get_transf())) {
					if (tile->get_collide_type() == LevelObject::COLLIDE_TYPE::TOTAL) {
						if (get_pos().y >= tile->get_pos().y) 
							m_vel *= -1.f;
						
					}
				}
			}
		}
	}

	update_physics(dt);
}

void Mushroom::render(SDL_Renderer* pRenderer)
{
	SDL_RenderCopyF(pRenderer, TextureManager::instance().get_texture("tiles"), &m_rect, &get_transf());
}

void Mushroom::on_hit(Mario* pMario)
{
	pMario->grow();
	HUD::instance().add_xp_text(get_pos().x, get_pos().y, 2000, 26 ,"grow");
	get_world()->delete_gameobject(this);
}

void Mushroom::update_physics(float dt)
{
	m_gravity += 2000.f * dt;

	if (m_gravity > 0.f)
		m_falling = true;

	move(m_vel, m_gravity, dt);

	for (auto go = get_world()->begin(); go != get_world()->end(); ++go) {
		LevelObject* level_object = dynamic_cast<LevelObject*>(go->get());

		if (level_object) {
			if (level_object->get_collide_type() != LevelObject::COLLIDE_TYPE::NONE && level_object->is_drawable()) {
				if (SDL_HasIntersectionF(&get_transf(), &level_object->get_transf())) {
					if (get_pos().y <= level_object->get_pos().y) {
						set_y(level_object->get_pos().y - get_size().y);
						m_falling = false;
						m_gravity = 0.f;
						break;
					}
				}
			}
		}
	}
}

