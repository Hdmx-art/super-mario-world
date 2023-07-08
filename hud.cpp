#include "HUD.h"
#include "SDL_render.h"

HUD& HUD::instance()
{
	return s_instance;
}

#include "Game.h"
#include <iostream>
void HUD::init()
{
	TTF_Init();
	std::string path = Game::instance().get_basepath() + "assets\\fonts\\mario-font.ttf";
	m_font = TTF_OpenFont(path.c_str(), 24);

	if (!m_font) {
		std::cout << path << std::endl;
		BREAK();
	}
}

void HUD::draw_text(SDL_Renderer* pRenderer, const std::string& text, const Vector2f& pos, float char_size)
{
	SDL_Surface* surface = TTF_RenderText_Solid(m_font, text.c_str(), { 255,255,255 });
	SDL_Texture* texture = SDL_CreateTextureFromSurface(pRenderer, surface);

	if (!surface)
		BREAK();

	SDL_Surface* mask = TTF_RenderText_Solid(m_font, text.c_str(), { 0,0,0 });
	SDL_Texture* mask_texture = SDL_CreateTextureFromSurface(pRenderer, mask);

	SDL_FRect mask_rect = { pos.x,pos.y, char_size * text.size() / 1.5f,char_size + 5.f };
	SDL_RenderCopyF(pRenderer, mask_texture, nullptr, &mask_rect);

	SDL_FRect rect = { pos.x,pos.y, char_size * text.size() / 1.5f,char_size };
	SDL_RenderCopyF(pRenderer, texture, nullptr, &rect);
}

HUD HUD::s_instance;

HUD::HUD()
{
	m_font = nullptr;
}
