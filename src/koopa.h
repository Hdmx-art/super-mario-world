#ifndef KOOPA_H
#define KOOPA_H

#include "ennemy.h"

class Mario;
class Shell;

class Koopa : public Ennemy
{
public:
	Koopa(World* pWorld);

	void update(float dt);
	void render(SDL_Renderer* pRenderer);

	void on_hit(Mario* pMario);
	bool sliding();

	bool is_kicked();

private:
	void jump_mario(Mario* pMario, const std::string& ID);
	void on_jumped(float dt);
	void on_kicked(float dt);
	void movement(float dt);
	void update_physics(float dt);

	AnimComponent* m_animator;
	SDL_Rect m_rect;
	Shell* m_shell;
	
	Vector2f m_kicked_pos;
	bool m_falling;
	float m_gravity;
	float m_vel;
	bool m_is_sliding;
	bool m_jumped_on;
	bool m_kicked_out_of_shell;
};

#endif