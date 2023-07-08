#ifndef GAME_H
#define GAME_H

#include "config.h"
#include "rects.h"
#include "mario.h"
#include "world.h"

struct SDL_Window;
struct SDL_Renderer;

class Game
{
public:
	void run();

	const Vector2i get_window_size();
	const std::string& get_basepath();

	const uint32_t get_frame_rate() {
		return 144;
	}

	static Game& instance() {
		return s_instance;
	}

private:
	static Game s_instance;

	Game();

	void init();
	void destroy();

	void update_events();
	void update(float dt);
	void render();

	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	bool m_running;
	float m_delta_time;
	std::string m_basePath;

	uint64_t t1;
	World *m_world;
	uint64_t m_time_passed;
};

#endif