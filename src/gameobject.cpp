#include "GameObject.h"
#include "world.h"

GameObject::GameObject(World* pWorld)
{
	m_world_instance = pWorld;
}

GameObject::~GameObject()
{

}

void GameObject::destroy()
{
	for (auto& comp : m_components) 
		delete comp;
	
	m_components.clear();
}

void GameObject::update(float dt)
{

}

void GameObject::render(SDL_Renderer* pRenderer)
{

}
