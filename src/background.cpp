#include "background.h"
#include "Game.h"

Background::Background(World* pWorld) : LevelObject(pWorld)
{
	set_size(Game::instance().get_window_size().y * 1.75f * 1.2f, Game::instance().get_window_size().y * 1.75f);
	set_pos(0.f, Game::instance().get_window_size().y - get_size().y);
	m_rect = { 0,0,512,432 };
}

void Background::set_rect(const Vector2i& pos)
{
	m_rect.x = pos.x;
	m_rect.y = pos.y;
}

void Background::update(float dt)
{

}

void Background::render(SDL_Renderer* pRenderer)
{
	SDL_RenderCopyF(pRenderer, TextureManager::instance().get_texture("background"), &m_rect, &get_transf());
}
