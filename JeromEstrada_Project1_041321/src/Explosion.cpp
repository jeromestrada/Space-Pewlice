#include "Explosion.h"

Explosion::Explosion(ofVec3f position, float mag, float partRadius, float lifeSpan, float groupSize, string expSound) {
	oneShot = true;
	fired = false;
	setParticleRadius(partRadius);
	setLifespan(lifeSpan);
	setGroupSize(groupSize);
	setEmitterType(RadialEmitter);
	setPosition(position);
	force = new ImpulseRadialForce(mag);
	force->applied = false;
	sys->addForce(force);
	setSound(expSound);
}

void Explosion::setSound(string soundSource) {
	expSound.loadSound(soundSource);
}