// Kyle Overstreet
// CMPS 3350
// Dodge Project
// Individual source code
// Last edit: 4/17/17

/*

 *************** NOTE TO GORDON *****************
 Young helped write the dropItems function below

 ====================WEEK 11====================
 -Made many changes to our main source file (src/dodge.cpp).
 -Cleaned up project's repo. Removed files, renamed files, etc.
 -Helped Young with his create items functions (dodge.cpp and youngO.cpp)

 ====================SPRING BREAK====================
 -Removed large sound/images files from project repo.
 -Fixed all compiling errors.
 -Removed unnecessary code from src/dodge.cpp.
 -Implemented collision detection between player and items.
 -Added star item.
 -Added player "status" display for testing purposes.
 -Fixed black box behind falling items!!! (finally)
 -Implemented image of player with helmet on.
 */

#include <ctime>
#include <fstream>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>
#include "src/shared.h"
#include "src/youngO.h"

extern "C" {
#include "src/fonts.h"
}

using namespace std;

void dropItems(int, const int, const int);
void display_score(int, int);
void display_collisions(int, int);
void display_player_status(int, int);
void gamelog(string, int);
void upload_scores();

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

extern Spike *sphead;
extern Helmet *hhead;
extern Star *sthead;
extern Heart *hearthead;
extern int score;

const float timeslice = 1.0f;
const float gravity = -0.2f;
int drop_rate = 1; // use higher num to increase drop rate
int spike_collisions = 0;
int helm_collisions = 0;
int star_collisions = 0;
int heart_collisions = 0;
bool helm_status = false;
bool invincible = false;
int health = 3;

void dropItems(int player_pos, const int xres, const int yres)
{
	// Create the items
	if (random(100) < 15) {
		createSpikes(drop_rate, xres, yres);
	}
	if (random(200) < 1.5) {
		createHelmets(drop_rate, xres, yres);
	}
	if (random(200) < 1.5) {
		createStars(drop_rate, xres, yres);
	}
	if (random(200) < 1.5)
		createHeart(drop_rate, xres, yres);
		// For now, player is invincible until another star drops
		invincible = false;
	

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
		// old was y < 80 and x-radius of 40
		if (((spike->pos[1] > 0 && spike->pos[1] < 70)) &&
				((spike->pos[0] > player_pos-40) &&
				 (spike->pos[0] < player_pos+40))) {
			// Spike has hit player
			spike_collisions++;
			if (!invincible) {
				if (helm_status == false) {
					// Spike hit vulnerable player - game over
					health--;
					if (health == 0) {
#ifdef USE_OPENAL_SOUND
						play_game_over();
#endif
						cout << "Game over! (console msg only for now)" << endl;
						cout << "Score: " << score << endl << endl;
						score = 0;
						health = 3;
					}
					deleteSpike(spike);
				} else {
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
		if (((helmet->pos[1] > 0 && helmet->pos[1] < 70)) &&
				((helmet->pos[0] > player_pos-40) &&
				 (helmet->pos[0] < player_pos+40))) {
			// Helmet has hit player
#ifdef USE_OPENAL_SOUND
			play_helmet_hit();
#endif
			helm_collisions++;
			helm_status = true;
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
		if (((star->pos[1] > 0 && star->pos[1] < 70)) &&
				((star->pos[0] > player_pos-40) &&
				 (star->pos[0] < player_pos+40))) {
			// Star has hit player
			star_collisions++;
#ifdef USE_OPENAL_SOUND
			play_powerup();
#endif
			// TO DO: set invincibility for x seconds
			invincible = true;
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
		if (((heart->pos[1] > 0 && heart->pos[1] < 70)) &&
				((heart->pos[0] > player_pos-40) &&
				 (heart->pos[0] < player_pos+40))) {
			// Heart has hit player
			heart_collisions++;
			
			if (health != 3) {
#ifdef USE_OPENAL_SOUND
			//some sound here;
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
}

// Display player health at top-center
void display_health(int xres, int yres)
{	
	Rect r;
	r.bot = yres - 35;
	//r.left = xres - 722;
	r.left = xres/2 - 55;
	r.center = 0;
	unsigned int red = 0xff0000;
	ggprint16(&r, 16, red, "Health:  %i / 3", health);
}

// Displays the player score at top-center
void display_score(int xres, int yres)
{
	Rect r;
	r.bot = yres - 50;
	//r.left = xres - 700;
	r.left = xres/2 - 27;
	r.center = 0;
	unsigned int color = 0x00dddd00;
	ggprint13(&r, 16, color, "Score: %i", score);
}

// Displays collisions with player (for testing purposes)
void display_collisions(int xres, int yres)
{
	/*//this was to check player height/width
	Rect r2;
	r2.bot = yres  112;
	r2.left = xres - 500;
	r2.center = 0;
	ggprint8b(&r2, 16, 0xff0000, "w");

	Rect r3;
	r3.bot = yres  112;
	r3.left = xres - 564;
	r3.center = 0;
	ggprint8b(&r3, 16, 0xff0000, "w");*/
	
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

/** The following two functions do not yet work as intended!! **/

// Append player name, score, and date to gamelog
void gamelog(string p1, int score1)
{
	// Setup for date output
	tm *my_time;
	time_t t = time(NULL);
	my_time = localtime(&t);

	ofstream f;
	f.open("gamelog.txt", ofstream::app);
	f << p1 << " "
		<< score1 << " "
		<< my_time->tm_mon+1 << "/"
		<< my_time->tm_mday << "/"
		<< my_time->tm_year+1900 << endl;
	f.close();
}

// Upload the gamelog to an html webpage
void upload_scores()
{
	cout << "Ran Kyle's upload_scores function." << endl
		<< "See \"scores.html\". This file will eventually upload to web server" << endl
		<< "Webpage will look like this: http://cs.csubak.edu/~koverstreet/3350/dodge/scores.html" << endl;

	vector<string> players;
	vector<int> scores;
	vector<string> dates;

	string p;
	int s;
	string d;

	// Store data from gamelog into vectors
	ifstream ifs("gamelog.txt");
	while (true) {
		ifs >> p >> s >> d;
		if (ifs.eof()) {
			break;
		}
		players.push_back(p);
		scores.push_back(s);
		dates.push_back(d);
	}
	ifs.close();

	ofstream ofs("scores.html");
	ofs << 
		"<html>\n"
		"<head>\n"
		"<script src=\"sorttable.js\" type=\"text/javascript\"></script>\n"
		"<title>Dodge Scores</title>\n"
		"<style>\n"
		"body {\n"
		"    background-color: #75b3ad;\n"
		"}\n"
		"table {\n"
		"    font-family: arial, sans-serif;\n"
		"    border-collapse: collapse;\n"
		"    width: 100%\n"
		"}\n"
		"td, th {\n"
		"    border: 1px solid #dddddd;\n"
		"    text-align: left;\n"
		"    padding: 8px\n"
		"}\n"
		"tr:nth-child(even) {\n"
		"    background-color: #dddddd;"
		"}\n"
		"</style>\n"
		"</head>\n"
		"<body>\n"
		"<h2>Dodge Scores</h2>\n"
		"<table>\n"
		"<table class=\"sortable\">\n"
		"<tr>\n"
		"<th>Player</th>\n"
		"<th>Score</th>\n"
		"<th>Date</th>\n"
		"</tr>\n";
	// Setup table row for each line in gamelog
	for (int i = players.size()-1; i >= 0; i--) {
		ofs << 
			"<tr>\n"
			"<td>" << players[i] << "</td>\n"
			"<td>" << scores[i] << "</td>\n"
			"<td>" << dates[i] << "</td>\n"
			"</tr>\n";
	}
	ofs <<
		"</table>\n"
		"</body>\n"
		"</html>";
	ofs.close();
}
