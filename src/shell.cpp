#include "shell.h"
#include "world.h"
#include "koopa.h"
#include "mysterblock.h"
#include "Game.h"
#include "soundmanager.h"
#include "all_particles.h"
#include "hud.h"

Shell::Shell(World* pWorld) : Ennemy(pWorld)
{
	m_animator = add_component<AnimComponent>();
	m_rect = {};
	set_size(64.f, 64.f);
	m_vel = 0.f;
	m_falling = true;
	m_gravity = 0.f;

	m_animator->add_animation("on_spawn", { {16,16},{32,208 },3,0.2f,0,false });
	m_animator->add_animation("sliding", { {16,16},{32,256 },4,0.4f,0,true });
	m_animator->add_animation("idle", { {16,16},{32,256 },1,0.4f,0,true });
	m_animator->set_animation("on_spawn");
	m_animator->update(0.f, &m_rect);
}

void Shell::spawn(float x, float y, Color color)
{
	m_animator->set_animation("on_spawn");
	set_pos(x, y);
}

float Shell::get_vel()
{
	return m_vel;
}

void Shell::update(float dt)
{
	m_animator->update(dt, &m_rect);

	if (!is_ejected()) {
		
		move(m_vel, 0.f, dt);
		update_physics(dt);
		
		if (m_vel < -MAX_VEL)
			m_vel = -MAX_VEL;
		else if (m_vel > MAX_VEL)
			m_vel = MAX_VEL;

		for (auto go = get_world()->begin(); go != get_world()->end(); ++go) {
			LevelObject* level_object = dynamic_cast<LevelObject*>(go->get());

			if (level_object != nullptr) {
				if (!level_object->is_ejected() && level_object->is_drawable()) {
					SDL_FRect rect = {};
					if (SDL_IntersectFRect(&get_transf(), &level_object->get_transf(), &rect)) {

						if (dynamic_cast<Mario*>(level_object)) {
							Mario* mario = dynamic_cast<Mario*>(level_object);

							if (mario->is_falling()) {
								if (fabs(m_vel) == MAX_VEL) {
									m_vel = 0.f;
									mario->bump();

									StompParticle* particle = get_world()->add_gameobject<StompParticle>();
									particle->spawn(rect.x, rect.y - 24.f, 24.f, 24.f);
									break;
								}
								else {
									if (mario->get_pos().x <= get_pos().x)
										m_vel = MAX_VEL * 3;
									else if (mario->get_pos().x >= get_pos().x)
										m_vel = -MAX_VEL * 3;

									StompParticle* particle = get_world()->add_gameobject<StompParticle>();
									particle->spawn(rect.x, rect.y - 24.f, 24.f, 24.f);
									break;
								}
							}
							else {
								if (fabs(m_vel) == MAX_VEL) {
									mario->eject();

									StompParticle* particle = get_world()->add_gameobject<StompParticle>();
									particle->spawn(rect.x, rect.y - 24.f, 24.f, 24.f);
									break;
								}
								else {
									if (mario->get_pos().x <= get_pos().x)
										m_vel = MAX_VEL * 3;
									else if (mario->get_pos().x >= get_pos().x)
										m_vel = -MAX_VEL * 3;

									StompParticle* particle = get_world()->add_gameobject<StompParticle>();
									particle->spawn(rect.x, rect.y - 24.f, 24.f, 24.f);
									break;
								}
							}

							
						}
						else if (dynamic_cast<BasicTile*>(go->get())) {
							BasicTile* tile = dynamic_cast<BasicTile*>(go->get());

							if (tile->get_collide_type() == LevelObject::COLLIDE_TYPE::TOTAL) {
								if (get_pos().y + get_size().y - 5.f >= tile->get_pos().y) {
									m_vel *= -1.f;
								}
							}
						}
						else if (dynamic_cast<MysterBlock*>(go->get())) {
							MysterBlock* myster_block = dynamic_cast<MysterBlock*>(go->get());
							if (get_pos().y >= myster_block->get_pos().y) {
								m_vel *= -3.f;
							}
						}
						else if (dynamic_cast<Ennemy*>(go->get())) {
							Ennemy* ennemy = dynamic_cast<Ennemy*>(go->get());

							if (dynamic_cast<Koopa*>(go->get())) {
								Koopa* koopa = dynamic_cast<Koopa*>(go->get());
								if (!koopa->sliding() && abs(m_vel) > 0.f) {
									koopa->eject();
									HUD::instance().add_xp_text(koopa->get_pos().x, koopa->get_pos().y, 1000, "stomp");
									break;
								}
							}
							else if (dynamic_cast<Shell*>(go->get()) && go->get() != this) {
								Shell* shell = dynamic_cast<Shell*>(go->get());

								if (fabs(m_vel) == fabs(shell->get_vel())) {
									eject();
									HUD::instance().add_xp_text(get_pos().x, get_pos().y, 500, "stomp");

									shell->eject();
									HUD::instance().add_xp_text(shell->get_pos().x, shell->get_pos().y, 500, "stomp");

									if (m_vel > 0.f)
										inverse_eject_vel();
									else if (shell->m_vel > 0.f)
										inverse_eject_vel();

									break;
								}
								else if (fabs(m_vel) > fabs(shell->get_vel())) {
									shell->eject();
									HUD::instance().add_xp_text(get_pos().x, get_pos().y, 500, "stomp");
									break;
								}
							}

							if (fabs(m_vel) > 0.f && go->get() != this) {
								StompParticle* particle = get_world()->add_gameobject<StompParticle>();
								particle->spawn(rect.x, rect.y, 24.f, 24.f);
								break;
							}
						}
					}
				}
			}
		}
	}
	else {
		eject_process(get_world(), 550.f, dt);
	}
}

void Shell::render(SDL_Renderer* pRenderer)
{
	SDL_RenderCopyExF(pRenderer, TextureManager::instance().get_texture("koopa"), &m_rect, &get_transf(),
		0.f, &get_origin(), get_flip());
}

void Shell::update_physics(float dt)
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
