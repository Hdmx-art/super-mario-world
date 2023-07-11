#ifndef LEVELOBJECT_H
#define LEVELOBJECT_H

#include "gameobject.h"

class World;

class LevelObject : public GameObject
{
public:
	enum class COLLIDE_TYPE {
		NONE,
		ON_TOP,
		TOTAL
	};

	LevelObject(World* pWorld);
	
	void set_pos(float x, float y);
	Vector2f get_pos();

	void set_x(float x);
	void set_y(float y);

	void move(float x, float y, float dt);
	virtual void eject_process(World* pWorld, float velx, float dt);

	void set_size(float w, float h);
	Vector2f get_size();

	const Vector4f& get_transf();

	COLLIDE_TYPE get_collide_type();

	void set_flip(SDL_RendererFlip flip);
	SDL_RendererFlip get_flip();

	void  set_origin(float x, float y);
	const Vector2f& get_origin();

	bool is_ejected() {
		return m_eject;
	}

	float& eject_gravity() {
		return m_eject_gravity;
	}

	void eject() {
		m_eject = true;
		m_eject_gravity = -1000.f;
	}

	bool is_drawable();
	void set_drawable(bool d);

	void rotate(float r);
	float get_rotation();

	void set_collide_type(COLLIDE_TYPE type);

protected:
	float get_eject_coef();
	void inverse_eject_vel();

private:
	TransformComponent* m_transf;
	COLLIDE_TYPE m_collide;
	bool m_drawable;
	bool m_eject;
	float m_eject_gravity;
	float m_coef;
};

#endif