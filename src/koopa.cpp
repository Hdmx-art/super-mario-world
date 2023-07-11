#include "koopa.h"
#include "texturemanager.h"
#include "mario.h"
#include "world.h"
#include "shell.h"
#include "Game.h"
#include "soundmanager.h"
#include "hud.h"

Koopa::Koopa(World* pWorld) : Ennemy(pWorld)
{
	m_animator = add_component<AnimComponent>();
	m_jumped_on = false;
	m_kicked_out_of_shell = false;
	m_is_sliding = false;
	m_kicked_pos = { 0.f,0.f };
	m_shell = nullptr;
	m_falling = false;
	m_gravity = 0.f;

	set_size(64.f, 64.f);
	m_rect = { 32,149,16,27 };
	m_vel = -100.f;

	set_flip(SDL_FLIP_NONE);

	m_animator->add_animation("move", { {16,27},{32,149},2,0.4f,0,true });
	m_animator->add_animation("kicked", { {16,16},{32,432},2,0.75f,0,true });
	m_animator->add_animation("dead", { {16,8},{32,472},1,0.75f,0,false });
	m_animator->set_animation("move");
}

void Koopa::update(float dt)
{
	m_animator->update(dt, &m_rect);

	if (!is_ejected()) {
		update_physics(dt);

		if (m_jumped_on) {
			on_jumped(dt);
		}
		else if (m_kicked_out_of_shell) {
			on_kicked(dt);
		}
		else {
			movement(dt);
		}
	}
	else {
		eject_process(get_world(), 550.f, dt);
	}
}

void Koopa::render(SDL_Renderer* pRenderer)
{
	SDL_RenderCopyExF(pRenderer, TextureManager::instance().get_texture("koopa"),
		&m_rect, &get_transf(), get_rotation(), &get_origin(), get_flip());
}

void Koopa::on_hit(Mario* pMario)
{
	if (m_kicked_out_of_shell == false) {
		m_kicked_out_of_shell = true;
		jump_mario(pMario, "kicked");
		move(0.f, get_size().y / 2.5f, 1.f);
		set_flip(SDL_FLIP_HORIZONTAL);
		m_vel = 800.f;
		m_is_sliding = true;

		Shell* shell = get_world()->add_gameobject<Shell>();
		shell->spawn(get_pos().x, get_pos().y, Shell::Color::RED);
	}
	else if (m_jumped_on == false) {
		m_jumped_on = true;
		move(0.f, get_size().y / 2.f, 1.f);
		jump_mario(pMario, "dead");
		HUD::instance().add_xp_text(get_pos().x, get_pos().y, 1000, 26);
	}
}

bool Koopa::sliding()
{
	return m_is_sliding;
}

bool Koopa::is_kicked()
{
	return m_kicked_out_of_shell;
}

void Koopa::jump_mario(Mario* pMario, const std::string& ID)
{
	if (pMario) pMario->bump();
	m_animator->set_animation(ID);
}

void Koopa::on_jumped(float dt)
{
	set_size(64.f, 32.f);

	if (m_animator->is_finished()) {
		get_world()->delete_gameobject(this);
		return;
	}
}

void Koopa::on_kicked(float dt)
{
	set_size(64.f, 64.f);

	m_vel += 1500.f * dt;

	if (m_vel <= 1100.f && m_is_sliding) {

		for (auto go = get_world()->begin(); go != get_world()->end(); ++go) {
			LevelObject* level_object = dynamic_cast<LevelObject*>(go->get());

			if (level_object != nullptr) {
				if (level_object->is_drawable()) {
					if (level_object->get_collide_type() == LevelObject::COLLIDE_TYPE::TOTAL) {
						if (SDL_HasIntersectionF(&get_transf(), &level_object->get_transf())) {
							if (get_pos().y >= level_object->get_pos().y) {
								m_is_sliding = false;
								set_x(level_object->get_pos().x - get_size().x - 10.f);
								break;
							}
						}
					}
				}
			}
		}

		move(m_vel, 0.f, dt);
	}
	else
		m_is_sliding = false;
}

void Koopa::movement(float dt)
{
	set_size(64.f, 96.f);
	move(m_vel, 0.f, dt);

	for (auto go = get_world()->begin(); go != get_world()->end(); ++go) {
		LevelObject* level_object = dynamic_cast<LevelObject*>(go->get());

		if (level_object != nullptr) {
			if (level_object->is_drawable()) {
				if (level_object->get_collide_type() == LevelObject::COLLIDE_TYPE::TOTAL) {
					if (SDL_HasIntersectionF(&get_transf(), &level_object->get_transf())) {
						m_vel *= -1;

						if (get_flip() == SDL_FLIP_NONE)
							set_flip(SDL_FLIP_HORIZONTAL);
						else if (get_flip() == SDL_FLIP_HORIZONTAL)
							set_flip(SDL_FLIP_NONE);

						break;
					}
				}
			}
		}
	}
}

void Koopa::update_physics(float dt)
{
	m_gravity += 2000.f * dt;

	if (m_gravity > 0.f)
		m_falling = true;

	move(0, m_gravity, dt);

	for (auto go = get_world()->begin(); go != get_world()->end(); ++go) {
		LevelObject* level_object = dynamic_cast<LevelObject*>(go->get());

		if (level_object) {
			if (level_object->is_drawable()) {
				if (level_object->get_collide_type() != LevelObject::COLLIDE_TYPE::NONE) {
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
}
