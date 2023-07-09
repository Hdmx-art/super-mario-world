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

	void add_xp_text(float x, float y, int32_t number, const std::string& sound="none");
	void update(float dt);

	void init();
	void draw_text(SDL_Renderer*pRenderer, const std::string& text, const Vector2f& pos, float char_size);
	
	void render(SDL_Renderer* pRenderer);

private:

	struct TextProperties {
		Vector2f pos;
		Vector2f init_pos;
		int32_t number;
		float vel = 100.f;
	};

	static HUD s_instance;
	HUD();

	std::vector<TextProperties> m_texts;
	TTF_Font* m_font;
};

#endif