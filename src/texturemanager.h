#ifndef TEXTURE_H
#define TEXTURE_H

#include <unordered_map>
#include <string>

struct SDL_Texture;
struct SDL_Renderer;

class TextureManager
{
public:
	static TextureManager& instance() {
		return s_instance;
	}

	SDL_Texture* get_texture(const std::string& ID, const std::string& path = "none", SDL_Renderer* pRenderer = nullptr);

	void clear();

private:
	TextureManager();
	static TextureManager s_instance;

	std::string m_base_path;

	std::unordered_map<std::string, SDL_Texture*> m_textures;
};

#endif