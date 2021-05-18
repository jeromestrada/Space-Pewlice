#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "BaseObject.h"
#include "Sprite.h"
#include "SpriteSystem.h"
#include "SpriteEmitter.h"
#include "TransformObject.h"
#include "Particle.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"
#include "Explosion.h"

/* Seems like I can include just the Emitter and 
it would include the others but might as well*/

// typedef enum { MoveStop, MoveLeft, MoveRight, MoveUp, MoveDown } MoveDir; // I didn't use this so I'm taking it out for now.

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();

		glm::vec3 curveEval(float x, float scale, float cycles);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		// function that help set up emitters
		//void setUpEmitter(ofVec3f pos, string emitterImgSource, string childImgSource, string soundSource);
		
		Emitter *player;
		vector<Emitter *> enemies;
		vector<Emitter *> enemyReinf;
		vector<Emitter *> friendlies;
		ImpulseRadialForce *force = new ImpulseRadialForce(50000);
		int numEmitters = 0;
		float score = 0;

		float lastMinSpawned = 0.0f;

		Sprite bg;
		Sprite stars;
		Sprite bigStars;
		Sprite flash;

		bool friendlyHit;
		float flashTime;

		float startTime;
		float gameTime;
		float penalty;

		ofSoundPlayer bgSound;
		ofTrueTypeFont origin;
		ofTrueTypeFont clockFont;
		ofTrueTypeFont gameOverFont;
		ofTrueTypeFont spaceFont;
		ofTrueTypeFont prototypeFont;

		Sprite police;
		Sprite friends;
		Sprite opponents;

		vector<Explosion*> explosions;
		
		glm::vec3 mouse_last;
		glm::vec3 lastPos;
		bool bMouseDragged = FALSE;
		glm::vec3 mouseDragVec;

		bool bHide;
		bool bRightKeyPressed = FALSE;
		bool bLeftKeyPressed = FALSE;
		bool bUpKeyPressed = FALSE;
		bool bDownKeyPressed = FALSE;
		bool resetGame = FALSE;
		bool gameOver = FALSE;
		bool pauseGame = FALSE;
		bool moving = FALSE;

		bool bCounterClockRot = FALSE;
		bool bClockRot = FALSE;
		bool bControlRotation = FALSE;

		ofxFloatSlider rate;
		ofxFloatSlider life;
		ofxVec3Slider velocity;
		ofxLabel screenSize;


		ofxPanel gui;
};
