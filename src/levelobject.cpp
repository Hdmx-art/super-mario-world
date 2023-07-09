#include "LevelObject.h"
#include "Game.h"
#include "world.h"
#include "hud.h"

LevelObject::LevelObject(World* pWorld) : GameObject(pWorld)
{
	m_transf = add_component<TransformComponent>();
	m_transf->transf = { 0.f,0.f,0.f,0.f };
	m_drawable = false;
	m_eject = false;
	m_eject_gravity = 0.f;
	m_coef = 1.f;
	set_collide_type(COLLIDE_TYPE::NONE);
}

void LevelObject::set_pos(float x, float y)
{
	m_transf->transf.x = x;
	m_transf->transf.y = y;
}

Vector2f LevelObject::get_pos()
{
	return { m_transf->transf.x, m_transf->transf.y };
}

void LevelObject::set_x(float x)
{
	m_transf->transf.x = x;
}

void LevelObject::set_y(float y)
{
	m_transf->transf.y = y;
}

void LevelObject::move(float x, float y, float dt)
{
  	m_transf->transf.x += x * dt;
	m_transf->transf.y += y * dt;
}

void LevelObject::eject_process(World* pWorld, float velx, float dt)
{
	set_origin(get_size().x / 2.f, get_size().y / 2.f);
	eject_gravity() += 7500.f * dt;
	move(velx * get_eject_coef(), eject_gravity(), dt);
	rotate(800.f * dt);

	if (get_pos().y > Game::instance().get_window_size().y) {
		pWorld->delete_gameobject(this);
		return;
	}
}

void LevelObject::draw_eject(SDL_Renderer* pRenderer, const Vector2f& pos)
{
	HUD::instance().draw_text(pRenderer, std::to_string(200), pos, 30);
}

void LevelObject::set_size(float w, float h)
{
	m_transf->transf.w = w;
	m_transf->transf.h = h;
}

Vector2f LevelObject::get_size()
{
	return { m_transf->transf.w, m_transf->transf.h };
}

const Vector4f& LevelObject::get_transf()
{
	return m_transf->transf;
}

LevelObject::COLLIDE_TYPE LevelObject::get_collide_type()
{
	return m_collide;
}

void LevelObject::set_flip(SDL_RendererFlip flip)
{
	m_transf->flip = flip;
}

SDL_RendererFlip LevelObject::get_flip()
{
	return m_transf->flip;
}

void LevelObject::set_origin(float x, float y)
{
	m_transf->origin = { x,y };
}

const Vector2f& LevelObject::get_origin()
{
	return m_transf->origin;
}

bool LevelObject::is_drawable()
{
	return m_drawable;
}

void LevelObject::set_drawable(bool d)
{
	m_drawable = d;
}

void LevelObject::rotate(float r)
{
	m_transf->rotation += r;
}

float LevelObject::get_rotation()
{
	return m_transf->rotation;
}

void LevelObject::set_collide_type(COLLIDE_TYPE type)
{
	m_collide = type;
}

float LevelObject::get_eject_coef()
{
	return m_coef;
}

void LevelObject::inverse_eject_vel()
{
	m_coef *= -1.f;
}
