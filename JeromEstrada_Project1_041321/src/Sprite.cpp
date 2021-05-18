#include "Sprite.h"

//
// Basic Sprite Object
//
Sprite::Sprite() {
	speed = 0;
	velocity = glm::vec3(0, 0, 0);
	lifespan = -1;      // lifespan of -1 => immortal 
	birthtime = 0;
	bSelected = false;
	haveImage = false;
	name = "UnamedSprite";
	width = 60;
	height = 80;
}

// Return a sprite's age in milliseconds
//
float Sprite::age() {
	return (ofGetElapsedTimeMillis() - birthtime);
}

//  Set an image for the sprite. If you don't set one, a rectangle
//  gets drawn.
//
void Sprite::setImage(ofImage* img) {
	image = img;
	haveImage = true;
	width = image->getWidth();
	height = image->getHeight();
}

bool Sprite::setSound(string soundSource) { // sound in each sprite so the audio doesn't stop when new sounds are being played by other sprites in the system.
	if (sound.load(soundSource)) {
		return true;
	}
	return false;
}


//  Render the sprite
//
void Sprite::draw() {

	//ofSetColor(255, 255, 255, 255);

	// draw image centered and add in translation amount
	//
	if (haveImage) {// figured that translating with the negative of the offset will center the object properly.
		glm::mat4 rots = glm::rotate(glm::mat4(1.0), glm::radians(rot), glm::vec3(0, 0, 1));
		glm::mat4 transform = glm::translate(glm::mat4(1.0), glm::vec3(trans));
		glm::mat4 scale = glm::scale(glm::mat4(1.0), this->scale);
		ofPushMatrix();
		ofMultMatrix(transform * scale * rots);

		image->draw(-width / 2.0, (-height / 2.0));

		ofPopMatrix();

	}
	else {
		// in case no image is supplied, draw something.
		// 
		ofSetColor(255, 0, 0);
		ofDrawRectangle(-width / 2.0 + trans.x, -height / 2.0 + trans.y, width, height);
		ofSetColor(255, 255, 255);
	}
}