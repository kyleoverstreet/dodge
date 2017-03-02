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
#include "./libraries/ppm.h"

using namespace std;

#define MakeVector(x, y, z, v) (v)[0]=(x), (v)[1]=(y), (v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]

typedef double Flt;
typedef double Vec[3];
typedef struct t_player {
    Vec pos;
    Vec vel;
    Vec lastpos;
} Player;

void cleanupPPM(void) {
    system("rm ./images/standL.ppm");
    system("rm ./images/PixelBG.ppm");
    system("rm ./images/StandR.ppm");
}

void movePlayer(int xres, Player *player) {
    //Check if edge left
    if (player->pos[0] <= 40) {
	player->pos[0] = 40;
    }

    //Check if edge right
    if (player->pos[0] >= xres-40) {
	player->pos[0] = xres-40;
    }
}

void init(int xres, int yres, Player *player) {
    player->pos[0] = xres/2;
    player->pos[1] = yres/920;
    VecCopy(player->pos, player->lastpos);
    MakeVector(-150.0, 180.0, 0.0, player->pos);
    MakeVector(5.0,0.0,0.0, player->vel);
}

void keypressL(Player *player) {
    player->pos[0] -= 8.0;
}

void keypressR(Player *player) {
    player->pos[0] += 8.0;
}

void jacob () {
    cout << "This is Jacob's test function" << endl;
    return;
}
