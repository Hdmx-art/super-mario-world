#include "camera.h"
#include <SDL_render.h>
#include "Game.h"

void Camera::debug_draw(SDL_Renderer* pRenderer)
{
	SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);
	SDL_FRect rect = {200.f, 100.f,Game::instance().get_window_size().x / 2.f - 200.f, Game::instance().get_window_size().y - 200.f };
	SDL_RenderDrawRectF(pRenderer, &rect);
}

Camera Camera::s_instance;