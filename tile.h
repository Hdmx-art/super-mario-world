#ifndef TILE_H
#define TILE_H

#include "levelobject.h"

constexpr auto TILE_SIZE = 64.f;

class BasicTile : public LevelObject
{
public:
	BasicTile(World* pWorld) : LevelObject(pWorld) {
		m_rect = { 0,0,16,16 };
		set_size(65.f, 64.f);
	}

	virtual void render(SDL_Renderer* pRenderer) {
		SDL_RenderCopyF(pRenderer, TextureManager::instance().get_texture("tileset"), &m_rect, &get_transf());
	}

	void set_rect(const Vector2i& pos) {
		m_rect.x = pos.x;
		m_rect.y = pos.y;
	}

private:
	SDL_Rect m_rect;
};

#endif