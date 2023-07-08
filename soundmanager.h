#ifndef SOUND_H
#define SOUND_H

#include <SDL_mixer.h>
#include <string>
#include <unordered_map>

class SoundManager
{
public:
	static SoundManager& instance();

	void init();

	void load_sound(const std::string& ID, const std::string& path);

	void play(const std::string& ID, bool loop = false, int channel = -1);

private:
	static SoundManager s_instance;
	SoundManager();

	std::string m_base_path;

	std::unordered_map<std::string, Mix_Chunk*> m_sounds;
};

#endif