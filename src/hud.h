#ifndef HUD_H
#define HUD_H

#include "config.h"
#include "rects.h"
#include <SDL_ttf.h>
#include <vector>

class HUD
{
public:
	static HUD& instance();

	void add_xp_text(float x, float y, int32_t number, uint32_t size, const std::string& sound="none");
	void update(float dt);

	void init();
	void draw_number(SDL_Renderer*pRenderer, std::string text, const Vector2f& pos, float char_size);
	void draw_number_xp(SDL_Renderer*pRenderer, uint32_t xp, const Vector2f& pos, float char_size);
	
	void render(SDL_Renderer* pRenderer);

	void add_xp(uint32_t xp);

private:
	void draw_time(SDL_Renderer* pRenderer, std::string text, const Vector2f& pos, float char_size);

	struct TextProperties {
		Vector2f pos;
		Vector2f init_pos;
		int32_t number;
		uint32_t size;
		float vel = 100.f;
	};
	
	struct Icon {
		SDL_FRect transf;
		SDL_Rect rect;
	};

	static HUD s_instance;
	HUD();

	Icon m_center_square;
	Icon m_time;
	Icon m_coin;

	uint32_t m_total_xp;

	std::vector<TextProperties> m_texts;
	TTF_Font* m_font;
};

#endif