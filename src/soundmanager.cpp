#include "soundmanager.h"
#include "config.h"
#include "SDL.h"

SoundManager& SoundManager::instance()
{
	return s_instance;
}

void SoundManager::init()
{
	
}

#include <iostream>
void SoundManager::load_sound(const std::string& ID, const std::string& path)
{
	
	m_base_path = SDL_GetBasePath();
	std::string final_path = m_base_path + path;
	m_sounds[ID] = Mix_LoadWAV(final_path.c_str());
	std::cout << m_sounds["coin"] << std::endl;
	
}

void SoundManager::play(const std::string& ID, bool loop, int channel)
{
	Mix_PlayChannel(channel, m_sounds[ID], loop);
}

SoundManager SoundManager::s_instance;

SoundManager::SoundManager()
{
	
}
