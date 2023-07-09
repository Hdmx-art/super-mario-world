#include "texturemanager.h"
#include "SDL.h"
#include "SDL_image.h"
#include "config.h"

SDL_Texture* TextureManager::get_texture(const std::string& ID, const std::string& path, SDL_Renderer* pRenderer)
{
	if (m_textures.find(ID) != m_textures.end())
		return m_textures[ID];

	if (pRenderer == nullptr || path == "none")
		BREAK();

	std::string final_path = m_base_path + path;

	SDL_Surface* surface = IMG_Load(final_path.c_str());
	
	if (!surface)
		BREAK();

	m_textures[ID] = SDL_CreateTextureFromSurface(pRenderer, surface);

	if (!m_textures[ID])
		BREAK();

	SDL_Log(std::string("loaded " + ID).c_str());

	return m_textures[ID];
}

void TextureManager::clear()
{
	for (auto& texture : m_textures) {
		SDL_DestroyTexture(texture.second);
		SDL_Log(std::string("destroyed " + texture.first).c_str());
	}

	m_textures.clear();
}

TextureManager::TextureManager()
{
	m_base_path = SDL_GetBasePath();
}

TextureManager TextureManager::s_instance;
