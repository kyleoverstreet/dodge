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

extern GLuint deathTexture;
extern GLuint logoTexture;
extern GLuint mainmenuTexture;
extern bool game_started;

void cleanupPPM(void) {
    system("rm ./images/credits.ppm");
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
    system("rm ./images/death.ppm");
    system("rm ./images/DodgeLogo.ppm");
    system("rm ./images/Menu.ppm");
}

void convertpng2ppm(void) {
    system("convert ./images/credits.jpg ./images/credits.ppm");
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
    system("convert ./images/death.png ./images/death.ppm");
    system("convert ./images/DodgeLogo.png ./images/DodgeLogo.ppm");
    system("convert ./images/Menu.png ./images/Menu.ppm");
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
    game_started = true;
}

void gamestart2p(Player *player1, Player *player2, int xres) {
    player1->pos[0] = xres/2 - 50;
    player1->pos[1] = 30;
    player2->pos[0] = xres/2 + 50;
    player2->pos[1] = 30;
    game_started = true;
}

void tombstone(int x)
{
    // Display tombstone at x-position
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(x,25,0);
    glBindTexture(GL_TEXTURE_2D, deathTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-25,-25);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-25, 25);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( 25, 25);
    glTexCoord2f(1.0f, 1.0f); glVertex2i( 25,-25);
    glEnd();
    glPopMatrix();
}

void logo(int xres, int y)
{
    //display logo at (x,y)
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(xres/2,y,0);
    glBindTexture(GL_TEXTURE_2D, logoTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-100,-25);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-100, 25);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( 100, 25);
    glTexCoord2f(1.0f, 1.0f); glVertex2i( 100,-25);
    glEnd();
    glPopMatrix();
}
    
void mainmenu(int xres, int y)
{
    //display menu at (x,y)
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(xres/2,y,0);
    glBindTexture(GL_TEXTURE_2D, mainmenuTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-100,-25);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-100, 25);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( 100, 25);
    glTexCoord2f(1.0f, 1.0f); glVertex2i( 100,-25);
    glEnd();
    glPopMatrix();
}
