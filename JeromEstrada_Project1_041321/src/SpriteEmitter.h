#pragma once
#include "SpriteSystem.h"

//  General purpose Emitter class for emitting sprites
//  This works similar to a Particle emitter
//
class Emitter : public BaseObject {
public:
	Emitter(SpriteSystem *);
	void setDragBox(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4) {
		verts.push_back(p1);
		verts.push_back(p2);
		verts.push_back(p3);
		verts.push_back(p4);
	};
	Emitter(SpriteSystem *, ofVec3f pos, string emitterImgSource, string childImgSource, string soundSource, string moveSound);
	Emitter* spawnEmitter(string, string, string);
	bool inside(glm::vec3 p, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4);
	void draw();
	void start();
	void stop();
	void setChildLifespan(float); // of the children sprites
	void setChildVelocity(ofVec3f); // of the children sprites
	void setChildImage(ofImage*);
	void setImage(ofImage*); // sets image of the emitter, i.e. turret, gun, or whatever emmits some projectile
	void setChildSound(string);
	void setRate(float); // sets the rate of emmision
	void update();



	//float approachSpeed(float current, float goal, float dt);
	//float speed;
	SpriteSystem *sys;
	float rate;
	glm::vec3 childVelocity;

	float lifespan;
	bool started;
	float lastSpawned;
	ofImage *childImage;
	ofImage *image;
	string childSound;
	ofSoundPlayer moveS; // sound for emitter when moving.
	bool drawable;
	bool haveChildImage;
	bool haveImage;
	float width, height;
};