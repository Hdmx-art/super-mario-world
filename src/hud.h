#ifndef HUD_H
#define HUD_H

#include "config.h"
#include "rects.h"
#include <SDL_ttf.h>

class HUD
{
public:
	static HUD& instance();

	void init();
	void draw_text(SDL_Renderer*pRenderer, const std::string& text, const Vector2f& pos, float char_size);

private:
	static HUD s_instance;
	HUD();

	TTF_Font* m_font;
};

#endif