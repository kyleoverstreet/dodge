#include <iostream>
#include <cstring>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "src/ppm.h"
#include "src/shared.h"
using namespace std;

void cleanupPPM(void) {
	system("rm ./images/background1.ppm");
	system("rm ./images/standL.ppm");
	system("rm ./images/standhelmL.ppm");
	system("rm ./images/starplayer.ppm");
	system("rm ./images/invinciblehelm.ppm");
	system("rm ./images/player2.ppm");
	system("rm ./images/full_hp.ppm");
	system("rm ./images/three_fourths_hp.ppm");
	system("rm ./images/half_hp.ppm");
	system("rm ./images/one_fourth_hp.ppm");
	system("rm ./images/no_hp.ppm");
	system("rm ./images/invincible_hp.ppm");
	system("rm ./images/Spike.ppm");
	system("rm ./images/helmet.ppm");
	system("rm ./images/Star.ppm");
	system("rm ./images/heart.ppm");
}

int movePlayer(int xres, Player *player) {

	player->pos[0] += player->vel[0];

	//Check if edge left
	if (player->pos[0] <= 40) {
		player->pos[0] = 40;
		player->vel[0] = 0;
	}

	//Check if edge right
	if (player->pos[0] >= xres-40) {
		player->pos[0] = xres-40;
		player->vel[0] = 0;
	}

	if (player->vel[0] < -3) { 
		player->vel[0] += 2;
	} else if (player->vel[0] > 3) {
		player->vel[0] += -2;
	} else if (player->vel[0] <= 3 && player->vel[0] >= -3) {
		player->vel[0] = 0;
	}

	//return player's x-position (needed to detect collisions)
	return player->pos[0];	
}

void init(int xres, int yres, Player *player) {
	player->pos[0] = xres/2;
	player->pos[1] = yres/920;
	VecCopy(player->pos, player->lastpos);
	MakeVector(-150.0, 180.0, 0.0, player->pos);
	MakeVector(0.0,0.0,0.0, player->vel);
}

void keypressL(Player *player) {
	player->vel[0] -= 3.5;
	player->LR = false;
}

void keypressR(Player *player) {
	player->vel[0] += 3.5;
	player->LR = true;
}
