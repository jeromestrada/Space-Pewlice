#pragma once

#include "Particle.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"

class Explosion : public ParticleEmitter
{
public:
	Explosion(ofVec3f, float, float, float, float, string);
	void setSound(string);

	ImpulseRadialForce *force;
	ofSoundPlayer expSound;
};

