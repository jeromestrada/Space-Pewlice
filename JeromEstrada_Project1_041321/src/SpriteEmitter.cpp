#include "SpriteEmitter.h"

//  Create a new Emitter - needs a SpriteSystem
//
Emitter::Emitter(SpriteSystem *spriteSys) {
	sys = spriteSys;
	lifespan = 3000;    // milliseconds
	started = false;

	lastSpawned = 0;
	rate = 0;    // sprites/sec
	haveChildImage = false;
	haveImage = false;
	velocity = ofVec3f(0, -500, 0);
	drawable = true;
	width = 50;
	height = 50;
}

// Create a new Emitter with images for itself and its children along with sounds. Also needs a sprite system.
Emitter::Emitter(SpriteSystem *spriteSys, ofVec3f pos, string emitterImgSource, string childImgSource, string soundSource, string moveSound) {
	// Initialization from the regular Constructor.
	sys = spriteSys;
	lifespan = 3000;    // milliseconds
	started = false;

	lastSpawned = 0;
	rate = 0;    // sprites/sec
	haveChildImage = false;
	haveImage = false;
	childVelocity = ofVec3f(0, 0, 0);
	drawable = true;
	// setup a default emitter size, incase there is no image given for this emitter.
	width = 50;
	height = 50;

	// setup for the images and sounds.
	// Set the emitter's position based on the given pos.
	trans = pos;
	// drawable is initialized as false so that the emitter doesn't show up when unintended. Useful for idle windows
	drawable = false;

	// Set the images up

	// set up the emitter's image
	if (emitterImgSource != "") { // if the call provided an image source, try to load it in.
		ofImage *emitterImg = new ofImage();
		if (emitterImg->load(emitterImgSource)) {

			// Set up the vertices of its bounding box, I called it drag box here but it can be treated as the hitbox as well
			this->setDragBox(glm::vec3(-emitterImg->getWidth() / 2.0, -emitterImg->getHeight() / 2.0, 1),
				glm::vec3(emitterImg->getWidth() / 2.0, -emitterImg->getHeight() / 2.0, 1), glm::vec3(emitterImg->getWidth() / 2.0,
					emitterImg->getHeight() / 2.0, 1), glm::vec3(-emitterImg->getWidth() / 2.0, emitterImg->getHeight() / 2.0, 1));
			height = emitterImg->getHeight();
			width = emitterImg->getWidth();
			//cout << height << " " << width << endl;
			bubbleRadius = max(height, width); // if this was a box shape then I can reach the corner with this calculation.
			bubbleRadius *= 0.35;
			haveImage = true;
		}
		else { // if the image can't be loaded, just terminate the program and display the following error message.
			ofLogFatalError("can't load image: " + emitterImgSource);
			ofExit();
		}
		setImage(emitterImg);
		haveChildImage = true;
	}
	else { /* if the call did not provide an image, set the haveImage flag to false
			when the emitter is drawn later, it will be defaulted to a small rectangle box*/
		haveImage = false;
	}
	/*create an image for sprites being spawned by emitter, this must have an image
	 there is no sense of making an emitter that emits nothing. So I don't test if the user provided and image or not
	 I feel like this is bad form though but I'll leave it like this for now.*/
	ofImage *childImg = new ofImage();
	childImg->load(childImgSource);
	setChildImage(childImg);

	// set up the sound that is played when the sprites get emitted.
	setChildSound(soundSource);
	if (moveSound != "") {
		moveS.load(moveSound);
	}
}

Emitter* Emitter::spawnEmitter(string emitterImgSource, string childImgSource, string soundSource) {
	return (new Emitter(new SpriteSystem, this->trans, emitterImgSource, childImgSource, soundSource, ""));
}

/*
I modified the inside method from Lab 1 to work with a square box, I am not sure how the orientedAngle works but
my modifications allows me to trigger a true return when I'm within the bounds of the image.
*/
bool Emitter::inside(glm::vec3 p, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4) {
	glm::vec3 v1 = glm::normalize(p1 - p);
	glm::vec3 v2 = glm::normalize(p2 - p);
	glm::vec3 v3 = glm::normalize(p3 - p);
	glm::vec3 v4 = glm::normalize(p4 - p);
	float a1 = glm::orientedAngle(v1, v2, glm::vec3(0, 0, 1));
	float a2 = glm::orientedAngle(v2, v3, glm::vec3(0, 0, 1));
	float a3 = glm::orientedAngle(v3, v4, glm::vec3(0, 0, 1));
	float a4 = glm::orientedAngle(v4, v1, glm::vec3(0, 0, 1));
	//cout << "mouse pos is " << p << endl;
	//cout << "The oriented angles are: " << a1 << " " << a2 << " " << a3 << " " << a4 << endl;
	if (a1 > 0.5 && a2 > 0.5 && a3 > 0.5 && a4 > 0.5) return true;
	else return false;
}

//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
//
//
void Emitter::draw() {
	if (drawable) {
		glm::mat4 rots = glm::rotate(glm::mat4(1.0), glm::radians(rot), glm::vec3(0, 0, 1));
		glm::mat4 transform = glm::translate(glm::mat4(1.0), glm::vec3(trans));
		glm::mat4 scale = glm::scale(glm::mat4(1.0), this->scale);
		if (haveImage) {
			ofPushMatrix();
			ofMultMatrix(transform * scale * rots);

			image->draw(-image->getWidth() / 2.0, -image->getHeight() / 2.0);

			ofPopMatrix();
		}
		else {
			ofSetColor(0, 0, 200);
			ofDrawRectangle(-width / 2 + trans.x, -height / 2 + trans.y, width, height);
			ofSetColor(255, 255, 255);
		}
	}

	// draw sprite system
	//
	sys->draw();
}

//  Update the Emitter. If it has been started, spawn new sprites with
//  initial velocity, lifespan, birthtime.
//
void Emitter::update() {
	if (!started) return;


	float time = ofGetElapsedTimeMillis();
	if ((time - lastSpawned) > (1000.0 / rate)) {
		// spawn a new sprite
		Sprite sprite;
		if (haveChildImage) {
			sprite.setImage(childImage);
			sprite.height = sprite.image->getHeight();
			sprite.width = sprite.image->getWidth();
			sprite.bubbleRadius = max(height, width);
			sprite.bubbleRadius *= 0.35;
		}
		sprite.velocity = childVelocity;
		// this sets the sprite's rotation angle before it spawns
		sprite.rot = glm::degrees(glm::orientedAngle(glm::vec3(0, -1, 0), glm::normalize(childVelocity), glm::vec3(0, 0, 1)));
		sprite.lifespan = lifespan;
		sprite.setPosition(trans);
		sprite.birthtime = time;
		sys->add(sprite);
		if (sprite.setSound(childSound)) {// set the sound of this sprite. this allows us to play the sound anytime even though it's only gonna be called once for this instance.
			sprite.sound.play(); // play immediately as it spawns
		}
		lastSpawned = time;
	}
	sys->update();

}

// Start/Stop the emitter.
//
void Emitter::start() {
	started = true;
	drawable = true;
	lastSpawned = ofGetElapsedTimeMillis();
}

void Emitter::stop() {
	started = false;
}


void Emitter::setChildLifespan(float life) {
	lifespan = life;
}

void Emitter::setChildVelocity(ofVec3f v) {
	childVelocity = v;
}

void Emitter::setChildImage(ofImage* img) {
	childImage = img;
	haveChildImage = true;
}

void Emitter::setImage(ofImage* img) {
	image = img;
	haveImage = true;
}

void Emitter::setChildSound(string soundSource) { // sound in each sprite so the audio doesn't stop when new sounds are being played by other sprites in the system.
	childSound = soundSource;
}

void Emitter::setRate(float r) {
	rate = r;
}
