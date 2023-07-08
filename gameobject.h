#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <unordered_set>
#include "components.h"

class World;

class GameObject
{
public:
	GameObject(World* pWorld);
	virtual ~GameObject();

	void destroy();
	
	virtual void update(float dt);
	virtual void render(SDL_Renderer* pRenderer);

	template<class T>
	T* add_component() {
		T* new_component = new T;
		m_components.emplace(new_component);
		return new_component;
	}

	template<class T>
	T* get_component() {
		for (auto& comp : m_components) {
			if (dynamic_cast<T*>(comp))
				return dynamic_cast<T*>(comp);
		}

		return nullptr;
	}

	World* get_world() {
		return m_world_instance;
	}

private:
	std::unordered_set<Component*> m_components;
	World* m_world_instance;
};

#endif