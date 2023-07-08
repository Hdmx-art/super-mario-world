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

Mario::Mario(World* pWorld) : LevelObject(pWorld)
{
	set_size(64.f, 96.f);
	m_rect = { 0,0,18,32 };
	m_gravity = 0.f;
	m_falling = false;
	m_velocity = 500.f;
	m_hit_head = false;
	m_frozen = false;
	m_state = MarioState::SMALL;

	m_animator = add_component<AnimComponent>();
	m_animator->add_animation("small-idle", { {16,24}, {24,48}, 1, 0.3f,4, true });
	m_animator->add_animation("big-idle", { {48,48}, {8,576}, 1, 0.3f,0, true });

	m_animator->add_animation("small-walk", { {16,24}, {180,48}, 3, 0.15f,36, true });
	m_animator->add_animation("big-walk", { {48,48}, {164,576}, 3, 0.15f,4, true });
	
	m_animator->add_animation("small-jumping", { {16,24}, {76,112}, 1, 0.3f, 0, true });
	m_animator->add_animation("big-jumping", { {48,48}, {60,647}, 1, 0.3f, 0, true });

	m_animator->add_animation("small-falling", { {16,24}, {128,112}, 1, 0.3f, 0, true });
	m_animator->add_animation("big-falling", { {48,48}, {112,647}, 1, 0.3f, 0, true });

	m_animator->add_animation("grow", { {48,48}, {580,251}, 3, 0.5f, 4, false });
	m_animator->add_animation("die", { {16,24}, {752,44}, 1, 0.3f, 0, true });
}

void Mario::update(float dt)
{
	if (!is_ejected()) {
		if (m_animator->get_current_animation() != "grow") {
			vertical_movement(dt);
			vertical_collision(dt);

			horizontal_movement(dt);
			horizontal_collision(dt);

			collect_item(dt);
		}
		else if (m_animator->is_finished()) {
			if(m_state==MarioState::SMALL)
				m_animator->set_animation("small-idle");
			else
				m_animator->set_animation("big-idle");
		}

		if (m_state == MarioState::SMALL) {
			set_size(64.f, 96.f);
		}
		else {
			set_size(128.f, 156.f);
		}
	}

	m_animator->update(dt, &m_rect);

	if (is_ejected()) {
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
}

void Mario::bump()
{
	m_gravity = -1200.f;
}

void Mario::eject_process(World* pWorld, float velx, float dt)
{
	eject_gravity() += 2500.f * dt;
	move(0.f, eject_gravity(), dt);
	m_velocity = 0.f;

	if (get_pos().y > Game::instance().get_window_size().y) {
		pWorld->delete_gameobject(this);
		return;
	}
}

float Mario::get_vel()
{
	return m_velocity;
}

bool Mario::is_falling()
{
	return m_falling;
}

void Mario::grow()
{
	m_animator->set_animation("grow");
	m_state = MarioState::BIG;
}

#include <iostream>
void Mario::vertical_movement(float dt)
{
	m_gravity += 2000.f * dt;

	if (m_hit_head)
		m_gravity *= -1.f;

	if (Keyboard::instance().is_key_pressed(SDL_SCANCODE_SPACE) && m_falling == false) {
		bump();
		SoundManager::instance().play("jump");
		m_falling = true;
	}

	if (m_gravity > 0.f) {
		if (m_state == MarioState::SMALL)
			m_animator->set_animation("small-falling");
		else
			m_animator->set_animation("big-falling");
		m_falling = true;
	}
	else {
		if (m_state == MarioState::SMALL)
			m_animator->set_animation("small-jumping");
		else
			m_animator->set_animation("big-jumping");
	}

	if (get_pos().y <= 100.f) {
		Camera::instance().set_offset(Camera::instance().get_offset().x, -m_gravity);
		set_y(100.f);
	}
	else if (get_pos().y + get_size().y >= Game::instance().get_window_size().y - 100.f) {

		Camera::instance().set_offset(Camera::instance().get_offset().x, -m_gravity);
		set_y(Game::instance().get_window_size().y - 100.f - get_size().y);
	}
	else {
		Camera::instance().set_offset(Camera::instance().get_offset().x, 0.f);
	}

	move(0, m_gravity, dt);
}

void Mario::vertical_collision(float dt)
{
	m_hit_head = false;

	for (auto go = get_world()->begin(); go != get_world()->end(); ++go) {
		LevelObject* level_object = dynamic_cast<LevelObject*>(go->get());

		if (level_object != nullptr) {
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
						
						if (get_pos().y < level_object->get_pos().y && m_gravity > 0.f)
							on_floor(level_object);
					}
					else if (level_object->get_collide_type() == LevelObject::COLLIDE_TYPE::TOTAL) {
						
						if (get_pos().y < level_object->get_pos().y)
							on_floor(level_object);
						else if (get_pos().y > level_object->get_size().y) {
							m_hit_head = true;

							if (dynamic_cast<MysterBlock*>(level_object)) {
								MysterBlock* myster_block = dynamic_cast<MysterBlock*>(level_object);
								myster_block->on_hit();
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
		set_flip(SDL_FLIP_HORIZONTAL);
		move(m_velocity, 0.f, dt);

		if (!m_falling) {
			if(m_state == MarioState::SMALL)
				m_animator->set_animation("small-walk");
			else
				m_animator->set_animation("big-walk");
		}
	}
	else if (Keyboard::instance().is_key_pressed(SDL_SCANCODE_A)) {
		set_flip(SDL_FLIP_NONE);
		move(-m_velocity, 0.f, dt);

		if (!m_falling) {
			if (m_state == MarioState::SMALL)
				m_animator->set_animation("small-walk");
			else
				m_animator->set_animation("big-walk");
		}
	}

	if (get_pos().x <= 200.f && Keyboard::instance().is_key_pressed(SDL_SCANCODE_A)) {
		m_velocity = 0.f;
		Camera::instance().set_offset(750.f, Camera::instance().get_offset().y);
		set_x(200.f);
	}
	else if (get_pos().x + get_size().x >= Game::instance().get_window_size().x / 2.f && Keyboard::instance().is_key_pressed(SDL_SCANCODE_D)) {
		m_velocity = 0.f;
		Camera::instance().set_offset(-750.f, Camera::instance().get_offset().y);
		set_x(Game::instance().get_window_size().x / 2.f - get_size().x);
	}
	else {
		m_velocity = 500.f;
		Camera::instance().set_offset(0.f, Camera::instance().get_offset().y);
	}
}

void Mario::horizontal_collision(float dt)
{
	for (auto go = get_world()->begin(); go != get_world()->end(); ++go) {
		LevelObject* level_object = dynamic_cast<LevelObject*>(go->get());

		if (level_object != nullptr) {
			if (level_object->get_collide_type() != LevelObject::COLLIDE_TYPE::TOTAL) {
				
				if (SDL_HasIntersectionF(&level_object->get_transf(), &get_transf())) {
					
					if (dynamic_cast<Ennemy*>(level_object) && !m_falling) {

						if (dynamic_cast<Shell*>(level_object)) {
							Shell* shell = dynamic_cast<Shell*>(level_object);
							if (fabs(shell->get_vel()) == 0.f) {
								break;
							}
						}
						else if (dynamic_cast<Koopa*>(level_object)) {
							Koopa* koopa = dynamic_cast<Koopa*>(level_object);
							if (!koopa->is_kicked()) {
								if (get_pos().y < koopa->get_pos().y)
									break;
							}
						}

						eject();
						m_animator->set_animation("die");
						SoundManager::instance().play("die");
						Mix_HaltChannel(0);
						break;
					}
				}
			}
			else {
				if (SDL_HasIntersectionF(&level_object->get_transf(), &get_transf())) {
					if (Keyboard::instance().is_key_pressed(SDL_SCANCODE_D))
						move(-m_velocity, 0.f, dt);
					else if (Keyboard::instance().is_key_pressed(SDL_SCANCODE_A))
						move(m_velocity, 0.f, dt);
				}
			}
		}
	}
}

void Mario::on_floor(LevelObject* object)
{
	m_falling = false;
	if (m_state == MarioState::SMALL)
		m_animator->set_animation("small-idle");
	else
		m_animator->set_animation("big-idle");

	m_gravity = 0.f;
	set_y(object->get_pos().y - get_size().y);
}

void Mario::collect_item(float dt)
{
	for (auto go = get_world()->begin(); go != get_world()->end(); ++go) {
		Item* item = dynamic_cast<Item*>(go->get());

		if (item != nullptr) {
			if (SDL_HasIntersectionF(&get_transf(), &item->get_transf())) {
				item->on_hit(this);
				break;
			}
		}
	}
}