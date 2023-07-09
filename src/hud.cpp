#include "HUD.h"
#include "SDL_render.h"
#include "soundmanager.h"

HUD& HUD::instance()
{
	return s_instance;
}

void HUD::add_xp_text(float x, float y, int32_t number, const std::string& sound)
{
	SoundManager::instance().play(sound);
	m_texts.push_back({ {x,y} ,{x,y}, number });
}

void HUD::update(float dt)
{
	for (uint64_t i = 0; i < m_texts.size();i++) {
		
		m_texts[i].vel -= 70.f * dt;
		
		if (m_texts[i].vel > 30.f) {
			m_texts[i].pos.y -= m_texts[i].vel * dt;
		}
		else {
			m_texts.erase(m_texts.begin() + i);
		}
	}
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

	SDL_FRect mask_rect = { pos.x,pos.y, char_size * text.size() / 1.5f,char_size + 6.5f };
	SDL_RenderCopyF(pRenderer, mask_texture, nullptr, &mask_rect);

	SDL_FRect rect = { pos.x,pos.y, char_size * text.size() / 1.5f,char_size };
	SDL_RenderCopyF(pRenderer, texture, nullptr, &rect);
}

void HUD::render(SDL_Renderer* pRenderer)
{
	for (const auto& text : m_texts) {
		draw_text(pRenderer, std::to_string(text.number), text.pos, 30);
	}
}

HUD HUD::s_instance;

HUD::HUD()
{
	m_font = nullptr;
}
