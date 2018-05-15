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
extern GLuint gameoverTexture;
extern int p1_health;
extern int p1_score;
extern bool p1_dead;
extern int p2_health;
extern int p2_score;
extern bool p2_dead;
extern bool game_over;

extern Ppmimage *bgImage;
extern Ppmimage *bgTransImage;
extern Ppmimage *logoImage;
extern Ppmimage *playerImage;
extern Ppmimage *playerHelmImage;
extern Ppmimage *playerInvincibleImage;
extern Ppmimage *playerHelmInvincImage;
extern Ppmimage *player2Image;
extern Ppmimage *player2HelmImage;
extern Ppmimage *player2InvincibleImage;
extern Ppmimage *player2HelmInvincImage;
extern Ppmimage *spikeImage;
extern Ppmimage *helmetImage;
extern Ppmimage *starImage;
extern Ppmimage *heartImage;
extern Ppmimage *hp4Image;
extern Ppmimage *hp3Image;
extern Ppmimage *hp2Image;
extern Ppmimage *hp1Image;
extern Ppmimage *hp0Image;
extern Ppmimage *hpiImage;
extern Ppmimage *deathImage;
extern Ppmimage *gameoverImage;

extern GLuint bgTexture;
extern GLuint bgTransTexture;
extern GLuint playerTexture;
extern GLuint playerHelmTexture;
extern GLuint playerInvincibleTexture;
extern GLuint playerHelmInvincTexture;
extern GLuint player2Texture;
extern GLuint player2HelmTexture;
extern GLuint player2InvincibleTexture;
extern GLuint player2HelmInvincTexture;
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

void cleanupPPM(void) {
    remove("./images/credits.ppm");
    remove("./images/background1.ppm");
    remove("./images/p1.ppm");
    remove("./images/p1Helm.ppm");
    remove("./images/p1Invinc.ppm");
    remove("./images/p1HelmInvinc.ppm");
    remove("./images/p2.ppm");
    remove("./images/p2Helm.ppm");
    remove("./images/p2Invinc.ppm");
    remove("./images/p2HelmInvinc.ppm");
    remove("./images/hp4.ppm");
    remove("./images/hp3.ppm");
    remove("./images/hp2.ppm");
    remove("./images/hp1.ppm");
    remove("./images/hp0.ppm");
    remove("./images/hpi.ppm");
    remove("./images/Spike.ppm");
    remove("./images/helmet.ppm");
    remove("./images/Star.ppm");
    remove("./images/heart.ppm");
    remove("./images/death.ppm");
    remove("./images/DodgeLogo.ppm");
    remove("./images/GameOver.ppm");
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
    system("convert ./images/GameOver.png ./images/GameOver.ppm");
}

void getImage(void) {
    // Background images
    bgImage = ppm6GetImage("./images/background1.ppm");
    bgTransImage = ppm6GetImage("./images/transparent.ppm");

    // Logo image
    logoImage = ppm6GetImage("./images/DodgeLogo.ppm");

    // Player1 images
    playerImage = ppm6GetImage("./images/p1.ppm");
    playerHelmImage  = ppm6GetImage("./images/p1Helm.ppm");
    playerInvincibleImage = ppm6GetImage("./images/p1Invinc.ppm");
    playerHelmInvincImage = ppm6GetImage("./images/p1HelmInvinc.ppm");

    // Player2 images
    player2Image = ppm6GetImage("./images/p2.ppm");
    player2HelmImage = ppm6GetImage("./images/p2Helm.ppm");
    player2InvincibleImage = ppm6GetImage("./images/p2Invinc.ppm");
    player2HelmInvincImage = ppm6GetImage("./images/p2HelmInvinc.ppm");

    // Item images
    spikeImage = ppm6GetImage("./images/Spike.ppm");
    helmetImage = ppm6GetImage("./images/helmet.ppm");
    starImage = ppm6GetImage("./images/Star.ppm");
    heartImage = ppm6GetImage("./images/heart.ppm");

    // Health bar images
    hp4Image = ppm6GetImage("./images/hp4.ppm");
    hp3Image = ppm6GetImage("./images/hp3.ppm");
    hp2Image = ppm6GetImage("./images/hp2.ppm");
    hp1Image = ppm6GetImage("./images/hp1.ppm");
    hp0Image = ppm6GetImage("./images/hp0.ppm");
    hpiImage = ppm6GetImage("./images/hpi.ppm");

    // Tombstone image
    deathImage = ppm6GetImage("./images/death.ppm");

    // Game Over Image
    gameoverImage = ppm6GetImage("./images/GameOver.ppm");

}

void generateTextures(void) {
    glGenTextures(1, &playerTexture);
    glGenTextures(1, &playerHelmTexture);
    glGenTextures(1, &playerInvincibleTexture);
    glGenTextures(1, &playerHelmInvincTexture);
    glGenTextures(1, &player2Texture);
    glGenTextures(1, &player2HelmTexture);
    glGenTextures(1, &player2InvincibleTexture);
    glGenTextures(1, &player2HelmInvincTexture);
    glGenTextures(1, &deathTexture);
    glGenTextures(1, &gameoverTexture);
    glGenTextures(1, &logoTexture);
    glGenTextures(1, &hp4Texture);
    glGenTextures(1, &hp3Texture);
    glGenTextures(1, &hp2Texture);
    glGenTextures(1, &hp1Texture);
    glGenTextures(1, &hp0Texture);
    glGenTextures(1, &hpiTexture);
    glGenTextures(1, &silhouetteSpike);
    glGenTextures(1, &silhouetteHelm);
    glGenTextures(1, &silhouetteStar);
    glGenTextures(1, &silhouetteHeart);
    glGenTextures(1, &bgTexture);
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
    game_over = false;
    player->vel[0] = 0;
    player->pos[0] = xres/2;
    player->pos[1] = 30;
    p1_health = 4;
    p1_score = 0;
    p1_dead = false;
    p2_dead = false;
}

void gamestart2p(Player *player1, Player *player2, int xres) {
    game_over = false;
    player1->vel[0] = 0;
    player1->pos[0] = xres/2 - 100;
    player1->pos[1] = 30;
    p1_health = 4;
    p1_score = 0;
    p1_dead = false;
    player2->vel[0] = 0;
    player2->pos[0] = xres/2 + 100;
    player2->pos[1] = 30;
    p2_health = 4;
    p2_score = 0;
    p2_dead = false;
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
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-100,-30);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-100, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( 100, 30);
    glTexCoord2f(1.0f, 1.0f); glVertex2i( 100,-30);
    glEnd();
    glPopMatrix();
}

void gameover(int xres, int y)
{
    //display menu at (x,y)
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(xres/2,y,0);
    glBindTexture(GL_TEXTURE_2D, gameoverTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-125,-50);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-125, 50);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( 125, 50);
    glTexCoord2f(1.0f, 1.0f); glVertex2i( 125,-50);
    glEnd();
    glPopMatrix();
}
