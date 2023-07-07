#include "World.h"
#include "texturemanager.h"

World::World()
{
	add_gameobject<Level>();
	m_game_over = false;
}

void World::load_textures(SDL_Renderer* pRenderer)
{
	TextureManager::instance().get_texture("mario", "assets\\img\\mario.png", pRenderer);
	TextureManager::instance().get_texture("tiles", "assets\\img\\general-tiles.png", pRenderer);
	TextureManager::instance().get_texture("tileset", "assets\\img\\tiles.png", pRenderer);
	TextureManager::instance().get_texture("particles", "assets\\img\\particles.png", pRenderer);
	TextureManager::instance().get_texture("background", "assets\\img\\background.png", pRenderer);
	TextureManager::instance().get_texture("koopa", "assets\\img\\koopa.png", pRenderer);
}

void World::clear()
{
	for (auto& gameobject : m_gameobjects) {
		delete_gameobject(gameobject.get());
	}
}

#include "camera.h"
void World::update(float dt)
{
	for (auto& gameobject : m_gameobjects) {
	
		if (gameobject) {
			LevelObject* object = dynamic_cast<LevelObject*>(gameobject.get());

			if (object) {
				if (dynamic_cast<Mario*>(gameobject.get()) == nullptr) {
					object->move(Camera::instance().get_offset(), 0.f, dt);

					if (object->is_drawable())
						object->update(dt);
				}
				else {
					object->update(dt);
				}
			}
			else {
				gameobject->update(dt);
			}
		}
	}
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
}
