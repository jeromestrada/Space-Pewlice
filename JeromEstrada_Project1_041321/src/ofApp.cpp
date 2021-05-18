/*
Jerom Estrada
CS134 Computer Game Design - Sec 01

Project 1 - Vintage Arcade Game - Part 3 - Physics
03/21/2021

Final version (?) of the Vintage Arcade Game that will integrate physics into the movement of objects, most notably the Player's movement.
The player is swarmed by endless amounts of Hostile, sometimes Aggressive enemy Ships. The player has to eliminate as many enemy ships as possible
without shooting down Neutral Ships. If the Player shoots down a Neutral Ship, the score is heavily penalized.

Initial code written by Prof. Kevin M. Smith for CS 134

*/

//----------------------------------------------------------------------------------
//
// This example code demonstrates the use of an "Emitter" class to emit Sprites
// and set them in motion. The concept of an "Emitter" is taken from particle
// systems (which we will cover next week).
//
// The Sprite class has also been upgraded to include lifespan, velocity and age
// members.   The emitter can control rate of emission and the current velocity
// of the particles. In this example, there is no acceleration or physics, the
// sprites just move simple frame-based animation.
//
// The code shows a way to attach images to the sprites and optional the
// emitter (which is a point source) can also have an image.  If there are
// no images attached, a placeholder rectangle is drawn.
// Emitters  can be placed anywhere in the window. In this example, you can drag
// it around with the mouse.
//
// OF has an add-in called ofxGUI which is a very simple UI that is useful for
// creating sliders, buttons and fields. It is not recommended for commercial 
// game development, but it is useful for testing.  The "h" key will hide the GUI
// 
// If you want to run this example, you need to use the ofxGUI add-in in your
// setup.
//
//
//  Kevin M. Smith - CS 134 SJSU





#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	// we initialize the game time to 0 at setup
	gameTime = 0;

	ofSetVerticalSync(true);

	// load the background music
	bgSound.load("sounds/space invader version 420.mp3");
	bgSound.setLoop(true);
	bgSound.play();

	// load the Fonts that will be used in the game.
	origin.load("fonts/OriginTech personal use.ttf", 30, true, true);
	origin.setLineHeight(35.0f);
	origin.setLetterSpacing(1.5f);

	clockFont.load("fonts/alarm clock.ttf", 20, true, true);
	clockFont.setLineHeight(25.0f);
	clockFont.setLetterSpacing(1.0f);

	gameOverFont.load("fonts/game_over.ttf", 150, true, true);
	gameOverFont.setLineHeight(55.0f);
	gameOverFont.setLetterSpacing(2.0f);

	spaceFont.load("fonts/space age.ttf", 70, true, true);
	spaceFont.setLineHeight(55.0f);
	spaceFont.setLetterSpacing(1.0f);

	prototypeFont.load("fonts/Prototype.ttf", 30, true, true);
	prototypeFont.setLineHeight(55.0f);
	prototypeFont.setLetterSpacing(1.0f);

	// load images to display in the idle/instructions screen.
	ofImage *policeImg = new ofImage();
	policeImg->loadImage("images/ShipSprite.png");
	police.setImage(policeImg);
	police.trans = glm::vec3(ofGetWidth() - 130, 120, 0);
	police.scale = glm::vec3(2,2,2);

	ofImage *friendImg = new ofImage();
	friendImg->loadImage("images/Friendly.png");
	friends.setImage(friendImg);
	friends.trans = glm::vec3(ofGetWidth() * 0.33, ofGetHeight() * 0.33, 0);
	friends.scale = glm::vec3(1.3, 1.3, 1.3);

	ofImage *oppImg = new ofImage();
	oppImg->loadImage("images/EnemyShip.png");
	opponents.setImage(oppImg);
	opponents.trans = glm::vec3(ofGetWidth() * 0.33, ofGetHeight() / 2.0, 0);
	opponents.scale = glm::vec3(1.3, 1.3, 1.3);

	// load the assets for the background image.
	ofImage *bgImg = new ofImage();
	bgImg->loadImage("images/Background.png");
	bg.setImage(bgImg);
	bg.trans = glm::vec3(ofGetWidth() / 2.0, ofGetHeight()/ 2.0, 0);
	bg.scale = glm::vec3(1.3, 1.3, 1.3);

	ofImage *starsImg = new ofImage();
	starsImg->loadImage("images/Stars.png");
	stars.setImage(starsImg);
	stars.trans = glm::vec3(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);

	ofImage *bigStarsImg = new ofImage();
	bigStarsImg->loadImage("images/BigStars.png");
	bigStars.setImage(bigStarsImg);
	bigStars.trans = glm::vec3(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);
	
	// load asset for indicating that the Player shot a Neutral Ship down.
	ofImage *flashImg = new ofImage();
	flashImg->loadImage("images/Flash.png");
	flash.setImage(flashImg);
	flash.trans = glm::vec3(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);
	friendlyHit = false;

	// Setup the player Ship.
	player = new Emitter(new SpriteSystem(), glm::vec3(ofGetWidth() / 2.0, ofGetHeight() / 1.333, 0), "images/ShipSprite.png", "images/SphereMissileSprite.png", "sounds/Pew_Pew-DKnight556-1379997159.mp3", "sounds/Missle_Launch-Kibblesbob-2118796725.mp3");
	moving = FALSE;
	penalty = 0.8;

	//setUpEmitter(ofVec3f(ofGetWidth() / 2.0, ofGetHeight() / 5.0, 0), "images/EnemyShip.png", "images/EnemyMissile.png", "sounds/Laser Blasts-SoundBible.com-108608437.mp3"); // Enemy mother ship
	enemies.push_back(new Emitter(new SpriteSystem(), glm::vec3(ofGetWidth() / 2.0, ofGetHeight() / 5.0, 0), "images/EnemyShip.png", "images/EnemyMissile.png", "sounds/Laser Blasts-SoundBible.com-108608437.mp3", ""));

	// Setup the emitters for the swarms of enemy and neutral ships
	enemyReinf.push_back(new Emitter(new SpriteSystem(), glm::vec3(100, -50, 0), "", "images/EnemyShip.png", "Rocket Thrusters-SoundBible.com-1432176431.mp3", ""));
	friendlies.push_back(new Emitter(new SpriteSystem(), glm::vec3(100, -50, 0), "", "images/Friendly.png", "Rocket Thrusters-SoundBible.com-1432176431.mp3", ""));

	enemyReinf.push_back(new Emitter(new SpriteSystem(), glm::vec3(ofGetWindowWidth() / 2.0, -50, 0), "", "images/EnemyShip.png", "Rocket Thrusters-SoundBible.com-1432176431.mp3", ""));
	friendlies.push_back(new Emitter(new SpriteSystem(), glm::vec3(ofGetWindowWidth() / 2.0, -50, 0), "", "images/Friendly.png", "Rocket Thrusters-SoundBible.com-1432176431.mp3", ""));

	enemyReinf.push_back(new Emitter(new SpriteSystem(), glm::vec3(ofGetWindowWidth() - 100.0, -50, 0), "", "images/EnemyShip.png", "Rocket Thrusters-SoundBible.com-1432176431.mp3", ""));
	friendlies.push_back(new Emitter(new SpriteSystem(), glm::vec3(ofGetWindowWidth() - 100.0, -50, 0), "", "images/Friendly.png", "Rocket Thrusters-SoundBible.com-1432176431.mp3", ""));


	gui.setup();
	gui.add(rate.setup("rate", 5, 1, 100));
	gui.add(life.setup("life", 5, .1, 10));
	gui.add(velocity.setup("velocity", ofVec3f(0, -1000, 0), ofVec3f(-10000, -10000, -10000), ofVec3f(10000, 10000, 10000)));
	bHide = true;
}


/*
I'm thinking of putting the collision detection on the ofApp level, 
this way, I have access to all the objects rendered on the screen.

Not particularly sure if this is proper form.
*/
//--------------------------------------------------------------
void ofApp::update() {
	// when the player emitter has been started and the game isn't over we execute the main update sequence.
	if (player->started && !gameOver) {
		//if the game isn't pause, we continue.
		if (!pauseGame) {
			gameTime = ofGetElapsedTimeMillis() - startTime;
			if (score >= 500 && gameTime / 1000 <= 60) { // this is a small reward to players that can manage to score 500 points before the 60 second mark.
				penalty = 0.9; // I call it penalty here but this is the multiplier to the score which reduces it down to 90%, which is a 10% penalty.
			}
			if (gameTime / 1000 >= 120) { // 120 minutes of play time
				gameTime = 3000;
				gameOver = TRUE;
			}
			if (resetGame) { // if the game is ever reset, we reload emitters similar to how they were loaded in the setup.
				gameOver = FALSE;
				gameTime = 0;
				startTime = ofGetElapsedTimeMillis();
				player->stop();
				player = new Emitter(new SpriteSystem(), glm::vec3(ofGetWidth() / 2.0, ofGetHeight() / 1.333, 0), "images/ShipSprite.png", "images/SphereMissileSprite.png", "sounds/Pew_Pew-DKnight556-1379997159.mp3", "sounds/Missle_Launch-Kibblesbob-2118796725.mp3");

				score = 0; // clear the score to 0
				enemies.clear();
				//setUpEmitter(ofVec3f(ofGetWidth() / 2.0, ofGetHeight() / 5.0, 0), "images/EnemyShip.png", "images/EnemyMissile.png", "sounds/Laser Blasts-SoundBible.com-108608437.mp3"); // Enemy mother ship
				enemies.push_back(new Emitter(new SpriteSystem(), glm::vec3(ofGetWidth() / 2.0, ofGetHeight() / 5.0, 0), "images/EnemyShip.png", "images/EnemyMissile.png", "sounds/Laser Blasts-SoundBible.com-108608437.mp3", ""));

				enemyReinf.clear();
				friendlies.clear();

				enemyReinf.push_back(new Emitter(new SpriteSystem(), glm::vec3(100, -50, 0), "", "images/EnemyShip.png", "Rocket Thrusters-SoundBible.com-1432176431.mp3", ""));
				friendlies.push_back(new Emitter(new SpriteSystem(), glm::vec3(100, -50, 0), "", "images/Friendly.png", "Rocket Thrusters-SoundBible.com-1432176431.mp3", ""));

				enemyReinf.push_back(new Emitter(new SpriteSystem(), glm::vec3(ofGetWindowWidth() / 2.0, -50, 0), "", "images/EnemyShip.png", "Rocket Thrusters-SoundBible.com-1432176431.mp3", ""));
				friendlies.push_back(new Emitter(new SpriteSystem(), glm::vec3(ofGetWindowWidth() / 2.0, -50, 0), "", "images/Friendly.png", "Rocket Thrusters-SoundBible.com-1432176431.mp3", ""));

				enemyReinf.push_back(new Emitter(new SpriteSystem(), glm::vec3(ofGetWindowWidth() - 100.0, -50, 0), "", "images/EnemyShip.png", "Rocket Thrusters-SoundBible.com-1432176431.mp3", ""));
				friendlies.push_back(new Emitter(new SpriteSystem(), glm::vec3(ofGetWindowWidth() - 100.0, -50, 0), "", "images/Friendly.png", "Rocket Thrusters-SoundBible.com-1432176431.mp3", ""));
			}

			player->setChildLifespan(life * 1000); // converted into milliseconds
			player->setChildVelocity(glm::rotate(glm::mat4(1.0), glm::radians(player->rot), glm::vec3(0, 0, 1)) * glm::vec4(0, -700, 0, 1));

			// the following for-loops updates the emitters that spawns the swarm.
			for (int i = 0; i < enemyReinf.size(); i++) {
				enemyReinf[i]->setChildLifespan(life * 4000);
				float reinfRot = glm::orientedAngle(glm::vec3(0, -1, 0), glm::normalize(glm::vec3(player->trans - enemyReinf[i]->trans)), glm::vec3(0, 0, 1));
				enemyReinf[i]->rot = glm::degrees(reinfRot) + ofRandom(-15.0, 15.0);
				enemyReinf[i]->setChildVelocity(glm::rotate(glm::mat4(1.0), glm::radians(enemyReinf[i]->rot), glm::vec3(0, 0, 1)) * glm::vec4(0, -120, 0, 1));
				enemyReinf[i]->setRate(ofRandom(0.1, 0.5) + score / 2000.0);
				enemyReinf[i]->update();
			}

			for (int j = 0; j < friendlies.size(); j++) {
				friendlies[j]->setChildLifespan(life * 4000);
				float friendliesRot = glm::orientedAngle(glm::vec3(0, -1, 0), glm::normalize(glm::vec3(player->trans - friendlies[j]->trans)), glm::vec3(0, 0, 1));
				friendlies[j]->rot = glm::degrees(friendliesRot) + ofRandom(-22.0, 22.0);
				friendlies[j]->setChildVelocity(glm::rotate(glm::mat4(1.0), glm::radians(friendlies[j]->rot), glm::vec3(0, 0, 1)) * glm::vec4(0, -80, 0, 1));
				friendlies[j]->setRate(0.1 + score / 2000.0);
				friendlies[j]->update();
			}
			Emitter *motherEnemy;


			// rotation updates
			if (bClockRot) {
				player->clockwise = 6.0;
			}
			else {
				player->clockwise = 0;
			}
			if (bCounterClockRot) {
				player->counterclock = 6.0;
			}
			else {
				player->counterclock = 0;
			}
			// thrusts updates
			float thrust = 7.0;
			if (bUpKeyPressed) {
				player->clockwise /= 3;
				player->counterclock /= 3;
				player->acceleration += glm::vec3(0, -thrust, 0);
			}
			else {
				player->acceleration += glm::vec3(0, 0, 0);
			}
			if (bDownKeyPressed) {
				player->acceleration += glm::vec3(0, thrust, 0);
			}
			else {
				player->acceleration += glm::vec3(0, 0, 0);
			}
			if (bLeftKeyPressed) {
				player->acceleration += glm::vec3(-thrust, 0, 0);
			}
			else {
				player->acceleration += glm::vec3(0, 0, 0);
			}
			if (bRightKeyPressed) {
				player->acceleration += glm::vec3(thrust, 0, 0);
			}
			else {
				player->acceleration += glm::vec3(0, 0, 0);
			}

			// border collision handling, it's just a simple reversal of the current velocity so the Player is bounced back rather roughly.
			if (player->trans.y <= 10 || player->trans.y >= ofGetWindowHeight() - 10 ||
				player->trans.x <= 10 || player->trans.x >= ofGetWindowWidth() - 10) {
				player->velocity *= -1.0;
			}


			// if there are Aggresive enemies on the screen we update them in the following.
			if (enemies.size() > 0) {
				motherEnemy = enemies[0]; // One of the enemy ships will be a mother ship, or more like a shooting enemy.
				if (motherEnemy->started) {
					float rotRad2 = glm::orientedAngle(glm::vec3(0, -1, 0), glm::normalize(glm::vec3(player->trans - motherEnemy->trans)), glm::vec3(0, 0, 1));
					motherEnemy->rot = glm::degrees(rotRad2);
					motherEnemy->setChildVelocity(glm::rotate(glm::mat4(1.0), glm::radians(motherEnemy->rot), glm::vec3(0, 0, 1)) * glm::vec4(0, -500, 0, 1));
					motherEnemy->acceleration = glm::vec3(0, -1.0, 0);
					motherEnemy->integrate();
					motherEnemy->update();
					motherEnemy->setRate(1.0);
				}

				// - - The following is an experimentation of spawning new enemies from an existing Emitter.
				// The main ship spawns new ship in the following.
				if (player->started) {
					float minTime = ofGetElapsedTimeMillis();
					if ((minTime - lastMinSpawned) > (3000.0)) { // regularly spawns a new enemy ship
						enemies.push_back(motherEnemy->spawnEmitter("images/EnemyShip.png", "images/EnemyMissile.png", "sounds/Laser Blasts-SoundBible.com-108608437.mp3"));
						lastMinSpawned = minTime;
					}
					// the following for-loop moves the ships that were spawned by the main motherEnemy ship.
					for (int i = 1; i < enemies.size(); i++) {
						enemies[i]->start();
						float enemyRot = glm::orientedAngle(glm::vec3(0, -1, 0), glm::normalize(glm::vec3(player->trans - enemies[i]->trans)), glm::vec3(0, 0, 1));
						enemies[i]->rot = glm::degrees(enemyRot);
						if (!friendlyHit) {
							enemies[i]->acceleration = glm::vec3(0, -0.8, 0);
							enemies[i]->scale = glm::vec3(0.5, 0.5, 0.5);
						}
						else {
							enemies[i]->acceleration = glm::vec3(0, -3, 0);
							enemies[i]->scale = glm::vec3(0.6, 0.6, 0.6);
						}
						enemies[i]->integrate();
						enemies[i]->update();

					}
				}
			}
			player->integrate(); // physics based movement. This isn't accurate physics though.

			// Parallax effect for the background.
			bg.trans += -player->velocity * .00025;
			stars.trans += player->velocity * .00015;
			bigStars.trans += player->velocity * .00005;

			// The following looks horrible, caution when viewing code, might cause blindness. c(0v0c)
			// Collision Detection once all the enemies are updated a little bit.
			// borrowing the iterator idea of deleting sprites when they reach their lifespan
			if (player->sys->sprites.size() && (enemies.size() || enemyReinf.size())) {
				vector<Sprite>::iterator s = player->sys->sprites.begin();
				vector<Sprite>::iterator tmp;
				float partRadius = 1.5;
				float partLifeSpan = 0.75;
				int groupSize = 200;
				float magnitude = 50000;
				while (s != player->sys->sprites.end() && (enemies.size() || enemyReinf.size())) { // while there's player missiles flying and there's an enemy on the screen.
					vector<Emitter*>::iterator e = enemies.begin();
					vector<Emitter*>::iterator eTmp;
					while (s != player->sys->sprites.end() && e != enemies.end()) {
						//cout << (*e)->bubbleRadius << " " << s->bubbleRadius << endl;
						if (e != enemies.end() && (*e)->isNear(s->trans, ((*e)->bubbleRadius + s->bubbleRadius))) {
							Explosion *exp = new Explosion((*e)->trans, magnitude, partRadius, partLifeSpan, groupSize, "sounds/RPG_Plus_Shrapnel-Mike_Koenig-802194222.mp3");

							explosions.push_back(exp);
							exp->start();
							exp->expSound.play();
							exp->update();

							tmp = player->sys->sprites.erase(s); // destroy the missile
							s = tmp;

							eTmp = enemies.erase(e);
							e = eTmp;

							score += 5;
						}
						else if (e != enemies.end()) {
							e++;
						}


					}
					// collision detection for the Enemy Swarm
					for (int i = 0; i < enemyReinf.size(); i++) {
						vector<Sprite>::iterator r = enemyReinf[i]->sys->sprites.begin();
						vector<Sprite>::iterator rTmp;
						while (s != player->sys->sprites.end() && r != enemyReinf[i]->sys->sprites.end()) {

							if (r != enemyReinf[i]->sys->sprites.end() && r->isNear(s->trans, (r->bubbleRadius + s->bubbleRadius))) {
								Explosion *exp = new Explosion(r->trans, magnitude, partRadius, partLifeSpan, groupSize, "sounds/RPG_Plus_Shrapnel-Mike_Koenig-802194222.mp3");

								explosions.push_back(exp);
								exp->start();
								exp->expSound.play();
								exp->update();
								tmp = player->sys->sprites.erase(s); // destroy the missile
								s = tmp;

								rTmp = enemyReinf[i]->sys->sprites.erase(r);
								r = rTmp;

								score += 20;
							}
							else if (r != enemyReinf[i]->sys->sprites.end()) {
								r++;
							}
						}
					}

					// Collision detection for the Neutral Swarm.
					for (int i = 0; i < friendlies.size(); i++) {
						vector<Sprite>::iterator f = friendlies[i]->sys->sprites.begin();
						vector<Sprite>::iterator fTmp;
						while (s != player->sys->sprites.end() && f != friendlies[i]->sys->sprites.end()) {

							if (f != friendlies[i]->sys->sprites.end() && f->isNear(s->trans, (f->bubbleRadius + s->bubbleRadius))) {
								enemies.push_back(new Emitter(new SpriteSystem(), f->trans, "images/EnemyShip.png", "images/EnemyMissile.png", "sounds/Laser Blasts-SoundBible.com-108608437.mp3", ""));
								Explosion *exp = new Explosion(f->trans, magnitude, partRadius, partLifeSpan, groupSize, "sounds/RPG_Plus_Shrapnel-Mike_Koenig-802194222.mp3");

								explosions.push_back(exp);
								exp->start();
								exp->expSound.play();
								exp->update();
								tmp = player->sys->sprites.erase(s); // destroy the missile
								s = tmp;

								fTmp = friendlies[i]->sys->sprites.erase(f);
								f = fTmp;
								flashTime = ofGetElapsedTimeMillis();
								friendlyHit = true;

								score *= penalty; // if you hit a friendly, you lose 20% of your current score.
							}
							else if (f != friendlies[i]->sys->sprites.end()) {
								f++;
							}
						}
					}

					if (s != player->sys->sprites.end()) {
						s++;
					}
				}
			}
			// Collision detection between the aggresive ship's missiles and the player.
			if (enemies.size()) {
				motherEnemy = enemies[0];
				if (player->started && !motherEnemy->started) {
					motherEnemy->start();
				}
				if (motherEnemy->sys->sprites.size()) {
					vector<Sprite>::iterator m = motherEnemy->sys->sprites.begin();
					vector<Sprite>::iterator mTmp;
					while (m != motherEnemy->sys->sprites.end()) {
						if (player->isNear(m->trans, m->bubbleRadius + player->bubbleRadius)) {
							mTmp = motherEnemy->sys->sprites.erase(m);
							m = mTmp;
							score *= .95; // subtracts 5% point per shot taken.
						}
						else {
							m++;
						}
					}
				}
			}

			// updates the explosion animation.
			vector<Explosion*>::iterator ex = explosions.begin();
			vector<Explosion*>::iterator exTmp;
			while (ex != explosions.end()) {
				if (ofGetElapsedTimeMillis() - (*ex)->lastSpawned >= (*ex)->lifespan * 1000) {
					(*ex)->expSound.stop();
					exTmp = explosions.erase(ex);
					ex = exTmp;
				}
				if (ex != explosions.end()) {
					(*ex)->update();
					ex++;
				}
			}


			if (enemies.size() == 0) { // whenever all the enemies get eliminated, one will spawn somewhere at the bottom of the screen
				enemies.push_back(new Emitter(new SpriteSystem(), glm::vec3(ofRandom(0, ofGetWindowWidth()), ofGetHeight() + 50, 0), "images/EnemyShip.png", "images/EnemyMissile.png", "sounds/Laser Blasts-SoundBible.com-108608437.mp3", ""));
			}
			player->update();
		}
		// if the game is Paused, keep track of the time to resume the timer correctly.
		else {
			startTime = ofGetElapsedTimeMillis() - gameTime;
		}
	}
}



//--------------------------------------------------------------
void ofApp::draw() {
	// draw the background assets.
	ofSetColor(ofColor::white);
	bg.draw();
	stars.draw();
	bigStars.draw();

	// draw the player emitter.
	player->draw();

	// draw the Swarm.
	for (int i = 0; i < enemyReinf.size(); i++) {
		enemyReinf[i]->draw();
	}

	for (int i = 0; i < friendlies.size(); i++) {
		friendlies[i]->draw();
	}
	// draws a flash on the sides of the screen when a Neutral is hit.
	if (friendlyHit) {
		ofSetColor(ofColor::red);
		flash.draw();
		if (ofGetElapsedTimeMillis() - flashTime > 1500) {
			ofSetColor(ofColor::white);
			friendlyHit = false;
		}
	}
	// draw the Aggressive Hostiles.
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->draw();
	}

	// draw any explosions that are triggered.
	vector<Explosion*>::iterator ex = explosions.begin();
	vector<Explosion*>::iterator exTmp;
	while (ex != explosions.end()) {
		if (ofGetElapsedTimeMillis() - (*ex)->lastSpawned >= (*ex)->lifespan * 1000) {
			exTmp = explosions.erase(ex);
			ex = exTmp;
		}
		if (ex != explosions.end()) {
			(*ex)->draw();
			ex++;
		}
	}

	// draws the gui, hidden by default.
	if (!bHide) {
		gui.draw();
	}

	// draws the Idle screen with all the instructions and explanation of how the game works.
	if (!player->started && (ofGetElapsedTimeMillis() / 500) % 2) {
		origin.drawString("Press SPACEBAR to PLAY", (ofGetWindowWidth() / 2.0 - 330), ofGetWindowHeight() * 0.75);
	}
	if (!player->started) {
		friends.draw();
		ofDrawBitmapString("Neutral - Reduces SCORE by 20% when shot\nSpawns Aggressive Hostiles afterwards", friends.trans + glm::vec3(100, 0, 0));
		opponents.draw();
		ofDrawBitmapString("Hostile - 20 points when destroyed\nSometimes Aggresive and shoots at you for -5% points\nAggresives also spawn more Aggresives.", opponents.trans + glm::vec3(100, 0, 0));
		string message = "  Try to score as much as possible in 2 minutes! \nScore 500 before the 60 second mark and Neutral Kills punishes you less\n \t\t\t(-10% instead)";
		ofDrawBitmapString(message, glm::vec3(ofGetWindowWidth() / 2.0 - 240, opponents.trans.y + 105, 0));

		string inst = "ARROW UP - move Forward\nARROW DOWN - move Backwards\nARROW LEFT/RIGHT - rotate counter-clockwise/clockwise\n\'A\' - move Left\n\'D\' - move Right\nSPACEBAR - Fire Missiles!\nLeft-Mouse - Drag the ship around if you want it easy.";
		ofDrawBitmapString(inst, glm::vec3(ofGetWindowWidth() / 2.0 - 260, ofGetWindowHeight() - 200, 0));

		spaceFont.drawString("SPACE PEWLICE", (ofGetWindowWidth() / 2.0) - 550, 150);
		police.draw();
	}
	// if the game is over we display the score along with words of encouragement for the players that are having a tough time :D
	string str;
	str += "Score: " + to_string((int)score);
	if (gameOver) {
		gameOverFont.drawString("GAME OVER", (ofGetWindowWidth() / 2.0) - 260.0, (ofGetWindowHeight() / 2.0) - 150);
		ofSetColor(ofColor::white);
		if (score > 1000 && (ofGetElapsedTimeMillis() / 250) % 2) {
			ofSetColor(ofColor::gold);
		}
		if (score < 100 && (ofGetElapsedTimeMillis() / 750) % 2) {
			ofSetColor(ofColor::red);
		}
		origin.drawString(str, (ofGetWindowWidth() / 2.0) - 100.0, (ofGetWindowHeight() / 2.0) - 80);
		ofSetColor(ofColor::white);
		if ((ofGetElapsedTimeMillis() / 800) % 2) {
			ofSetColor(ofColor::slateGray);
			ofDrawBitmapString("PRESS \"R\" to RESTART", (ofGetWindowWidth() / 2.0) - 60.0, (ofGetWindowHeight() / 2.0) - 50);
			ofSetColor(ofColor::white);
		}
		string banter;
		if (score < 300) {
			ofSeedRandom(ofGetCurrentTime().seconds);
			int b = ((int)ofRandom(0, 9999) % 5);
			switch (b)
			{
			case 1:
				banter = "Pretty hard, huh?";
				break;
			case 2:
				banter = "Bet ya can't do better?";
				break;
			case 3:
				banter = "Oh, did you even try?";
				break;
			case 4:
				banter = "Wanna go again?";
				break;
			default:
				banter = "You got this! ... do you?";
				break;
			}

		}
		if (score >= 300 && score < 450) {
			banter = "Pff! That's it?";
		}
		if (score >= 450 && score < 500) {
			banter = "So close! Oof!";
		}
		if (score >= 500 && score < 1000) {
			banter = "... Wow you did really good, I guess";
		}
		if (score > 1000) {
			banter = "Alright, calm down a little!";
		}

		prototypeFont.drawString(banter, (ofGetWindowWidth() / 2.0) - ((banter.length() / 2) * 14), (ofGetWindowHeight() / 2.0) + 50);
		ofSetColor(ofColor::white);
	}

	// these get displayed during the play time of the Player, includes the Time, Score, and some instructions on Reset and Pause.
	else if (player->started) {
		ofSetColor(ofColor::white);
		if (gameTime / 1000 > 109) {
			if ((ofGetElapsedTimeMillis() / 500) % 2) {
				ofSetColor(ofColor::white);
			}
			else {
				ofSetColor(ofColor::red);
			}
		}
		clockFont.drawString("TIME: " + to_string(gameTime / 1000), (ofGetWindowWidth() / 2.0) - 200, 50);
		ofSetColor(ofColor::white);
		if ((ofGetElapsedTimeMillis() / 5000) % 2) {
			ofSetColor(ofColor::slateGray);
			ofDrawBitmapString("PRESS \"R\" to RESTART", 20, 30);
			ofSetColor(ofColor::white);
		}
		else {
			ofSetColor(ofColor::slateGray);
			ofDrawBitmapString("PRESS \"P\" to PAUSE", 20, 30);
			ofSetColor(ofColor::white);
		}
		ofSetColor(ofColor::white);
		if (score > 1000) {
			ofSetColor(ofColor::gold);
		}
		origin.drawString(str, ofGetWindowWidth() - 400, 50);
		ofSetColor(ofColor::white);
		ofDrawBitmapString("Penalty: " + to_string((int)(100 - penalty*100)) +"%", ofGetWindowWidth() - 400, 70);
		}
}

// Given x in pixel coordinates, return (x, y, z) on the sin wave
// Note that "z" is not used, so it is set to "0".
//
// Additional Parameters
//    scale - scales the curve in Y  (the amplitude)
//    cycles - number of cycles in sin wave.
//
glm::vec3 ofApp::curveEval(float x, float scale, float cycles)
{
	// x is in screen coordinates and his in [0, WindowWidth]
	float u = (cycles * x * PI) / ofGetWidth();
	return (glm::vec4(x, -scale * sin(u), 0, 0));
}




//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	//	cout << "mouse( " << x << "," << y << ")" << endl;
}

//--------------------------------------------------------------
/*
Logic is based on the mouse drag logic shared in class.
*/
void ofApp::mouseDragged(int x, int y, int button) {
	if (player->bSelected) {
		glm::vec3 mousePos = glm::vec3(x, y, 1);
		mouseDragVec = mousePos - mouse_last;
		lastPos = player->trans;
		player->trans += mouseDragVec;
		// this checks if the pos of the object being dragged is still within the borders of the window.
		if (player->trans.x >= ofGetWindowWidth() || player->trans.x <= 0 
			|| player->trans.y >= ofGetWindowHeight() || player->trans.y <= 0){
			player->trans = lastPos; // if so, the object is reverted back into its last position
}
		bMouseDragged = TRUE;
		mouse_last = mousePos;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	mouse_last = glm::vec3(x, y, 1);
	glm::vec3 mousePos = glm::vec3(x, y, 1);

	glm::mat4 invertSpritePos = glm::translate(glm::mat4(1.0), -player->trans);
	mousePos = invertSpritePos * glm::vec4(mousePos, 1);
	//cout << player->verts[0] << endl;
	glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(player->rot), glm::vec3(0, 0, 1));
	glm::mat4 scale = glm::scale(glm::mat4(1.0), player->scale);

	if (player->Emitter::inside(mousePos, rot * scale * glm::vec4(player->verts[0], 1),
		rot * scale * glm::vec4(player->verts[1], 1), rot * scale * glm::vec4(player->verts[2], 1), rot * scale * glm::vec4(player->verts[3], 1))) {

		player->bSelected = TRUE;
		//cout << "Inside by clicking!" << endl;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	player->bSelected = FALSE;
	bMouseDragged = FALSE;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

void ofApp::keyPressed(int key) {
	switch (key) {
	case 'C':
	case 'c':
		break;
	case 'F':
	case 'f':
		//ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		//bHide = !bHide;
		break;
	case 'r':
		resetGame = TRUE;
		gameOver = FALSE;
		break;
	case 'p':
		pauseGame = !pauseGame;
		break;
	case 's':
		break;
	case 'u':
		break;
	case OF_KEY_ALT:
		break;
	case OF_KEY_CONTROL:
		bControlRotation = TRUE;
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;
	case OF_KEY_LEFT:
		bCounterClockRot = TRUE;
		moving = TRUE;
		break;
	case OF_KEY_UP:
		bUpKeyPressed = TRUE;
		moving = TRUE;
		break;
	case OF_KEY_RIGHT:
		bClockRot = TRUE;
		moving = TRUE;
		break;
	case OF_KEY_DOWN:
		bDownKeyPressed = TRUE;
		moving = TRUE;
		break;
	case 'a':
		bLeftKeyPressed = TRUE;
		moving = TRUE;
		break;
	case 'd':
		bRightKeyPressed = TRUE;
		moving = TRUE;
		break;
	case ' ':
		// This waits for the first spacebar press before starting the emitter.
		// The emitter is also not drawable during the first idle stage.
		if (!player->started) {
			startTime = ofGetElapsedTimeMillis();
			player->start();
		}
		player->setRate(rate);
		for (int i = 0; i < enemyReinf.size(); i++) {
			if (!enemyReinf[i]->started) {
				enemyReinf[i]->start();
			}
		}
		
		for (int i = 0; i < friendlies.size(); i++) {
			if (!friendlies[i]->started) {
				friendlies[i]->start();
			}
		}
		if (enemies.size() > 0) {
			if (!enemies[0]->started) {
				enemies[0]->start();
			}
		}
		break;
	}
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch (key) {
		break;
	case OF_KEY_ALT:
		break;
	case OF_KEY_CONTROL:
		bControlRotation = FALSE;
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_LEFT:
		bCounterClockRot = FALSE;
		break;
	case OF_KEY_UP:
		bUpKeyPressed = FALSE;
		break;
	case OF_KEY_RIGHT:
		bClockRot = FALSE;
		break;
	case OF_KEY_DOWN:
		bDownKeyPressed = FALSE;
		break;
	case 'a':
		bLeftKeyPressed = FALSE;
		break;
	case 'd':
		bRightKeyPressed = FALSE;
		break;
	case 'r':
		resetGame = FALSE;
		break;
	case ' ':
		player->setRate(0);
		break;
	default:
		moving = FALSE;
	}

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

