#pragma once
// This is a base object that all drawable object inherit from
// It is possible this will be replaced by ofNode when we move to 3D
//
#include "ofMain.h"

class BaseObject {
public:
	BaseObject();
	glm::vec3 trans;
	glm::vec3 scale = glm::vec3(1, 1, 1);
	float	rot;
	bool	bSelected;

	glm::vec3 velocity;
	glm::vec3 acceleration;
	float rotSpeed;
	float clockwise;
	float counterclock;
	float bubbleRadius; // this will represent the radius of the hit-circle/sphere or bubble

	void setPosition(glm::vec3);

	// Integrate for physics
	void integrate();
	// Is near for collision detection.
	bool isNear(glm::vec3 point, float distance);

	vector<glm::vec3> verts;
};