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
	system("rm ./images/p1.ppm");
	system("rm ./images/p1Helm.ppm");
	system("rm ./images/p1Invinc.ppm");
	system("rm ./images/p1HelmInvinc.ppm");
	system("rm ./images/p2.ppm");
	system("rm ./images/p2Helm.ppm");
	system("rm ./images/p2Invinc.ppm");
	system("rm ./images/p2HelmInvinc.ppm");
	system("rm ./images/hp4.ppm");
	system("rm ./images/hp3.ppm");
	system("rm ./images/hp2.ppm");
	system("rm ./images/hp1.ppm");
	system("rm ./images/hp0.ppm");
	system("rm ./images/hpi.ppm");
	system("rm ./images/Spike.ppm");
	system("rm ./images/helmet.ppm");
	system("rm ./images/Star.ppm");
	system("rm ./images/heart.ppm");
}

void convertpng2ppm(void) {
    system("convert ./images/background1.jpg ./images/background1.ppm");
    system("convert ./images/p1.png ./images/p1.ppm");
    system("convert ./images/p1Helm.png ./images/p1Helm.ppm");
    system("convert ./images/p1Invinc.png ./images/p1Invinc.ppm");
    system("convert ./images/p1HelmInvinc.png ./images/p1HelmInvinc.ppm");
    system("convert ./images/p2.png ./images/p2.ppm");
    system("convert ./images/p2Helm.png ./images/p2Helm.ppm");
    system("convert ./images/p2Invinc.png ./images/p2Invinc.ppm");
    system("convert ./images/p2HelmInvinc.png ./images/p2HelmInvinc.ppm");
    system("convert ./images/hp4.png ./images/hp4.ppm");
    system("convert ./images/hp3.png ./images/hp3.ppm");
    system("convert ./images/hp2.png ./images/hp2.ppm");
    system("convert ./images/hp1.png ./images/hp1.ppm");
    system("convert ./images/hp0.png ./images/hp0.ppm");
    system("convert ./images/hpi.png ./images/hpi.ppm");
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

void keypressL(Player *player2) {
	player2->vel[0] -= 3.5;
	player2->LR = false;
}

void keypressR(Player *player2) {
	player2->vel[0] += 3.5;
	player2->LR = true;
}

void gamestart1p(Player *player, int xres) {
    player->pos[0] = xres/2;
    player->pos[1] = 30;
}

void gamestart2p(Player *player1, Player *player2, int xres) {
    player1->pos[0] = xres/2 - 50;
    player1->pos[1] = 30;
    player2->pos[0] = xres/2 + 50;
    player2->pos[1] = 30;
}
