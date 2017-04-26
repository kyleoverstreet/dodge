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

void convertpng2ppm(void) {
    system("convert ./images/background1.jpg ./images/background1.ppm");
    system("convert ./images/standL.png ./images/standL.ppm");
    system("convert ./images/standhelmL.png ./images/standhelmL.ppm");
    system("convert ./images/starplayer.png ./images/starplayer.ppm");
    system("convert ./images/invinciblehelm.png ./images/invinciblehelm.ppm");
    system("convert ./images/player2.png ./images/player2.ppm"); 
    system("convert ./images/full_hp.png ./images/full_hp.ppm");
    system("convert ./images/three_fourths_hp.png ./images/three_fourths_hp.ppm");
    system("convert ./images/half_hp.png ./images/half_hp.ppm");
    system("convert ./images/one_fourth_hp.png ./images/one_fourth_hp.ppm");
    system("convert ./images/no_hp.png ./images/no_hp.ppm");
    system("convert ./images/invincible_hp.png ./images/invincible_hp.ppm");
    system("convert ./images/Spike.png ./images/Spike.ppm");
    system("convert ./images/helmet.png ./images/helmet.ppm");
    system("convert ./images/Star.png ./images/Star.ppm");
    system("convert ./images/heart.png ./images/heart.ppm");
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

void keypressL(Player *player) {
	player->vel[0] -= 3.5;
	player->LR = false;
}

void keypressR(Player *player) {
	player->vel[0] += 3.5;
	player->LR = true;
}

void gamestart1p(Player *player, int xres) {
    player->pos[0] = xres/2;
    player->pos[1] = 30;
}

void gamestart2p(Player *player1, Player *player2, int xres) {
    player1->pos[0] = xres/2 - 50;
    player1->pos[1] = 30;
    player2->pos[0] = xres/2 - 50;
    player2->pos[1] = 30;
}
