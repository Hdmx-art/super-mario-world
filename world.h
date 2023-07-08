#ifndef WORLD_H
#define WORLD_H

#include <SDL.h>
#include <deque>
#include <memory>
#include "mario.h"
#include "coin.h"
#include "level.h"

class World
{
public:
	World();

	void load_textures(SDL_Renderer* pRenderer);
	void clear();

	void update(float dt);
	void render(SDL_Renderer* pRenderer);
	
	template<class T>
	T* add_gameobject() {
		m_gameobjects.push_back(std::make_unique<T>(this));
		return dynamic_cast<T*>(m_gameobjects.back().get());
	}
	
	template<class T>
	T* add_gameobject_to_back() {
		m_gameobjects.emplace_front(std::make_unique<T>(this));
		return dynamic_cast<T*>(m_gameobjects.front().get());
	}

	void reset() {
		m_gameobjects.clear();
	}

	void delete_gameobject(GameObject* pGameObject) {
		for (auto go = begin(); go != end(); ++go) {
			if (go->get() == pGameObject) {
				
				(*go)->destroy();
				go->reset();
				m_gameobjects.erase(go);
				break;
			}
		}
	}

	std::deque<std::unique_ptr<GameObject>>::iterator begin() {
		return m_gameobjects.begin();
	}

	std::deque<std::unique_ptr<GameObject>>::iterator end() {
		return m_gameobjects.end();
	}

private:
	std::deque<std::unique_ptr<GameObject>> m_gameobjects;
	bool m_game_over;
};

#endif