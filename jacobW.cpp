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
				system("rm ./images/Spike.ppm");
				//system("rm ./images/walking1.ppm");
				//system("rm ./images/walking2.ppm");
				system("rm ./images/helmet.ppm");
				system("rm ./images/Star.ppm");;
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

				// return player's x-position (needed to detect collisions)
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
				//mv == 1 is walking1.ppm
				if (player->mv == 1) {
								//mv == 2 is walking2.ppm
								player->mv = 2;
				} else { 
								player->mv = 1;
				}

				player->LR = false;
}

void keypressR(Player *player) {
				player->vel[0] += 3.5;

				if (player->mv == 1) {
								player->mv = 2;
				} else {
								player->mv = 1;
				}

				player->LR = true;
}

void jacob () {
				cout << "This is Jacob's test function" << endl;
				return;
}
