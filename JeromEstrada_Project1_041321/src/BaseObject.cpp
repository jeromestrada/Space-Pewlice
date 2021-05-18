#include "BaseObject.h"

BaseObject::BaseObject() {
	trans = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	rot = 0;
	rotSpeed = 0;
	clockwise = 0;
	counterclock = 0;
}

void BaseObject::setPosition(glm::vec3 pos) {
	trans = pos;
}

void BaseObject::integrate() {
	float dt = 1.0 / (1.0 + ofGetFrameRate());

	rot += rotSpeed * dt;
	rotSpeed += (clockwise - counterclock);
	rotSpeed *= 0.99;

	glm::mat4 rotation = glm::rotate(glm::mat4(1.0), glm::radians(rot), glm::vec3(0, 0, 1));
	acceleration = rotation * glm::vec4(acceleration, 1);


	trans += velocity * dt;
	velocity += acceleration;
	velocity *= 0.99; //damping;

	acceleration = glm::vec3(0, 0, 0);

}

bool BaseObject::isNear(glm::vec3 point, float distance) {
	ofVec3f v = trans - point;
	//cout << v.length() << "  " << distance << endl;
	if (v.length() < distance) {
		
		return true;
	}
	return false;
}