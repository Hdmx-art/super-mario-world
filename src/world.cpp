#include "World.h"
#include "texturemanager.h"
#include "soundmanager.h"
#include "hud.h"
#include <ctime>
#include <algorithm>

World::World()
{
	m_level = add_gameobject<Level>();
	m_game_over = false;
	m_init_time = 0;
	m_delay = 0;
}

void World::load_textures(SDL_Renderer* pRenderer)
{
	TextureManager::instance().get_texture("mario", "assets\\img\\mario.png", pRenderer);
	TextureManager::instance().get_texture("tiles", "assets\\img\\general-tiles.png", pRenderer);
	TextureManager::instance().get_texture("tileset", "assets\\img\\tiles.png", pRenderer);
	TextureManager::instance().get_texture("particles", "assets\\img\\particles.png", pRenderer);
	TextureManager::instance().get_texture("background", "assets\\img\\background.png", pRenderer);
	TextureManager::instance().get_texture("koopa", "assets\\img\\koopa.png", pRenderer);
	TextureManager::instance().get_texture("enemies", "assets\\img\\enemies.png", pRenderer);
	TextureManager::instance().get_texture("hud", "assets\\img\\hud.png", pRenderer);

	SoundManager::instance().load_sound("title", "assets\\sounds\\title.wav");
	SoundManager::instance().load_sound("die", "assets\\sounds\\die.wav");
	SoundManager::instance().load_sound("coin", "assets\\sounds\\coin-collected.wav");
	SoundManager::instance().load_sound("kick", "assets\\sounds\\kick.wav");
	SoundManager::instance().load_sound("jump", "assets\\sounds\\jump.wav");
	SoundManager::instance().load_sound("stomp", "assets\\sounds\\stomp.wav");
	SoundManager::instance().load_sound("grow", "assets\\sounds\\grow.wav");
	SoundManager::instance().load_sound("clear", "assets\\sounds\\course_clear.wav");
	SoundManager::instance().load_sound("thunder", "assets\\sounds\\thunder.wav");
	SoundManager::instance().load_sound("shatter", "assets\\sounds\\shatter.wav");

	m_level->load_level();
}

void World::clear()
{
	auto go = m_gameobjects.begin();
	while (go != m_gameobjects.end()) {
		if (*go) {
			(*go)->destroy();
			go->reset();
			go = m_gameobjects.erase(go);
		}
		else {
			++go;
		}
	}
}

void World::game_over()
{
	m_game_over = true;
}

#include "camera.h"
#include <iostream>
void World::update(float dt)
{
	std::cout << m_gameobjects.size() << std::endl;
	for (auto& gameobject : m_gameobjects) {

		if (gameobject) {

			if (dynamic_cast<Mario*>(gameobject.get()) != nullptr) {
				gameobject->update(dt);
			}
			else if (SDL_GetTicks() > m_init_time + m_delay) {
				LevelObject* object = dynamic_cast<LevelObject*>(gameobject.get());

				if (object) {
					object->move(Camera::instance().get_offset().x, Camera::instance().get_offset().y, dt);

					if (object->is_drawable())
						object->update(dt);
				}
				else {
					gameobject->update(dt);
				}
			}
		}
	}

	HUD::instance().update(dt);
}

#include "Game.h"
void World::render(SDL_Renderer* pRenderer)
{
	for (auto& gameobject : m_gameobjects) {

		if(gameobject) {
			LevelObject* object = dynamic_cast<LevelObject*>(gameobject.get());
			if (object) {
				object->set_drawable(false);

				if (object->get_pos().x + object->get_size().x >= 0.f
					&& object->get_pos().x < Game::instance().get_window_size().x) {

					object->set_drawable(true);
					object->render(pRenderer);
				}

				//object->get_component<TransformComponent>()->debug_draw(pRenderer);
			}
		}
	}

	HUD::instance().render(pRenderer);

	if (m_game_over || m_win) {

		SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, alpha);
		SDL_RenderFillRectF(pRenderer, nullptr);

		if (alpha < 255)
			alpha += 1;
		else {
			if (m_time == 0) m_time = SDL_GetTicks64();

			if (SDL_GetTicks64() > m_time + 1500) {
				clear();

				if (m_win) {
					HUD::instance().draw_text(pRenderer, "COURSE CLEAR", 
						{ Game::instance().get_window_size().x / 2.f - 208.f, 150.f }, 48);
					HUD::instance().draw_text(pRenderer, "Thanks for playing ! (click to exit)",
						{ Game::instance().get_window_size().x / 2.f - 592.f, 450.f }, 48);

					Vector2i mouse_pos;
					uint32_t click = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

					SDL_Rect r = { Game::instance().get_window_size().x / 2 - 592,450,(37 * 48)/1.5f,48, };
					if (SDL_PointInRect(&mouse_pos, &r) && click == SDL_BUTTON_LEFT) {
						Game::instance().close();
					}

					return;
				}

				m_level = add_gameobject<Level>();
				m_level->load_level();
				m_game_over = false;
				HUD::instance().reset();
				m_time = 0;
				alpha = 0;
			}
		}
	}
}

void World::set_pause_for(uint64_t milliseconds)
{
	m_init_time = SDL_GetTicks();
	m_delay = milliseconds;
}

Level* World::get_level()
{
	return m_level;
}
