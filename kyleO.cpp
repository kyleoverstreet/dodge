// Kyle Overstreet
// CMPS 3350
// Dodge Project
// Individual source code
// Last edit: 4/25/17

/*
 *************** NOTE TO GORDON *****************
 Young helped write the dropItems function below

 ====================WEEK 11====================
 -Made many changes to our main source file (src/dodge.cpp).
 -Cleaned up project's repo. Removed files, renamed files, etc.
 -Helped Young with his create items functions (dodge.cpp and youngO.cpp)

 ====================SPRING BREAK===============
 -Removed large sound/images files from project repo.
 -Fixed all compiling errors.
 -Removed unnecessary code from src/dodge.cpp.
 -Implemented collision detection between player and items.
 -Added star item.
 -Added player "status" display for testing purposes.
 -Fixed black box behind falling items!!! (finally)
 -Implemented image of player with helmet on.

 =====================WEEK 12====================
 -Created heart image and implemented heart collision detection
 -Implemented communication with webpage
 (see cs.csubak.edu/~koverstreet/3350/dodge for additional code)
 -Added tutorial
 -Implemented second player and added item collision & effects
 */

#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <time.h>
#include <vector>
#include "src/ppm.h"
#include "src/shared.h"
#include "src/youngO.h"

extern "C" {
#include "src/fonts.h"
}

using namespace std;

void tutorial(const int, const int);
void init2(int, int, Player*);
void keypressA(Player*);
void keypressD(Player*);
int movePlayer2(int, Player*);
void dropItems(int, int, bool, const int, const int);
void display_score(int, int);
void display_collisions(int, int);
void display_player_status(int, int);
void gamelog();

#ifdef USE_OPENAL_SOUND
extern void play_helmet_hit();
extern void play_powerup();
extern void play_game_over();
extern void play_health_pickup();
#endif
extern void createSpikes(const int, const int, const int);
extern void drawSpikes(void);
extern void deleteSpike(Spike*);
extern void createHelmets(const int, const int, const int);
extern void drawHelmets(void);
extern void deleteHelmet(Helmet*);
extern void createStars(const int, const int, const int);
extern void drawStars(void);
extern void deleteStar(Star*);
extern void spike_bounce(Spike* spike);
extern void createHeart(const int, const int, const int);
extern void drawHeart(void);
extern void deleteHeart(Heart*);
extern bool check_helm_timer(bool helm);
extern bool start_helm_timer();
extern bool start_powerup_timer();
extern bool check_powerup_timer(bool powerup);


extern Ppmimage full_hpImage;

extern GLuint silhouetteSpike;
extern GLuint silhouetteHelm;
extern GLuint silhouetteStar;
extern GLuint silhouetteHeart;
extern GLuint full_hpTexture;
extern GLuint three_fourths_hpTexture;
extern GLuint half_hpTexture;
extern GLuint one_fourth_hpTexture;
extern GLuint no_hpTexture;
extern GLuint invincible_hpTexture;

extern Player *player;
extern Player *player2;
extern Spike *sphead;
extern Helmet *hhead;
extern Star *sthead;
extern Heart *hearthead;
extern int score;
extern string player_name;

const float timeslice = 1.0f;
const float gravity = -0.2f;
int drop_rate = 1; // use higher num to increase drop rate

int spike_collisions = 0;
int helm_collisions = 0;
int star_collisions = 0;
int heart_collisions = 0;

bool deleted_spike;
bool deleted_helm;
bool deleted_star;
bool deleted_heart;

int p1_health = 4;
int p1_score = 0;
bool p1_helm = false;
bool p1_invincible = false;
bool p1_dead = false;

extern bool two_player;
int p2_health = 4;
int p2_score = 0;
bool p2_helm = false;
bool p2_invincible = false;
bool p2_dead = false;

void tutorial(const int xres, const int yres)
{

	unsigned int red = 0xff0000;
	//unsigned int green = 0x00ff00;
	//unsigned int yellow = 0x00dddd00;
	unsigned int white = 0xffffff;

	Rect temp;
	temp.bot = yres - 150;
	temp.left = xres/2 + 100;
	temp.center = 0;
	ggprint8b(&temp, 16, red, "TUTORIAL IS CURRENTLY A WORK-IN-PROGRESS");

	Rect r;
	r.bot = yres - 70;
	r.left = xres/10;
	r.center = 0;
	ggprint8b(&r, 16, white, "The goal in Dodge is to move the character");
	ggprint8b(&r, 16, white, "to avoid falling spikes for as long as possible");

	Rect r2;
	r2.bot = yres - 130;
	r2.left = xres/10;
	r2.center = 0;
	ggprint8b(&r2, 16, white, "Key controls:");
	ggprint8b(&r2, 16, white, "Left arrow - moves character left");
	ggprint8b(&r2, 16, white, "Right arrow - moves character right");

	Rect r3;
	r3.bot = yres - 200;
	r3.left = xres/10;
	r3.center = 0;
	ggprint8b(&r3, 16, white, "Items:");

	float w = 10.0;

	// Draw spike
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef((xres/10)+10,yres-215,0);
	glBindTexture(GL_TEXTURE_2D, silhouetteSpike);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(-w,-w);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(-w, w);
	glTexCoord2f(0.0f, 0.0f); glVertex2i( w, w);
	glTexCoord2f(0.0f, 1.0f); glVertex2i( w,-w);
	glEnd();
	glPopMatrix();

	// Spike description
	Rect item1;
	item1.bot = yres - 220;
	item1.left = xres/10 + 30;
	item1.center = 0;
	ggprint8b(&item1, 16, white, "Spike - hurts the player (removes 1/4 hp per hit)");

	// Draw star
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef((xres/10)+10,yres-240,0);
	glBindTexture(GL_TEXTURE_2D, silhouetteStar);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(-w,-w);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(-w, w);
	glTexCoord2f(0.0f, 0.0f); glVertex2i( w, w);
	glTexCoord2f(0.0f, 1.0f); glVertex2i( w,-w);
	glEnd();
	glPopMatrix();

	// Star description
	Rect item2;
	item2.bot = yres - 245;
	item2.left = xres/10 + 30;
	item2.center = 0;
	ggprint8b(&item2, 16, white, "Star - gives the player invincibility for 5 seconds");

	// Draw helmet
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef((xres/10)+10,yres-265,0);
	glBindTexture(GL_TEXTURE_2D, silhouetteHelm);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(-w,-w);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(-w, w);
	glTexCoord2f(0.0f, 0.0f); glVertex2i( w, w);
	glTexCoord2f(0.0f, 1.0f); glVertex2i( w,-w);
	glEnd();
	glPopMatrix();

	// Helmet description
	Rect item3;
	item3.bot = yres - 270;
	item3.left = xres/10 + 30;
	item3.center = 0;
	ggprint8b(&item3, 16, white, "Helmet - protects the player from one spike hit");

	// Draw heart
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef((xres/10)+10,yres-290,0);
	glBindTexture(GL_TEXTURE_2D, silhouetteHeart);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(-w,-w);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(-w, w);
	glTexCoord2f(0.0f, 0.0f); glVertex2i( w, w);
	glTexCoord2f(0.0f, 1.0f); glVertex2i( w,-w);
	glEnd();
	glPopMatrix();

	// Heart description
	Rect item4;
	item4.bot = yres - 295;
	item4.left = xres/10 + 30;
	item4.center = 0;
	ggprint8b(&item4, 16, white, "Heart - player regains 1/4 hp if not full hp");
}

/*void init2(int xres, int yres, Player *player2)
  {
  p2_health = 4;
  p2_score = 0;
  p2_helm_status = false;
  p2_invincible = false;
  player2->pos[0] = xres/2 + 80;
  player2->pos[1] = yres/920;
  VecCopy(player2->pos, player2->lastpos);
  MakeVector(-150.0, 180.0, 0.0, player2->pos);
  MakeVector(0.0,0.0,0.0, player2->vel);
  }*/

void keypressA(Player *player2) {
	player2->vel[0] -= 3.5;
	player2->LR = false;
}

void keypressD(Player *player2) {
	player2->vel[0] += 3.5;
	player2->LR = true;
}

int movePlayer2(int xres, Player *player2) {

	player2->pos[0] += player2->vel[0];

	//Check if edge left
	if (player2->pos[0] <= 40) {
		player2->pos[0] = 40;
		player2->vel[0] = 0;
	}

	//Check if edge right
	if (player2->pos[0] >= xres-40) {
		player2->pos[0] = xres-40;
		player2->vel[0] = 0;
	}

	if (player2->vel[0] < -3) {
		player2->vel[0] += 2;
	} else if (player2->vel[0] > 3) {
		player2->vel[0] += -2;
	} else if (player2->vel[0] <= 3 && player2->vel[0] >= -3) {
		player2->vel[0] = 0;
	}

	//return player2's x-position (needed to detect collisions)
	return player2->pos[0];
}

void dropItems(int player_pos, int player2_pos, bool two_player, const int xres, const int yres)
{
	// Create the items
	if (random(100) < 15) {
		createSpikes(drop_rate, xres, yres);
	}
	if (random(200) < 1.3) {
		createHelmets(drop_rate, xres, yres);
	}
	if (random(200) < 1.3) {
		createStars(drop_rate, xres, yres);

	}
	if (random(200) < 1.3) {
		createHeart(drop_rate, xres, yres);
	}

	// Move items on screen
	Spike *spike = sphead;
	while (spike) {
		// Force is toward the ground
		spike->vel[1] += gravity;
		VecCopy(spike->pos, spike->lastpos);
		spike->pos[0] += spike->vel[0] * timeslice;
		spike->pos[1] += spike->vel[1] * timeslice;
		if (fabs(spike->vel[1]) > spike->maxvel[1]) {
			spike->vel[1] *= 0.96;
		}
		spike->vel[0] *= 0.999;
		spike = spike->next;
	}

	Helmet *helmet = hhead;
	while (helmet) {
		// Force is toward the ground
		helmet->vel[1] += gravity;
		VecCopy(helmet->pos, helmet->lastpos);
		helmet->pos[0] += helmet->vel[0] * timeslice;
		helmet->pos[1] += helmet->vel[1] * timeslice;
		if (fabs(helmet->vel[1]) > helmet->maxvel[1]) {
			helmet->vel[1] *= 0.96;
		}
		helmet->vel[0] *= 0.999;
		helmet = helmet->next;
	}

	Star *star = sthead;
	while (star) {
		// Force is toward the ground
		star->vel[1] += gravity;
		VecCopy(star->pos, star->lastpos);
		star->pos[0] += star->vel[0] * timeslice;
		star->pos[1] += star->vel[1] * timeslice;
		if (fabs(star->vel[1]) > star->maxvel[1]) {
			star->vel[1] *= 0.96;
		}
		star->vel[0] *= 0.999;
		star = star->next;
	}

	Heart *heart = hearthead;
	while (heart) {
		// Force is toward the ground
		heart->vel[1] += gravity;
		VecCopy(heart->pos, heart->lastpos);
		heart->pos[0] += heart->vel[0] * timeslice;
		heart->pos[1] += heart->vel[1] * timeslice;
		if (fabs(heart->vel[1]) > heart->maxvel[1]) {
			heart->vel[1] *= 0.96;
		}
		heart->vel[0] *= 0.999;
		heart = heart->next;
	}

	// Check item positions (for collision with player or off-screen)
	spike = sphead;
	while (spike) {
		if (((spike->pos[1] > 0 && spike->pos[1] < 68)) &&
				((spike->pos[0] > player_pos-38) &&
				(spike->pos[0] < player_pos+38)) &&
				(!p1_dead)) {
			// Spike has hit Player1
			spike_collisions++;
			if (!p1_invincible) {
				if (!p1_helm) {
					// Player1 is vulnerable
					p1_health--;
					if (p1_health > 0) {
#ifdef USE_OPENAL_SOUND
						// PLAY SPIKE SOUND HERE
#endif
					} else {
						// Player1 has no health
#ifdef USE_OPENAL_SOUND
						play_game_over();
#endif
						gamelog();
						p1_dead = true;
						if (!two_player) {
							cout << "Game over" << endl;
							cout << "Score: " << p1_score;
						}
						if (two_player && p2_dead) {
							cout << "P1 score" << p1_score;
							cout << "\nP2 score: " << p2_score;
							cout << endl << endl;
							cout << "Player 1 wins!" << endl;
						}
					}
					deleteSpike(spike);
					deleted_spike = true;
				} else {
					// Spike hit Player1's helmet
#ifdef USE_OPENAL_SOUND
					play_helmet_hit();
#endif
					spike_bounce(spike);
					p1_helm = false;
				}
			}
		}
		if (two_player && !p2_dead) {
			if (((spike->pos[1] > 0 && spike->pos[1] < 68)) &&
					((spike->pos[0] > player2_pos-38) &&
					(spike->pos[0] < player2_pos+38))) {
				// Spike has hit Player2
				spike_collisions++;
				if (!p2_invincible) {
					if (!p2_helm) {
						// Player2 is vulnerable
						p2_health--;
						if (p2_health > 0) {
#ifdef USE_OPENAL_SOUND
							// PLAY SPIKE SOUND HERE
#endif
						} else {
							// Player2 has no health
#ifdef USE_OPENAL_SOUND
							play_game_over();
#endif
							gamelog();
							p2_dead = true;
							if (p1_dead) {
								cout << "P1 score: " << p1_score;
								cout << "\nP2 score: " << p2_score;
								cout << endl << endl;
								cout << "Player 2 wins!" << endl;
							}
						}
						if (!deleted_spike) {
							deleteSpike(spike);
						}
					} else {
						// Spike hit Player2's helmet
#ifdef USE_OPENAL_SOUND
						play_helmet_hit();
#endif
						spike_bounce(spike);
						p2_helm = false;
					}
				}
			}
		}
		if (spike->pos[1] < -20.0f) {
			// Spike has hit ground
			Spike *savespike = spike->next;
			deleteSpike(spike);
			spike = savespike;
			continue;
		}
		spike = spike->next;
		deleted_spike = false;
	}

	helmet = hhead;
	while (helmet) {
		if (((helmet->pos[1] > 0 && helmet->pos[1] < 68)) &&
				((helmet->pos[0] > player_pos-38) &&
				(helmet->pos[0] < player_pos+38)) &&
				(!p1_dead)) {
			// Helmet has landed on Player1
#ifdef USE_OPENAL_SOUND
			play_helmet_hit();
#endif
			helm_collisions++;
			p1_helm = start_helm_timer();
			deleteHelmet(helmet);
			deleted_helm = true;
		}
		if (two_player && !p2_dead) {
			if (((helmet->pos[1] > 0 && helmet->pos[1] < 68)) &&
					((helmet->pos[0] > player2_pos-38) &&
					(helmet->pos[0] < player2_pos+38))) {
				// Helmet has landed on Player2
#ifdef USE_OPENAL_SOUND
				play_helmet_hit();
#endif
				helm_collisions++;
				p2_helm = start_helm_timer();
				if (!deleted_helm) {
					deleteHelmet(helmet);
				}
			}
		}
		if (helmet->pos[1] < -20.0f) {
			// Helmet has hit ground
			Helmet *savehelm = helmet->next;
			deleteHelmet(helmet);
			helmet = savehelm;
			continue;
		}
		helmet = helmet->next;
		deleted_helm = false;
	}

	star = sthead;
	while (star) {
		if (((star->pos[1] > 0 && star->pos[1] < 68)) &&
				((star->pos[0] > player_pos-38) &&
				(star->pos[0] < player_pos+38)) &&
				(!p1_dead)) {
			// Star has hit landed on Player1
			star_collisions++;
#ifdef USE_OPENAL_SOUND
			play_powerup();
#endif
			p1_invincible = start_powerup_timer();
			deleteStar(star);
			deleted_star = true;
		}
		if (two_player && !p2_dead) {
			if (((star->pos[1] > 0 && star->pos[1] < 68)) &&
					((star->pos[0] > player2_pos-38) &&
					(star->pos[0] < player2_pos+38))) {
				// Star has landed on Player2
				star_collisions++;
#ifdef USE_OPENAL_SOUND
				play_powerup();
#endif
				p2_invincible = start_powerup_timer();
				if (!deleted_star) {
					deleteStar(star);
				}
			}

		}
		if (star->pos[1] < -20.0f) {
			// Star has hit ground
			Star *savestar = star->next;
			deleteStar(star);
			star = savestar;
			continue;
		}
		star = star->next;
		deleted_star = false;
	}

	heart = hearthead;
	while (heart) {
		if (((heart->pos[1] > 0 && heart->pos[1] < 68)) &&
				((heart->pos[0] > player_pos-38) &&
				(heart->pos[0] < player_pos+38)) &&
				(!p1_dead)) {
			// Heart has landed on Player1
			heart_collisions++;

			if (p1_health != 4) {
#ifdef USE_OPENAL_SOUND
				play_health_pickup();
#endif
				p1_health++;
			}
			deleteHeart(heart);
			deleted_heart = true;
		}
		if (two_player && !p2_dead) {

			if (((heart->pos[1] > 0 && heart->pos[1] < 68)) &&
					((heart->pos[0] > player2_pos-38) &&
					(heart->pos[0] < player2_pos+38))) {
				// Heart has landed on Player2
				heart_collisions++;

				if (p2_health != 4) {
#ifdef USE_OPENAL_SOUND
					play_health_pickup();
#endif
					p2_health++;
				}
				if (!deleted_heart) {
					deleteHeart(heart);
				}
			}
		}
		if (heart->pos[1] < -20.0f) {
			// Heart has hit ground
			Heart *saveheart = heart->next;
			deleteHeart(heart);
			heart = saveheart;
			continue;
		}
		heart = heart->next;
		deleted_heart = false;
	}

	// Check the timers for the powerup and helmet
	p1_helm = check_helm_timer(p1_helm);
	p1_invincible = check_powerup_timer(p1_invincible);
	if (two_player) {
		p2_helm = check_helm_timer(p2_helm);
		p2_invincible = check_powerup_timer(p2_invincible);
	}
}

// Display player health(s) at top-center
void display_health(int xres, int yres)
{
	int x;
	int y;
	int x2;
	int y2;
	unsigned int white = 0xffffff;
	
	// Set up coordinates for Player1's health bar based on game mode
	if (!two_player) {
		x = xres/2;
		y = yres - 20;
	} else {
		x = 100;
		y = yres - 20;

		x2 = xres - 100;
		y2 = yres - 20;
	}
	
	Rect h;
	h.bot = y - 10;
	h.left = x - 65;
	h.center = 0;
	ggprint13(&h, 16, white, "HP");
	
	// Player1 health bar
	if (p1_invincible) {
		// Invincible HP
		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		glTranslatef(x,y,0);
		glBindTexture(GL_TEXTURE_2D, invincible_hpTexture);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(-45,-18);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(-45, 18);
		glTexCoord2f(1.0f, 0.0f); glVertex2i( 45, 18);
		glTexCoord2f(1.0f, 1.0f); glVertex2i( 45,-18);
		glEnd();
		glPopMatrix();
	} else if (p1_health == 4) {
		// Full HP
		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		glTranslatef(x,y,0);
		glBindTexture(GL_TEXTURE_2D, full_hpTexture);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(-45,-18);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(-45, 18);
		glTexCoord2f(1.0f, 0.0f); glVertex2i( 45, 18);
		glTexCoord2f(1.0f, 1.0f); glVertex2i( 45,-18);
		glEnd();
		glPopMatrix();
	} else if (p1_health == 3) {
		// 3/4 HP
		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		glTranslatef(x,y,0);
		glBindTexture(GL_TEXTURE_2D, three_fourths_hpTexture);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(-45,-18);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(-45, 18);
		glTexCoord2f(1.0f, 0.0f); glVertex2i( 45, 18);
		glTexCoord2f(1.0f, 1.0f); glVertex2i( 45,-18);
		glEnd();
		glPopMatrix();
	} else if (p1_health == 2) {
		// 1/2 HP
		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		glTranslatef(x,y,0);
		glBindTexture(GL_TEXTURE_2D, half_hpTexture);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(-45,-18);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(-45, 18);
		glTexCoord2f(1.0f, 0.0f); glVertex2i( 45, 18);
		glTexCoord2f(1.0f, 1.0f); glVertex2i( 45,-18);
		glEnd();
		glPopMatrix();
	} else if (p1_health == 1) {
		// 1/4 HP
		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		glTranslatef(x,y,0);
		glBindTexture(GL_TEXTURE_2D, one_fourth_hpTexture);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(-45,-18);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(-45, 18);
		glTexCoord2f(1.0f, 0.0f); glVertex2i( 45, 18);
		glTexCoord2f(1.0f, 1.0f); glVertex2i( 45,-18);
		glEnd();
		glPopMatrix();
	} else if (p1_health == 0) {
		// No HP
		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		glTranslatef(x,y,0);
		glBindTexture(GL_TEXTURE_2D, no_hpTexture);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(-45,-18);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(-45, 18);
		glTexCoord2f(1.0f, 0.0f); glVertex2i( 45, 18);
		glTexCoord2f(1.0f, 1.0f); glVertex2i( 45,-18);
		glEnd();
		glPopMatrix();
	}

	if (two_player) {
		Rect h2;
		h2.bot = y2 - 10;
		h2.left = x2 - 65;
		h2.center = 0;
		ggprint13(&h2, 16, white, "HP");

		// Player2 health bar
		if (p2_invincible) {
			// Invincible HP
			glColor3f(1.0f, 1.0f, 1.0f);
			glPushMatrix();
			glTranslatef(x2,y2,0);
			glBindTexture(GL_TEXTURE_2D, invincible_hpTexture);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(255,255,255,255);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(-45,-18);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(-45, 18);
			glTexCoord2f(1.0f, 0.0f); glVertex2i( 45, 18);
			glTexCoord2f(1.0f, 1.0f); glVertex2i( 45,-18);
			glEnd();
			glPopMatrix();
		} else if (p2_health == 4) {
			// Full HP
			glColor3f(1.0f, 1.0f, 1.0f);
			glPushMatrix();
			glTranslatef(x2,y2,0);
			glBindTexture(GL_TEXTURE_2D, full_hpTexture);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(255,255,255,255);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(-45,-18);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(-45, 18);
			glTexCoord2f(1.0f, 0.0f); glVertex2i( 45, 18);
			glTexCoord2f(1.0f, 1.0f); glVertex2i( 45,-18);
			glEnd();
			glPopMatrix();
		} else if (p2_health == 3) {
			// 3/4 HP
			glColor3f(1.0f, 1.0f, 1.0f);
			glPushMatrix();
			glTranslatef(x2,y2,0);
			glBindTexture(GL_TEXTURE_2D, three_fourths_hpTexture);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(255,255,255,255);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(-45,-18);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(-45, 18);
			glTexCoord2f(1.0f, 0.0f); glVertex2i( 45, 18);
			glTexCoord2f(1.0f, 1.0f); glVertex2i( 45,-18);
			glEnd();
			glPopMatrix();
		} else if (p2_health == 2) {
			// 1/2 HP
			glColor3f(1.0f, 1.0f, 1.0f);
			glPushMatrix();
			glTranslatef(x2,y2,0);
			glBindTexture(GL_TEXTURE_2D, half_hpTexture);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(255,255,255,255);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(-45,-18);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(-45, 18);
			glTexCoord2f(1.0f, 0.0f); glVertex2i( 45, 18);
			glTexCoord2f(1.0f, 1.0f); glVertex2i( 45,-18);
			glEnd();
			glPopMatrix();
		} else if (p2_health == 1) {
			// 1/4 HP
			glColor3f(1.0f, 1.0f, 1.0f);
			glPushMatrix();
			glTranslatef(x2,y2,0);
			glBindTexture(GL_TEXTURE_2D, one_fourth_hpTexture);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(255,255,255,255);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(-45,-18);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(-45, 18);
			glTexCoord2f(1.0f, 0.0f); glVertex2i( 45, 18);
			glTexCoord2f(1.0f, 1.0f); glVertex2i( 45,-18);
			glEnd();
			glPopMatrix();
		} else if (p2_health == 0) {
			// No HP
			glColor3f(1.0f, 1.0f, 1.0f);
			glPushMatrix();
			glTranslatef(x2,y2,0);
			glBindTexture(GL_TEXTURE_2D, no_hpTexture);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(255,255,255,255);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(-45,-18);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(-45, 18);
			glTexCoord2f(1.0f, 0.0f); glVertex2i( 45, 18);
			glTexCoord2f(1.0f, 1.0f); glVertex2i( 45,-18);
			glEnd();
			glPopMatrix();
		}
	}
}

// Displays the player score(s) at top-center
void display_score(int xres, int yres)
{
	unsigned int yellow = 0x00dddd00;

	if (!two_player) {
		Rect r;
		r.bot = yres - 50;
		r.left = xres/2 - 25;
		r.center = 0;
		ggprint10(&r, 16, yellow, "Score: %i", p1_score);
	} else {
		Rect r;
		r.bot = yres - 50;
		r.left = 75;
		r.center = 0;
		ggprint10(&r, 16, yellow, "Score: %i", p1_score);

		Rect r2;
		r2.bot = yres - 50;
		r2.left = xres - 125;
		r2.center = 0;
		ggprint10(&r2, 16, yellow, "Score: %i", p2_score);
	}
}

// Displays collisions with player (for testing purposes)
void display_collisions(int xres, int yres)
{
	Rect r;
	r.bot = yres - 20;
	r.left = xres - 198;
	r.center = 0;
	unsigned int white = 0xffffff;
	ggprint8b(&r, 16, white, "Collisions (for testing):");
	ggprint8b(&r, 16, white, "Spikes - %i", spike_collisions);
	ggprint8b(&r, 16, white, "Stars - %i", star_collisions);
	ggprint8b(&r, 16, white, "Helmets - %i", helm_collisions);
	ggprint8b(&r, 16, white, "Hearts - %i", heart_collisions);
}

// Displays helmet and invincibility status (for testing purposes)
void display_player_status(int xres, int yres)
{
	Rect r;
	r.bot = yres - 110;
	r.left = xres - 198;
	r.center = 0;
	unsigned int red = 0xff0000;
	unsigned int green = 0x00ff00;
	unsigned int yellow = 0x00dddd00;
	ggprint8b(&r, 16, 0xffffff, "Status (for testing):");
	if (p1_helm) {
		ggprint8b(&r, 16, green, "Helmet ON");
	} else {
		ggprint8b(&r, 16, red, "No helmet");
	}
	if (p1_invincible) {
		ggprint8b(&r, 16, yellow, "Invincible");
	} else {
		ggprint8b(&r, 16, red, "Not invincible");
	}
}

// Append player name, score, and date to gamelog via PHP script
// Viewable at cs.csubak.edu/~koverstreet/3350/dodge/scores.txt
void gamelog()
{
	stringstream ss;
	ss << p1_score;
	string score_str = ss.str();

	string command =
		"curl http://cs.csubak.edu/\\~koverstreet/3350/dodge/update_scores.php";
	command += "\\?name=" + player_name;
	command += "\\&score=" + score_str;

	system(command.c_str());
}
