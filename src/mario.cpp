#include "mario.h"
#include "SDL.h"
#include "world.h"
#include "keyboard.h"
#include "tile.h"
#include "coin.h"
#include "mysterblock.h"
#include "camera.h"
#include "game.h"
#include "koopa.h"
#include "shell.h"
#include "hud.h"
#include "soundmanager.h"
#include "all_particles.h"
#include "item.h"
#include "magikoopa.h"
#include "magicbullet.h"

Mario::Mario(World* pWorld) : LevelObject(pWorld)
{
	set_size(64.f, 96.f);
	m_rect = { 0,0,18,32 };
	m_direction.y = 0.f;
	m_falling = false;
	m_direction = { 0.f,0.f };
	m_hit_head = false;
	m_frozen = false;
	m_state = MarioState::SMALL;
	m_unstoppable_time = 1500;
	m_time = 0;
	m_coin_count = 0;

	m_animator = add_component<AnimComponent>();
	m_animator->add_animation("small-idle", { {14,19}, {4,9}, 1, 0.3f,0, true });
	m_animator->add_animation("big-idle", { {15,28}, {4,60}, 1, 0.3f,0, true });

	m_animator->add_animation("small-walk", { {15,19}, {4,9}, 2, 0.15f,2, true });
	m_animator->add_animation("big-walk", { {16,28}, {4,60}, 2, 0.2f,1, true });
	
	m_animator->add_animation("small-jumping", { {16,22}, {105,6}, 1, 0.3f, 0, true });
	m_animator->add_animation("big-jumping", { {16,31}, {145,59}, 1, 0.3f, 0, true });

	m_animator->add_animation("small-falling", { {16,20}, {122,7}, 1, 0.3f, 0, true });
	m_animator->add_animation("big-falling", { {16,29}, {168,60}, 1, 0.3f, 0, true });

	m_animator->add_animation("grow", { {15,27}, {0,228}, 3, 0.75f, 2, false });
	m_animator->add_animation("decrease", { {15,27}, {50,228}, 3, 0.5f, 2, false });
	m_animator->add_animation("die", { {16,24}, {401,6}, 1, 0.3f, 0, true });

	m_animator->set_animation("small-idle");
}

#include <iostream>
void Mario::update(float dt)
{
	std::cout << (int)m_state << std::endl;
	if (!is_ejected()) {

		if (m_state != MarioState::GROWING && m_state != MarioState::DECREASING) {
			vertical_movement(dt);
			vertical_collision(dt);

			horizontal_movement(dt);
			horizontal_collision(dt);

			collect_item(dt);
			select_anim();
		}
		else {
			if (m_state == MarioState::GROWING) {
				if (m_animator->is_finished()) {
					m_state = MarioState::BIG;
				}
			}
			else if (m_state == MarioState::DECREASING) {
				if (m_animator->is_finished()) {
					m_state = MarioState::SMALL;
				}
			}

			if (m_state == MarioState::DEAD)
				m_animator->set_animation("die");

			set_size(64.f, 112.f);
		}

		if (m_state == MarioState::BIG)
			set_size(64.f, 112.f);
		else if (m_state == MarioState::SMALL)
			set_size(64.f, 88.f);
	}

	m_animator->update(dt, &m_rect);

	if (is_ejected()) {
		Camera::instance().set_offset(0.f, 0.f);
		set_size(64.f, 96.f);
		eject_process(get_world(), 0.f, dt);
	}
}

void Mario::render(SDL_Renderer* pRenderer)
{
	SDL_RenderCopyExF(
		pRenderer,
		TextureManager::instance().get_texture("mario"),
		&m_rect,
		&get_transf(),
		0.f,
		&get_origin(),
		get_flip()
	);

	HUD::instance().draw_number(pRenderer, std::to_string(m_coin_count), {Game::instance().get_window_size().x - 175.f,70.f}, 48);
}

void Mario::bump()
{
	m_direction.y = -1200.f;
}

void Mario::eject_process(World* pWorld, float velx, float dt)
{
	eject_gravity() += 2500.f * dt;
	move(0.f, eject_gravity(), dt);
	m_direction.x = 0.f;

	if (get_pos().y > Game::instance().get_window_size().y) {
		pWorld->delete_gameobject(this);
		return;
	}
}

float Mario::get_vel()
{
	return m_direction.x;
}

bool Mario::is_falling()
{
	return m_falling;
}

void Mario::decrease()
{

}

void Mario::grow()
{
	m_state = MarioState::GROWING;
	m_animator->set_animation("grow");
	get_world()->set_pause_for(1500);
}

Mario::MarioState Mario::get_state()
{
	return m_state;
}

void Mario::add_coin()
{
	m_coin_count++;
}

uint32_t Mario::get_coin_count()
{
	return m_coin_count;
}

#include <iostream>
void Mario::vertical_movement(float dt)
{
	if (m_hit_head)
		m_direction.y *= -1.f;

	if (Keyboard::instance().is_key_pressed(SDL_SCANCODE_SPACE) && m_falling == false) {
		bump();
		SoundManager::instance().play("jump");
		m_falling = true;
	}

	if (m_direction.y > 0.f) {
		m_falling = true;
	}

	if (get_pos().y <= 100.f) {
		Camera::instance().set_offset(Camera::instance().get_offset().x, -m_direction.y);
		set_y(100.f);
		m_direction.x = 0.f;
	}
	else if (get_pos().y + get_size().y >= Game::instance().get_window_size().y - 100.f) {

		Camera::instance().set_offset(Camera::instance().get_offset().x, -m_direction.y);
		set_y(Game::instance().get_window_size().y - 100.f - get_size().y);
		m_direction.x = 0.f;
	}
	else 
		Camera::instance().set_offset(Camera::instance().get_offset().x, 0.f);
	
	move(0, m_direction.y, dt);
	m_direction.y += 2000.f * dt;
}

void Mario::vertical_collision(float dt)
{
	m_hit_head = false;

	for (auto go = get_world()->begin(); go != get_world()->end(); ++go) {
		LevelObject* level_object = dynamic_cast<LevelObject*>(go->get());

		if (level_object != nullptr) {
			if (level_object->is_drawable()) {
				if (level_object->get_collide_type() == LevelObject::COLLIDE_TYPE::NONE) {
					SDL_FRect rect = {};
					if (SDL_IntersectFRect(&get_transf(), &level_object->get_transf(), &rect)) {
						if (dynamic_cast<Koopa*>(level_object) && m_falling) {
							Koopa* koopa = dynamic_cast<Koopa*>(level_object);

							if (!koopa->sliding()) {
								koopa->on_hit(this);

								StompParticle* particle = get_world()->add_gameobject<StompParticle>();
								particle->spawn(rect.x, rect.y, 24.f, 24.f);

								break;
							}
						}
					}
				}
				else {
					if (SDL_HasIntersectionF(&get_transf(), &level_object->get_transf())) {

						if (level_object->get_collide_type() == LevelObject::COLLIDE_TYPE::ON_TOP) {

							if (get_pos().y < level_object->get_pos().y && m_direction.y > 0.f)
								on_floor(level_object);
						}
						else if (level_object->get_collide_type() == LevelObject::COLLIDE_TYPE::TOTAL) {

							if (m_direction.y > 0.f) {
								on_floor(level_object);
								break;
							}
							else if (m_direction.y < 0.f) {
								set_y(level_object->get_pos().y + level_object->get_size().y);
								m_hit_head = true;

								if (dynamic_cast<MysterBlock*>(level_object)) {
									MysterBlock* myster_block = dynamic_cast<MysterBlock*>(level_object);
									myster_block->on_hit();
								}

								break;
							}

						}
					}
				}
			}
		}
	}
}

void Mario::horizontal_movement(float dt)
{
	if (Keyboard::instance().is_key_pressed(SDL_SCANCODE_D)) {
		set_flip(SDL_FLIP_NONE);
		m_direction.x = 500.f;
	}
	else if (Keyboard::instance().is_key_pressed(SDL_SCANCODE_A)) {
		set_flip(SDL_FLIP_HORIZONTAL);
		m_direction.x = -500.f;
	}
	else {
		m_direction.x = 0.f;
	}

	if (get_pos().x <= 200.f && Keyboard::instance().is_key_pressed(SDL_SCANCODE_A)) {
		m_direction.x = 0.f;
		Camera::instance().set_offset(750.f, Camera::instance().get_offset().y);
		set_x(200.f);
	}
	else if (get_pos().x + get_size().x >= Game::instance().get_window_size().x / 2.f && Keyboard::instance().is_key_pressed(SDL_SCANCODE_D)) {
		m_direction.x = 0.f;
		Camera::instance().set_offset(-750.f, Camera::instance().get_offset().y);
		set_x(Game::instance().get_window_size().x / 2.f - get_size().x);
	}
	else {
		Camera::instance().set_offset(0.f, Camera::instance().get_offset().y);
	}

	move(m_direction.x, 0.f, dt);
}

void Mario::horizontal_collision(float dt)
{
	for (auto go = get_world()->begin(); go != get_world()->end(); ++go) {
		LevelObject* level_object = dynamic_cast<LevelObject*>(go->get());

		if (level_object != nullptr) {
			if (level_object->is_drawable()) {
				if (level_object->get_collide_type() != LevelObject::COLLIDE_TYPE::TOTAL) {

					if (SDL_HasIntersectionF(&level_object->get_transf(), &get_transf())) {

						if (dynamic_cast<Ennemy*>(level_object)) {
							kill();
							break;
						}
					}
				}
				else {
					if (SDL_HasIntersectionF(&level_object->get_transf(), &get_transf()) && !m_hit_head) {
						if (m_direction.x > 0.f) {
							set_x(level_object->get_pos().x - get_size().x);
							break;
						}
						else if (m_direction.x < 0.f) {
							set_x(level_object->get_pos().x + level_object->get_size().x);
							break;
						}
					}
				}
			}
		}
	}
}

void Mario::kill()
{
	if (m_state != MarioState::DECREASING) {
		if (m_state == MarioState::SMALL) {
			SoundManager::instance().play("die");
			m_state = MarioState::DEAD;
			eject();
			Mix_HaltChannel(0);
		}
		else if (m_state == MarioState::BIG) {
			m_state = MarioState::DECREASING;
			m_animator->set_animation("decrease");
		}
	}
}

void Mario::select_anim()
{
	if (fabs(m_direction.x) > 0.f) {
		if(m_state == MarioState::SMALL) m_animator->set_animation("small-walk");
		else if(m_state == MarioState::BIG) m_animator->set_animation("big-walk");
	}
	else if (m_direction.x == 0.f) {
		if (m_state == MarioState::SMALL) m_animator->set_animation("small-idle");
		else if (m_state == MarioState::BIG) m_animator->set_animation("big-idle");
	}

	if (m_falling) {
		if (m_direction.y < 0.f) {
			if (m_state == MarioState::SMALL) m_animator->set_animation("small-jumping");
			else if (m_state == MarioState::BIG) m_animator->set_animation("big-jumping");
		}
		else if (m_direction.y > 0.f) {
			if (m_state == MarioState::SMALL) m_animator->set_animation("small-falling");
			else if (m_state == MarioState::BIG) m_animator->set_animation("big-falling");
		}
	}
}

void Mario::on_floor(LevelObject* object)
{
	m_falling = false;
	m_direction.y = 0.f;
	set_y(object->get_pos().y - get_size().y);
}

void Mario::collect_item(float dt)
{
	for (auto go = get_world()->begin(); go != get_world()->end(); ++go) {
		Item* item = dynamic_cast<Item*>(go->get());

		if (item != nullptr) {
			if (item->is_drawable()) {
				if (SDL_HasIntersectionF(&get_transf(), &item->get_transf())) {
					item->on_hit(this);
					break;
				}
			}
		}
	}
}