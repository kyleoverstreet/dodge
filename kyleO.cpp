// Kyle Overstreet
// CMPS 3350
// Dodge Project
// Individual source code
// Last edit: 4/23/17

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
void movePlayer2(int, Player*);
void dropItems(int, const int, const int);
void display_score(int, int);
void display_collisions(int, int);
void display_player_status(int, int);
void gamelog();

#ifdef USE_OPENAL_SOUND
extern void play_helmet_hit();
extern void play_powerup();
extern void play_game_over();
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
bool helm_status = false;
bool invincible = false;
int health = 4;

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

void init2(int xres, int yres, Player *player2)
{
    player2->pos[0] = xres/2 + 80;
    player2->pos[1] = yres/920;
    VecCopy(player2->pos, player2->lastpos);
    MakeVector(-150.0, 180.0, 0.0, player2->pos);
    MakeVector(0.0,0.0,0.0, player2->vel);
}

void keypressA(Player *player2) {
    player2->vel[0] -= 3.5;
    player2->LR = false;
}

void keypressD(Player *player2) {
    player2->vel[0] += 3.5;
    player2->LR = true;
}

void movePlayer2(int xres, Player *player2) {

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
    //return player2->pos[0];
}

void dropItems(int player_pos, const int xres, const int yres)
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
		 (spike->pos[0] < player_pos+38))) {
	    // Spike has hit player
	    spike_collisions++;
	    if (!invincible) {
		if (helm_status == false) {
		    // Spike hit vulnerable player
		    health--;
		    if (health > 0) {
#ifdef USE_OPENAL_SOUNDD
			// PLAY SPIKE SOUND HERE
#endif
		    } else {
			// Player has no health
#ifdef USE_OPENAL_SOUND
			play_game_over();
#endif
			gamelog();
			cout << "Game over!" << endl;
			cout << "Score: " << score;
			cout << endl << endl;
			score = 0;
			health = 4;
		    }
		    deleteSpike(spike);
		} else {
		    // Spike hit helmet
#ifdef USE_OPENAL_SOUND
		    play_helmet_hit();
#endif
		    spike_bounce(spike);
		    helm_status = false;
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
    }

    helmet = hhead;
    while (helmet) {
	if (((helmet->pos[1] > 0 && helmet->pos[1] < 68)) &&
		((helmet->pos[0] > player_pos-38) &&
		 (helmet->pos[0] < player_pos+38))) {
	    // Helmet has hit player
#ifdef USE_OPENAL_SOUND
	    play_helmet_hit();
#endif
	    helm_collisions++;
	    helm_status = start_helm_timer();
	    deleteHelmet(helmet);
	}
	if (helmet->pos[1] < -20.0f) {
	    // Helmet has hit ground
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
		 (star->pos[0] < player_pos+38))) {
	    // Star has hit player
	    star_collisions++;
#ifdef USE_OPENAL_SOUND
	    play_powerup();
#endif
	    invincible = start_powerup_timer();
	    deleteStar(star);
	}
	if (star->pos[1] < -20.0f) {
	    // Star has hit ground
	    Star *savestar = star->next;
	    deleteStar(star);
	    star = savestar;
	    continue;
	}
	star = star->next;
    }

    heart = hearthead;
    while (heart) {
	if (((heart->pos[1] > 0 && heart->pos[1] < 68)) &&
		((heart->pos[0] > player_pos-38) &&
		 (heart->pos[0] < player_pos+38))) {
	    // Heart has hit player
	    heart_collisions++;

	    if (health != 4) {
#ifdef USE_OPENAL_SOUND
		// PLAY HEALTH SOUND HERE
#endif
		health++;
	    }
	    deleteHeart(heart);
	}
	if (heart->pos[1] < -20.0f) {
	    // Heart has hit ground
	    Heart *saveheart = heart->next;
	    deleteHeart(heart);
	    heart = saveheart;
	    continue;
	}
	heart = heart->next;
    }

    // Check the timers for the powerup and helmet
    helm_status = check_helm_timer(helm_status);
    invincible = check_powerup_timer(invincible);
}

// Display player health at top-center
void display_health(int xres, int yres)
{	
    /*Rect r;
      r.bot = yres - 35;
      r.left = xres/2 - 55;
      r.center = 0;
      unsigned int red = 0xff0000;
      ggprint16(&r, 16, red, "Health:  %i / 4", health);*/

    if (invincible) {
	// Invincible HP
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef((xres/2)-30,yres-20,0);
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
    } else if (health == 4) {
	// Full HP
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef((xres/2)-30,yres-20,0);
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
    } else if (health == 3) {
	// 3/4 HP
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef((xres/2)-30,yres-20,0);
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
    } else if (health == 2) {
	// 1/2 HP
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef((xres/2)-30,yres-20,0);
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
    } else if (health == 1) {
	// 1/4 HP
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef((xres/2)-30,yres-20,0);
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
    } else if (health == 0) {
	// No HP
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef((xres/2)-30,yres-20,0);
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

// Displays the player score at top-center
void display_score(int xres, int yres)
{
    Rect r;
    r.bot = yres - 50;
    r.left = xres/2 - 63;
    r.center = 0;
    unsigned int color = 0x00dddd00;
    ggprint13(&r, 16, color, "Score: %i", score);
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
    if (helm_status == true) {
	ggprint8b(&r, 16, green, "Helmet ON");
    } else {
	ggprint8b(&r, 16, red, "No helmet");
    }
    if (invincible == true) {
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
    ss << score;
    string score_str = ss.str();

    string command =
	"curl http://cs.csubak.edu/\\~koverstreet/3350/dodge/update_scores.php";
    command += "\\?name=" + player_name;
    command += "\\&score=" + score_str;

    system(command.c_str());
}
