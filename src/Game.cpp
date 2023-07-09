#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include "texturemanager.h"
#include "hud.h"
#include "soundmanager.h"

Game Game::s_instance;

#include <iostream>

void Game::run()
{
	init();
	srand((unsigned)time(nullptr));

	std::cout << "Launch time : " << float(SDL_GetTicks64() - t1) / 1000.f << std::endl;

	while (s_instance.m_running) {

		if (SDL_GetTicks64() > m_time_passed + FRAME_TIME_144 * 1000) {

			update_events();
			update(m_delta_time);
			render();

			m_delta_time = static_cast<float>(SDL_GetTicks64() - m_time_passed)/1000.f;
			m_time_passed = SDL_GetTicks64();
		}
	}

	destroy();
}

const Vector2i Game::get_window_size()
{
	Vector2i size{};
	SDL_GetWindowSize(m_window, &size.x, &size.y);
	return size;
}

const std::string& Game::get_basepath()
{
	return m_basePath;
}

Game::Game()
{
	t1 = SDL_GetTicks64();

	m_window = nullptr;
	m_renderer = nullptr;
	m_running = false;
	m_time_passed = 1000;
	m_delta_time = 1.f / 144.f;

	SDL_CHECK(SDL_Init(SDL_INIT_EVERYTHING));
	int32_t flag = IMG_Init(IMG_INIT_PNG);

	if ((flag & IMG_INIT_PNG) == false)
		BREAK();

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

	m_basePath = SDL_GetBasePath();

	HUD::instance().init();
	SoundManager::instance().init();
}

void Game::init()
{
	SDL_CHECK(SDL_CreateWindowAndRenderer(1920, 1080, SDL_WINDOW_SHOWN, &m_window, &m_renderer));
	SDL_SetWindowTitle(m_window, "NewSuperMarioBros");

	std::string path = m_basePath + "assets\\img\\icon.png";

	SDL_Surface *surface = IMG_Load(path.c_str());
	SDL_SetWindowIcon(m_window, surface);
	SDL_FreeSurface(surface);
	
	m_running = true;

	m_world = new World();
	m_world->load_textures(m_renderer);
}

void Game::destroy()
{
	delete m_world;
	TextureManager::instance().clear();

	TTF_Quit();
	IMG_Quit();

	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Game::update_events()
{
	SDL_Event event{};
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			m_running = false;
	}
}

void Game::update(float dt)
{
	m_world->update(dt);
}

void Game::render()
{
	SDL_CHECK(SDL_SetRenderDrawColor(m_renderer, 57, 102, 0, 255));
	SDL_CHECK(SDL_RenderClear(m_renderer));
	
	m_world->render(m_renderer);
	std::string text = std::to_string(int(1.f / m_delta_time));
	HUD::instance().draw_text(m_renderer, text + " FPS", {0.f,0.f}, 48);

	SDL_RenderPresent(m_renderer);
}
