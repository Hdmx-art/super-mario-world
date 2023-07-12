#include "winbar.h"
#include "texturemanager.h"

WinBar::WinBar(World* pWorld) : LevelObject(pWorld)
{
	m_rect = { 26,228,24,8 };
	set_size(128.f, 32.f);
}

void WinBar::update(float dt)
{
	move(0.f, m_vel, dt);

	if (get_pos().y >= m_init_pos.y + 400.f)
		m_vel *= -1.f;
	else if (get_pos().y <= m_init_pos.y)
		m_vel *= -1.f;
}

void WinBar::render(SDL_Renderer* pRenderer)
{
	SDL_RenderCopyF(pRenderer, TextureManager::instance().get_texture("tileset"),
		&m_rect, &get_transf());
}

void WinBar::set_init_pos(float x, float y)
{
	m_init_pos.x = x;
	m_init_pos.y = y;
}
