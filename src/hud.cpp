#include "HUD.h"
#include "SDL_render.h"
#include "soundmanager.h"
#include "texturemanager.h"

HUD& HUD::instance()
{
	return s_instance;
}

void HUD::add_xp_text(float x, float y, int32_t number, uint32_t size, const std::string& sound)
{
	add_xp(number);
	SoundManager::instance().play(sound);
	m_texts.push_back({ {x,y} ,{x,y}, number, size });
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

	m_time_elapsed += dt;
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

	m_center_square.transf = { 0.f, 30.f, 150.f, 150.f };
	m_center_square.transf.x = Game::instance().get_window_size().x / 2.f - m_center_square.transf.w / 2.f;
	m_center_square.rect = { 115,10,28,28 };

	m_coin.transf = { Game::instance().get_window_size().x - 350.f,m_center_square.transf.y + m_center_square.transf.h / 4.f, 100.f, 50.f };
	m_coin.rect = { 201,16,16,8 };

	m_time.transf = { m_center_square.transf.x - 250.f, m_center_square.transf.y, 156.f, 48.f };
	m_time.rect = { 153,16,24,7 };
}

void HUD::draw_number(SDL_Renderer* pRenderer, std::string text, const Vector2f& pos, float char_size)
{
	for (int i = 0; i < text.length(); i++) {
		int32_t number = text[i] - '0';
		SDL_FRect number_rect = { pos.x + i * char_size, pos.y, char_size, char_size };
		SDL_Rect rect = { 258 + number * 9, 11, 8, 7 };

		SDL_RenderCopyF(pRenderer, TextureManager::instance().get_texture("hud"), &rect, &number_rect);
	}
}

void HUD::draw_number_xp(SDL_Renderer* pRenderer, uint32_t xp, const Vector2f& pos, float char_size)
{
	SDL_FRect transf = { pos.x, pos.y,0.f,0.f };
	SDL_Rect rect;

	if (xp == 100) {
		rect = { 259,180,11,7 };
		transf.w = ((float)char_size / 1.5f) * 3;
		transf.h = char_size;
	}
	else if (xp == 200) {
		rect = { 271,180,11,7 };
		transf.w = ((float)char_size / 1.5f) * 3;
		transf.h = char_size;
	}
	else if (xp == 400) {
		rect = { 284,180,11,7 };
		transf.w = ((float)char_size / 1.5f) * 3;
		transf.h = char_size;
	}
	else if (xp == 800) {
		rect = { 297,180,11,7 };
		transf.w = ((float)char_size / 1.5f) * 3;
		transf.h = char_size;
	}
	else if (xp == 1000) {
		rect = { 311,180,15,7 };
		transf.w = ((float)char_size / 1.7f) * 4;
		transf.h = char_size;
	}
	else if (xp == 2000) {
		rect = { 327,180,15,7 };
		transf.w = ((float)char_size / 1.6f) * 4;
		transf.h = char_size;
	}
	else if (xp == 4000) {
		rect = { 344,180,15,7 };
		transf.w = ((float)char_size / 1.6f) * 4;
		transf.h = char_size;
	}
	else if (xp == 8000) {
		rect = { 361,180,15,7 };
		transf.w = ((float)char_size / 1.5f) * 4;
		transf.h = char_size;
	}

	SDL_RenderCopyF(pRenderer, TextureManager::instance().get_texture("hud"), &rect, &transf);
}

void HUD::draw_text(SDL_Renderer* pRenderer, std::string text, const Vector2f& pos, float char_size)
{
	SDL_Surface* surface = TTF_RenderText_Solid(m_font, text.c_str(), { 255,255,255 });
	SDL_Texture* texture = SDL_CreateTextureFromSurface(pRenderer, surface);

	SDL_FRect rect = { pos.x,pos.y,char_size * text.length() / 1.5f,char_size };
	SDL_RenderCopyF(pRenderer, texture, nullptr, &rect);
}

void HUD::render(SDL_Renderer* pRenderer)
{
	for (const auto& text : m_texts) {
		draw_number_xp(pRenderer, text.number, text.pos, (float)text.size);
	}

	draw_time(pRenderer, std::to_string(500 - int(m_time_elapsed)), { m_time.transf.x, m_time.transf.y + m_time.transf.h + 15.f }, 40);
	
	draw_number(pRenderer, std::to_string(m_total_xp), { m_coin.transf.x, m_coin.transf.y + m_coin.transf.y + 15.f }, 40);

	SDL_RenderCopyF(pRenderer, TextureManager::instance().get_texture("hud"), &m_center_square.rect, &m_center_square.transf);
	SDL_RenderCopyF(pRenderer, TextureManager::instance().get_texture("hud"), &m_coin.rect, &m_coin.transf);
	SDL_RenderCopyF(pRenderer, TextureManager::instance().get_texture("hud"), &m_time.rect, &m_time.transf);
}

void HUD::add_xp(uint32_t xp)
{
	m_total_xp += xp;
}

void HUD::reset()
{
	m_time_elapsed = 0;
	m_total_xp = 0;
}

void HUD::draw_time(SDL_Renderer* pRenderer, std::string text, const Vector2f& pos, float char_size)
{
	for (int i = 0; i < text.length(); i++) {
		int32_t number = text[i] - '0';
		SDL_FRect number_rect = { pos.x + i * char_size, pos.y, char_size, char_size };
		SDL_Rect rect = { 258 + number * 9, 20, 8, 7 };

		SDL_RenderCopyF(pRenderer, TextureManager::instance().get_texture("hud"), &rect, &number_rect);
	}
}

HUD HUD::s_instance;

HUD::HUD()
{
	m_font = nullptr;
}
