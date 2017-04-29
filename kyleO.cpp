// Kyle Overstreet
// CMPS 3350
// Dodge Project
// Individual source code
// Last edit: 4/29/17

/****** WEEK 13 ******
Working on many things:

Intro animation
New displays
Score webpage
**********************/

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
void keypressA(Player*);
void keypressD(Player*);
void dropItems(int, int, bool, const int, const int);
void display_health(int, int);
void display_score(int, int);
void gamelog(string, int);
void view_scores();

extern void createSpikes(float, const int, const int);
extern void drawSpikes(void);
extern void deleteSpike(Spike*);
extern void createHelmets(float, const int, const int);
extern void drawHelmets(void);
extern void deleteHelmet(Helmet*);
extern void createStars(float, const int, const int);
extern void drawStars(void);
extern void deleteStar(Star*);
extern void spike_bounce(Spike* spike);
extern void createHeart(float, const int, const int);
extern void drawHeart(void);
extern void deleteHeart(Heart*);
extern bool check_helm_timer(bool helm);
extern bool start_helm_timer();
extern bool start_powerup_timer();
extern bool check_powerup_timer(bool powerup);
extern void menu(int, int);
#ifdef USE_OPENAL_SOUND
extern void play_helmet_hit();
extern void play_powerup();
extern void play_game_over();
extern void play_health_pickup();
extern void play_health_loss();
#endif

extern GLuint silhouetteSpike;
extern GLuint silhouetteHelm;
extern GLuint silhouetteStar;
extern GLuint silhouetteHeart;
extern GLuint hp4Texture;
extern GLuint hp3Texture;
extern GLuint hp2Texture;
extern GLuint hp1Texture;
extern GLuint hp0Texture;
extern GLuint hpiTexture;

extern Player *player;
extern Player *player2;
extern Spike *sphead;
extern Helmet *hhead;
extern Star *sthead;
extern Heart *hearthead;

const float timeslice = 1.0f;
const float gravity = -0.2f;
float drop_rate = 1.0;;
float spike_mod = 15.0;
int level = 1;
int level_change = 100;

extern string p1_name;
int p1_health = 4;
int p1_score = 0;
bool p1_helm = false;
bool p1_invincible = false;
bool p1_dead = false;
int p1_deadpos;

extern bool two_player;
extern string p2_name;
int p2_health = 4;
int p2_score = 0;
bool p2_helm = false;
bool p2_invincible = false;
bool p2_dead = false;
int p2_deadpos;

extern bool start_game;
extern int display_menu;

bool deleted_spike;
bool deleted_helm;
bool deleted_star;
bool deleted_heart;

// Display tutorial to screen (accessible via menu)
void tutorial(const int xres, const int yres)
{
	unsigned int white = 0xffffff;

	Rect r;
	r.bot = yres - 70;
	r.left = xres/2 - 200;
	r.center = 0;
	ggprint8b(&r, 16, white, "The goal in Dodge is to move the character");
	ggprint8b(&r, 16, white, "to avoid falling spikes for as long as possible");

	Rect r2;
	r2.bot = yres - 130;
	r2.left = xres/10;
	r2.center = 0;
	ggprint8b(&r2, 16, white, "Key controls:");
	ggprint8b(&r2, 16, white, "a - moves player left");
	ggprint8b(&r2, 16, white, "d - moves player right");
	ggprint8b(&r2, 16, white, "Left arrow - moves player2 left");
	ggprint8b(&r2, 16, white, "Right arrow - moves player2 right");

	Rect r3;
	r3.bot = yres - 130;
	r3.left = xres/2;
	r3.center = 0;
	ggprint8b(&r3, 16, white, "Items:");

	float w = 10.0;

	// Display spike
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef((xres/2)+10,yres-145,0);
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
	item1.bot = yres - 150;
	item1.left = xres/2 + 30;
	item1.center = 0;
	ggprint8b(&item1, 16, white, "Spike - hurts the player (removes 1/4 hp per hit)");

	// Display star
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef((xres/2)+10,yres-170,0);
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
	item2.bot = yres - 175;
	item2.left = xres/2 + 30;
	item2.center = 0;
	ggprint8b(&item2, 16, white, "Star - gives the player invincibility for 5 seconds");

	// Display helmet
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef((xres/2)+10,yres-195,0);
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
	item3.bot = yres - 200;
	item3.left = xres/2 + 30;
	item3.center = 0;
	ggprint8b(&item3, 16, white, "Helmet - protects the player from one spike hit");

	// Display heart
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef((xres/2)+10,yres-220,0);
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
	item4.bot = yres - 225;
	item4.left = xres/2 + 30;
	item4.center = 0;
	ggprint8b(&item4, 16, white, "Heart - player regains 1/4 hp if not full hp");
}

// Move player left
void keypressA(Player *player) {
	player->vel[0] -= 3.5;
	player->LR = false;
}

// Move player right
void keypressD(Player *player) {
	player->vel[0] += 3.5;
	player->LR = true;
}

void dropItems(int player_pos, int player2_pos, const int xres, const int yres)
{
	// Create the items
	if ((p1_score > level_change || p2_score > level_change) && level < 10) {
		spike_mod += 4;
		level_change += 100;
		level++;
		cout << "increasing drop rate to level " << level << endl;
	}
	
	if (random(100) < spike_mod) {
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
			if (!p1_invincible) {
				if (!p1_helm) {
					// Player1 is vulnerable
					p1_health--;
					if (p1_health > 0) {
#ifdef USE_OPENAL_SOUND
						play_health_loss();
#endif
					} else {
						// Player1 has no health
#ifdef USE_OPENAL_SOUND
						play_game_over();
#endif
						gamelog(p1_name, p1_score);
						p1_dead = true;
						p1_deadpos = player_pos;
						if (!two_player) {
							cout << "Game over" << endl;
							cout << "Score: " << p1_score;
							cout << endl << endl;
							//display_menu = 1;
							start_game = false;
						}
						if (two_player && p2_dead) {
							cout << "P1 score: " << p1_score;
							cout << "\nP2 score: " << p2_score;
							cout << endl << endl;
							cout << "Player 1 wins!" << endl;
							//display_menu = 1;
							start_game = false;
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
				if (!p2_invincible) {
					if (!p2_helm) {
						// Player2 is vulnerable
						p2_health--;
						if (p2_health > 0) {
#ifdef USE_OPENAL_SOUND
							play_health_loss();
#endif
						} else {
							// Player2 has no health
#ifdef USE_OPENAL_SOUND
							play_game_over();
#endif
							gamelog(p2_name, p2_score);
							p2_dead = true;
							p2_deadpos = player2_pos;
							if (p1_dead) {
								cout << "P1 score: " << p1_score;
								cout << "\nP2 score: " << p2_score;
								cout << endl << endl;
								cout << "Player 2 wins!" << endl;
								//display_menu = 1;
								start_game = false;
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
			// Spike hit ground
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
		int helm_decision = 0;
		int d1;
		int d2;
		if (((helmet->pos[1] > 0 && helmet->pos[1] < 68)) &&
			((helmet->pos[0] > player_pos-38) &&
			(helmet->pos[0] < player_pos+38)) &&
			(!p1_dead)) {
			helm_decision += 1;
			d1 = abs(helmet->pos[0] - player_pos);
		}
		if (two_player && !p2_dead) {
			if (((helmet->pos[1] > 0 && helmet->pos[1] < 68)) &&
				((helmet->pos[0] > player2_pos-38) &&
				(helmet->pos[0] < player2_pos+38))) {
				helm_decision += 2;
				d2 = abs(helmet->pos[0] - player2_pos);
			}
		}
		if (helm_decision == 1) {
		// Helmet landed on Player1
#ifdef USE_OPENAL_SOUND
			play_helmet_hit();
#endif
			p1_helm = true;
			deleteHelmet(helmet);
		} else if (helm_decision == 2) {
		// Helmet landed on Player2
#ifdef USE_OPENAL_SOUND
			play_helmet_hit();
#endif
			p2_helm = true;
			deleteHelmet(helmet);

		} else if (helm_decision == 3) {
			// Helmet landed on both players
#ifdef USE_OPENAL_SOUND
			play_helmet_hit();
#endif
			// Give helmet to closest player
			if (d1 <= d2) {
				p1_helm = true;
			} else {
				p2_helm = true;
			}
			deleteHelmet(helmet);
		}
		if (helmet->pos[1] < -20.0f) {
			// Helmet hit ground
			Helmet *savehelm = helmet->next;
			deleteHelmet(helmet);
			helmet = savehelm;
			continue;
		}
		helmet = helmet->next;
	}

	star = sthead;
	while (star) {
		if (((star->pos[1] > 0 && star->pos[1] < 68)) &&
				((star->pos[0] > player_pos-38) &&
				 (star->pos[0] < player_pos+38)) &&
				(!p1_dead)) {
			// Star landed on Player1
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
				// Star landed on Player2
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
			// Star hit ground
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
			// Heart landed on Player1

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
				// Heart landed on Player2

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
			// Heart hit ground
			Heart *saveheart = heart->next;
			deleteHeart(heart);
			heart = saveheart;
			continue;
		}
		heart = heart->next;
		deleted_heart = false;
	}

	// Check the timers for the powerup and helmet
	//p1_helm = check_helm_timer(p1_helm);
	p1_invincible = check_powerup_timer(p1_invincible);
	if (two_player) {
		//p2_helm = check_helm_timer(p2_helm);
		p2_invincible = check_powerup_timer(p2_invincible);
	}
}

// Display player health(s) at top of window
void display_health(int xres, int yres)
{
	int x;
	int y;
	int x2;
	unsigned int white = 0xffffff;
	unsigned int yellow = 0x00dddd00;

	// Set up coordinates for Player1's name/health bar based on game mode
	if (!two_player) {
		x = xres/2;
		y = yres - 40;
	} else {
		x = 100;
		y = yres - 40;

		x2 = xres - 100;
	}

	// Player1 name
	const char* p1_cstr = p1_name.c_str();
	Rect n;
	n.bot = y + 10;
	n.left = x - 40;
	n.center = 0;
	ggprint13(&n, 16, yellow, p1_cstr);

	// Player1 health bar
	Rect h;
	h.bot = y - 10;
	h.left = x - 65;
	h.center = 0;
	ggprint13(&h, 16, white, "HP");

	GLuint p1_hpBar;

	// Set up appropriate Player1 health bar texture
	if (p1_invincible) {
		p1_hpBar = hpiTexture;
	} else if (p1_health == 4) {
		p1_hpBar = hp4Texture;
	} else if (p1_health == 3) {
		p1_hpBar = hp3Texture;
	} else if (p1_health == 2) {
		p1_hpBar = hp2Texture;
	} else if (p1_health == 1) {
		p1_hpBar = hp1Texture;
	} else if (p1_health == 0) {
		p1_hpBar = hp0Texture;
	}		

	// Display Player1 health bar
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(x,y,0);
	glBindTexture(GL_TEXTURE_2D, p1_hpBar);
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

	if (two_player) {

		// Player2 name
		const char* p2_cstr = p2_name.c_str();
		Rect n;
		n.bot = y + 10;
		n.left = x2 - 40;
		n.center = 0;
		ggprint13(&n, 16, yellow, p2_cstr);

		// Player2 health bar
		Rect h2;
		h2.bot = y - 10;
		h2.left = x2 - 65;
		h2.center = 0;
		ggprint13(&h2, 16, white, "HP");

		GLuint p2_hpBar;

		// Set up appropriate Player2 health bar texture
		if (p2_invincible) {
			p2_hpBar = hpiTexture;
		} else if (p2_health == 4) {
			p2_hpBar = hp4Texture;
		} else if (p2_health == 3) {
			p2_hpBar = hp3Texture;
		} else if (p2_health == 2) {
			p2_hpBar = hp2Texture;
		} else if (p2_health == 1) {
			p2_hpBar = hp1Texture;
		} else if (p2_health == 0) {
			p2_hpBar = hp0Texture;
		}

		// Display Player2 health bar	
		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		glTranslatef(x2,y,0);
		glBindTexture(GL_TEXTURE_2D, p2_hpBar);
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

// Displays the player score(s) under health bar
void display_score(int xres, int yres)
{
	unsigned int yellow = 0x00dddd00;

	if (!two_player) {
		Rect r;
		r.bot = yres - 70;
		r.left = xres/2 - 25;
		r.center = 0;
		ggprint10(&r, 16, yellow, "Score: %i", p1_score);
	} else {
		Rect r;
		r.bot = yres - 70;
		r.left = 75;
		r.center = 0;
		ggprint10(&r, 16, yellow, "Score: %i", p1_score);

		Rect r2;
		r2.bot = yres - 70;
		r2.left = xres - 125;
		r2.center = 0;
		ggprint10(&r2, 16, yellow, "Score: %i", p2_score);
	}
}

// Appends player name, score, and date to a gamelog via PHP script
// PHP script executes a separate program to ouput gamelog data to HTML webpage
// View scores at cs.csubak.edu/~koverstreet/3350/dodge/scores.html
void gamelog(string player, int score)
{
	stringstream ss;
	ss << score;
	string score_str = ss.str();

	string command =
		"curl http://cs.csubak.edu/\\~koverstreet/3350/dodge/update_scores.php";
	command += "\\?name=" + player;
	command += "\\&score=" + score_str;

	system(command.c_str());
}

void view_scores()
{
	system("firefox http://cs.csubak.edu/~koverstreet/3350/dodge/scores.html");
}
