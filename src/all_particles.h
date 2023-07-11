#ifndef ALL_PARTICLES_H
#define ALL_PARTICLES_H

#include "particle.h"
#include "soundmanager.h"

class CoinParticle : public Particle 
{
public:
	CoinParticle(World* pWorld) : Particle(pWorld) {
		animator->add_animation("tick", { {16,28},{0,0}, 10, 0.4f, 0, false });
		animator->set_animation("tick");
	}
};

class StompParticle : public Particle
{
public:
	StompParticle(World* pWorld) : Particle(pWorld) {
		SoundManager::instance().play("stomp");
		animator->add_animation("tick", { {16,16},{0,48}, 7, 0.4f, 4, false });
		animator->set_animation("tick");
	}
};

#endif