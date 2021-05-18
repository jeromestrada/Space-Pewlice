#pragma once
//  General Sprite class  (similar to a Particle)
//
#include "BaseObject.h"

class Sprite : public BaseObject {
public:
	Sprite();
	void draw();
	float age();
	void setImage(ofImage*);
	bool setSound(string);

	float speed;    //   in pixels/sec
	ofVec3f velocity; // in pixels/sec
	ofImage* image;
	ofSoundPlayer sound;
	float birthtime; // elapsed time in ms
	float lifespan;  //  time in ms
	string name;
	bool haveImage;
	float width, height;
};
