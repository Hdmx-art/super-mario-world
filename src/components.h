#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "rects.h"
#include <SDL_render.h>
#include <unordered_map>
#include <string>
#include "config.h"

class Component {
public:
	Component()
	{
	}

	virtual ~Component() 
	{
	}
};

class TransformComponent : public Component {
public:
	TransformComponent() 
	{
		transf = { 0.f,0.f,0.f,0.f };
		origin = { 0.f,0.f };
		flip = SDL_FLIP_NONE;
		rotation = 0.f;
	}

	void debug_draw(SDL_Renderer* pRenderer) {
		SDL_SetRenderDrawColor(pRenderer, 0, 255, 0, 255);
		SDL_RenderDrawRectF(pRenderer, &transf);
	}

	Vector4f transf;
	Vector2f origin;
	SDL_RendererFlip flip;
	float rotation;
};

class AnimComponent : public Component {

public:
	struct Anim {
	public:
		Vector2i size;
		Vector2i start;
		int32_t frame_count;
		float duration;
		int32_t offset;
		bool loop;
	};
	
	AnimComponent() 
	{
		m_time = 0;
		m_id = "none";
		m_finished = false;
	}
		
	void add_animation(const std::string& ID, const Anim& anim) {
		m_animations[ID] = anim;
	}

	void set_animation(const std::string& ID) {
		if (m_animations.find(ID) == m_animations.end())
			BREAK();

		if (m_id != ID) {

			if (m_animations[ID].loop == false)
				m_finished = false;

			m_time = 0.f;
			m_id = ID;
		}
	}

	void update(float dt, SDL_Rect* pRect) {
		m_time += dt;
		
		Anim anim = m_animations[m_id];
		float scaled_time = (m_time / anim.duration);
		int32_t frame = static_cast<int>(scaled_time * anim.frame_count);
		
		if(anim.loop)
			frame %= anim.frame_count;
		else if (frame >= anim.frame_count) {
			frame = anim.frame_count - 1;
			m_finished = true;
		}

		*pRect = { anim.start.x + frame * (anim.size.x + anim.offset),anim.start.y,
			anim.size.x, anim.size.y };
	}

	const std::string& get_current_animation() {
		return m_id;
	}

	void reset() {
		m_time = 0;
		m_finished = false;
	}

	bool is_finished() {
		return m_finished;
	}

private:
	std::unordered_map<std::string, Anim> m_animations;
	std::string m_id;

	float m_time;
	bool m_finished;
};

#endif