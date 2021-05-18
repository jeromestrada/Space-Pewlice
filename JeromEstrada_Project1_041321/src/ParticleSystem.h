#pragma once
//  Kevin M. Smith - CS 134 SJSU

#include "Particle.h"


//  Pure Virtual Function Class - must be subclassed to create new forces.
//
class ParticleForce {
protected:
public:
	bool applyOnce = false;
	bool applied = false;
	virtual void updateForce(Particle *) = 0;
};

class ParticleSystem {
public:
	void add(const Particle &);
	void addForce(ParticleForce *);
	void remove(int);
	void update();
	void setLifespan(float);
	void reset();
	int removeNear(const ofVec3f & point, float dist);
	void draw();
	vector<Particle> particles;
	vector<ParticleForce *> forces;
};



// Some convenient built-in forces
//
class GravityForce: public ParticleForce {
	ofVec3f gravity;
public:
	GravityForce(const ofVec3f & gravity);
	void setGravityForce(const ofVec3f & g);
	void updateForce(Particle *);
};

class TurbulenceForce : public ParticleForce {
	ofVec3f tmin, tmax;
public:
	TurbulenceForce(const ofVec3f & min, const ofVec3f &max);
	void updateForce(Particle *);
	void setTurbulence(const ofVec3f & min, const ofVec3f &max);
};

class ImpulseRadialForce : public ParticleForce {
public:
	ImpulseRadialForce(float magnitude); 
	void updateForce(Particle *);
	void setMagnitude(float mag);
	void setYClamp(float minY, float maxY);

	float magnitude;
	float yClampMin;
	float yClampMax;
};

class CyclicForce : public ParticleForce {
public:
	CyclicForce(float mag);
	void updateForce(Particle *);
	void setMagnitude(float mag);

	float magnitude = 10;
};