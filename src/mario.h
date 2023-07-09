#ifndef MARIO_H
#define MARIO_H

#include "levelobject.h"

class Mario : public LevelObject
{
	enum class MarioState {
		SMALL, BIG, DECREASING
	};

public:
	Mario(World* pWorld);

	void update(float dt) override;
	void render(SDL_Renderer* pRenderer) override;

	void bump();
	virtual void eject_process(World* pWorld, float velx, float dt) override;

	float get_vel();
	bool is_falling();

	void decrease();
	void grow();
	MarioState get_state();

private:
	void vertical_movement(float dt);
	void vertical_collision(float dt);
	void horizontal_movement(float dt);
	void horizontal_collision(float dt);

	void kill();

	void on_floor(LevelObject* object);
	void collect_item(float dt);
	
	AnimComponent* m_animator;
	Vector4i m_rect;
	
	float m_velocity;
	float m_gravity;
	bool m_falling;
	bool m_hit_head = false;
	bool m_frozen;
	const float MAX_VEL = 750.f;
	MarioState m_state;

	uint64_t m_time;
	uint64_t m_unstoppable_time;
};

#endif