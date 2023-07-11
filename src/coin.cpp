#include "coin.h"
#include "world.h"
#include "soundmanager.h"
#include "all_particles.h"
#include "hud.h"

Coin::Coin(World* pWorld) : Item(pWorld)
{
	m_animator = add_component<AnimComponent>();

	set_size(64.f, 64.f);

	m_rect = { 0,0,16,16 };
	
	m_animator->add_animation("rotate", { {16,16},{0,0}, 4, 0.4f, 0, true });
	m_animator->set_animation("rotate");
}

void Coin::update(float dt)
{
	m_animator->update(dt, &m_rect);
}

void Coin::render(SDL_Renderer* pRenderer)
{
	SDL_CHECK(SDL_RenderCopyExF(
		pRenderer,
		TextureManager::instance().get_texture("tiles"),
		&m_rect,
		&get_transf(),
		0.f,
		&get_origin(),
		get_flip()
	));
}

void Coin::on_hit(Mario* pMario)
{
	SoundManager::instance().play("coin");

	CoinParticle* coin_particle = get_world()->add_gameobject<CoinParticle>();
	coin_particle->spawn(get_pos().x, get_pos().y, 36.f, 64.f);

	if (pMario) {
		pMario->add_coin();
	}

	HUD::instance().add_xp(10);

	get_world()->delete_gameobject(this);
}