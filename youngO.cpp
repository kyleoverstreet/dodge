// Young Soo Oh
// CMPS 3350
// Dodge Project

//********************************************************************
//* intro - two characters moving ramdomly before start game         *
//* 	  - blinking "press "m" to start"                            *
//*       - basically my intro connects to menu interface            *
//* count down - count down numbers shows up right before start game *
//* create, drop, and delete items (spikes, helmets, starts, hearts) *
//********************************************************************

#include <iostream>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <stdlib.h>
#include "src/log.h"
#include "src/ppm.h"
#include "src/shared.h"
using namespace std;

extern Spike *sphead;
extern Helmet *hhead;
extern Star *sthead;
extern Heart *hearthead;
extern Ppmimage *spikeImage;
extern Ppmimage *helmetImage;
extern Ppmimage *starImage;
extern Ppmimage *heartImage;
extern GLuint spikeTexture;
extern GLuint helmetTexture;
extern GLuint starTexture;
extern GLuint heartTexture;
extern GLuint silhouetteSpike;
extern GLuint silhouetteHelm;
extern GLuint silhouetteStar;
extern GLuint silhouetteHeart;
extern bool two_player;
extern int p1_score;
extern int p2_score;
extern bool p1_dead;
extern bool p2_dead;
extern bool blinkon;
extern bool blinkoff;
extern bool countdown_done;
extern int movePlayer(int xres, Player *player);
extern void keypressA(Player *player);
extern void keypressD(Player *player);
extern void keypressR(Player *player2);
extern void keypressL(Player *player2);
extern bool check_countDown_timer();

extern void play_one();
extern void play_two();
extern void play_three();
extern void play_go();

bool one = true;
bool two = true;
bool three = true;
bool four = true;

extern "C" {
#include "src/fonts.h"
}

bool start_notext_timer(); 
void countDown3(int, int);
void countDown2(int, int);
void countDown1(int, int);
void countDown0(int, int);

timespec txt_start, txt_current;
timespec countdown_start, countdown_current;

extern void createSpikes(float n, const int xres, const int yres)
{
    //create new spikes drops...
    int i;
    for (i = 0; i < n; i++) {
	Spike *node = (Spike *)malloc(sizeof(Spike));
	if (node == NULL) {
	    Log("error allocating node.\n");
	    exit(EXIT_FAILURE);
	}
	node->prev = NULL;
	node->next = NULL;
	node->sound=0;
	node->pos[0] = rnd() * (float)xres;
	node->pos[1] = rnd() * 100.0f + (float) yres;
	VecCopy(node->pos, node->lastpos);
	node->vel[0] = node->vel[1] = 0.0f;
	node->linewidth = spikeImage->width;
	//larger linewidth = faster speed
	node->maxvel[1] = (float) (node->linewidth*16);
	node->length = node->maxvel[1] * 0.2f;
	//put spikes into linked list
	node->next = sphead;
	if (sphead != NULL)
	    sphead->prev = node;
	sphead = node;
    }
}

extern void drawSpikes(void)
{
    Spike *node = sphead;
    while (node) {
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(node->pos[0],node->pos[1],node->pos[2]);
	glBindTexture(GL_TEXTURE_2D, silhouetteSpike);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);

	float w = 10.0;
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(-w,-w);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(-w, w);
	glTexCoord2f(0.0f, 0.0f); glVertex2i( w, w);
	glTexCoord2f(0.0f, 1.0f); glVertex2i( w,-w);
	glEnd();
	glPopMatrix();
	node = node->next;
    }
    glLineWidth(1);
}

void deleteSpike(Spike *node)
{
    if (node->prev == NULL && node->next == NULL) {
	sphead = NULL;
    }
    else if (node->prev == NULL) {
	sphead = node->next;
	node->next->prev = NULL;
    }
    else if (node->next == NULL) {
	node->prev->next = NULL;
    }
    else {
	node->next->prev = node->prev;
	node->prev->next = node->next;
    }

    // Free the node's memory and set node to NULL
    delete node;
    node = NULL;

    if (!p1_dead) {
	p1_score++;
    }
    if (two_player && !p2_dead) {
	p2_score++;
    }
}

extern void createHelmets(float n, const int xres, const int yres)
{
    //create new Helmets drops...
    int i;
    for (i = 0; i < n; i++) {
	Helmet *node = (Helmet *)malloc(sizeof(Helmet));
	if (node == NULL) {
	    Log("error allocating node.\n");
	    exit(EXIT_FAILURE);
	}
	node->prev = NULL;
	node->next = NULL;
	node->sound=0;
	node->pos[0] = rnd() * (float)xres;
	node->pos[1] = rnd() * 100.0f + (float) yres;
	VecCopy(node->pos, node->lastpos);
	node->vel[0] = node->vel[1] = 0.0f;
	node->linewidth = helmetImage->width;
	//larger linewidth = faster speed
	node->maxvel[1] = (float) (node->linewidth*16);
	node->length = node->maxvel[1] * 0.2f;
	//put helmets into linked list
	node->next = hhead;
	if (hhead != NULL)
	    hhead->prev = node;
	hhead = node;
    }
}

extern void drawHelmets(void)
{
    Helmet *node = hhead;
    while (node) {
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(node->pos[0],node->pos[1],node->pos[2]);
	glBindTexture(GL_TEXTURE_2D, silhouetteHelm);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	float w = 10.0;
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(-w,-w);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(-w, w);
	glTexCoord2f(0.0f, 0.0f); glVertex2i( w, w);
	glTexCoord2f(0.0f, 1.0f); glVertex2i( w,-w);
	glEnd();
	glPopMatrix();
	node = node->next;
    }
    glLineWidth(1);
}

extern void deleteHelmet(Helmet *node)
{
    //only
    if (node->prev == NULL && node->next == NULL) {
	hhead = NULL;
    }
    //beginning
    else if (node->prev == NULL){
	hhead = node->next;
	node->next->prev = NULL;
    }
    //end
    else if (node->next == NULL){
	node->prev->next = NULL;
    }
    //node is somewhere else
    else{
	node->next->prev = node->prev;
	node->prev->next = node->next;
    }
    // Free the node's memory and set node to NULL
    delete node;
    node = NULL;
}

extern void createStars(float n, const int xres, const int yres)
{
    int i;
    for (i = 0; i < n; i++) {
	Star *node = (Star *)malloc(sizeof(Star));
	if (node == NULL) {
	    Log("error allocating node.\n");
	    exit(EXIT_FAILURE);
	}
	node->prev = NULL;
	node->next = NULL;
	node->sound=0;
	node->pos[0] = rnd() * (float)xres;
	node->pos[1] = rnd() * 100.0f + (float) yres;
	VecCopy(node->pos, node->lastpos);
	node->vel[0] = node->vel[1] = 0.0f;
	node->linewidth = starImage->width;
	//larger linewidth = faster speed
	node->maxvel[1] = (float) (node->linewidth*16);
	node->length = node->maxvel[1] * 0.2f;
	//put stars into linked list
	node->next = sthead;
	if (sthead != NULL)
	    sthead->prev = node;
	sthead = node;
    }
}

extern void drawStars(void)
{
    Star *node = sthead;
    while (node) {
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(node->pos[0],node->pos[1],node->pos[2]);
	glBindTexture(GL_TEXTURE_2D, silhouetteStar);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	float w = 10.0;
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(-w,-w);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(-w, w);
	glTexCoord2f(0.0f, 0.0f); glVertex2i( w, w);
	glTexCoord2f(0.0f, 1.0f); glVertex2i( w,-w);
	glEnd();
	glPopMatrix();
	node = node->next;
    }
    glLineWidth(1);
}

void deleteStar(Star *node)
{
    if (node->prev == NULL && node->next == NULL) {
	sthead = NULL;
    }
    else if (node->prev == NULL) {
	sthead = node->next;
	node->next->prev = NULL;
    }
    else if (node->next == NULL) {
	node->prev->next = NULL;
    }
    else {
	node->next->prev = node->prev;
	node->prev->next = node->next;
    }

    // Free the node's memory and set node to NULL
    delete node;
    node = NULL;
}
//----------- create heart ---------------
extern void createHeart(float n, const int xres, const int yres)
{
    //create new rain drops...
    int i;
    for (i = 0; i < n; i++) {
	Heart *node = (Heart *)malloc(sizeof(Heart));
	if (node == NULL) {
	    Log("error allocating node.\n");
	    exit(EXIT_FAILURE);
	}
	node->prev = NULL;
	node->next = NULL;
	node->sound=0;
	node->pos[0] = rnd() * (float)xres;
	node->pos[1] = rnd() * 100.0f + (float) yres;
	VecCopy(node->pos, node->lastpos);
	node->vel[0] = node->vel[1] = 0.0f;
	node->linewidth = heartImage->width;
	//larger linewidth = faster speed
	node->maxvel[1] = (float) (node->linewidth*16);
	node->length = node->maxvel[1] * 0.2f;
	//put raindrop into linked list
	node->next = hearthead;
	if (hearthead != NULL)
	    hearthead->prev = node;
	hearthead = node;
    }
}

extern void drawHeart(void)
{
    Heart *node = hearthead;
    while (node) {
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(node->pos[0],node->pos[1],node->pos[2]);
	glBindTexture(GL_TEXTURE_2D, silhouetteHeart);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	float w = 10.0;
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(-w,-w);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(-w, w);
	glTexCoord2f(0.0f, 0.0f); glVertex2i( w, w);
	glTexCoord2f(0.0f, 1.0f); glVertex2i( w,-w);
	glEnd();
	glPopMatrix();
	node = node->next;
    }
    glLineWidth(1);
}

void deleteHeart(Heart *node)
{
    if (node->prev == NULL && node->next == NULL) {
	hearthead = NULL;
    }
    else if (node->prev == NULL) {
	hearthead = node->next;
	node->next->prev = NULL;
    }
    else if (node->next == NULL) {
	node->prev->next = NULL;
    }
    else {
	node->next->prev = node->prev;
	node->prev->next = node->next;
    }

    // Free the node's memory and set node to NULL
    delete node;
    node = NULL;
}
//characters move randomly
void moveRandomly(Player *player, Player *player2)
{
    srand(time(NULL));

    movePlayer(800, player);
    int p1_goPosition = random(800);
    int p1_currentPosition = player->pos[0];
    if (abs(p1_goPosition - p1_currentPosition) > 200) {
	// Move 200 pixels minimum to avoid jerky movement
	if (p1_goPosition < p1_currentPosition) {
	    keypressA(player);
	} else {
	    keypressR(player);
	}
    }

    movePlayer(800, player2);
    int p2_goPosition = random(800);
    int p2_currentPosition = player2->pos[0];
    if (abs(p2_goPosition - p2_currentPosition) > 200) {
	// Move 200 pixels minimum to avoid jerky movement
	if (p2_goPosition < p2_currentPosition) {
	    keypressL(player2);
	} else {
	    keypressR(player2);
	}
    }
}
//blink "press "m" to start
bool start_text_timer() 
{
    clock_gettime(CLOCK_REALTIME, &txt_start);
    return true;
}

bool check_text_timer(bool txt) 
{
    if(txt == false) 
	return false;
    clock_gettime(CLOCK_REALTIME, &txt_current);
    int timediff = txt_current.tv_sec - txt_start.tv_sec;
    if( timediff < 1) 
	return true;
    else {
	blinkoff = start_notext_timer(); 
	return false;	
    }
}

bool start_notext_timer() {
    clock_gettime(CLOCK_REALTIME, &txt_start);
    return true;
}
bool check_notext_timer(bool txt) {
    if(txt == false) 
	return false;
    clock_gettime(CLOCK_REALTIME, &txt_current);
    int timediff = txt_current.tv_sec - txt_start.tv_sec;
    if( timediff < 1) 
	return true;
    else {
	blinkon = start_text_timer();
	return false;
    }	
}
//countDown 
void start_countDown_timer() 
{
    clock_gettime(CLOCK_REALTIME, &countdown_start);
    one = true;
    two = true;
    three = true;
    four = true;
}

bool check_countDown_timer() 
{    
    clock_gettime(CLOCK_REALTIME, &countdown_current);
    int timediff = countdown_current.tv_sec - countdown_start.tv_sec;
    if( timediff < 1) {
	if(three) {
	    play_three();
	    three = false;
	}
	countDown3(800,600);
	return false;
    }
    if(timediff < 2 && timediff >= 1){
	if(two) {
	    play_two();
	    two = false;
	}
	countDown2(800,600);
	return false;
    }
    if(timediff < 3 && timediff >=2){
	if(one) {
	    play_one();
	    one = false;
	}
	countDown1(800,600);
	return false;
    }
    if(timediff < 4 && timediff >=3){
	if(four) {
	    play_go();
	    four = false;
	}
	countDown0(800,600);
	return true;
    }
    return true;
}

void countDown3(const int xres, const int yres)
{
    unsigned int yellow = 0x00dddd00;
    Rect i;

    i.bot = yres/2 + 80;
    i.left = xres/2;
    i.center = 0;
    ggprint17(&i, 20, yellow, "   3");
}

void countDown2(const int xres, const int yres)
{
    unsigned int yellow = 0x00dddd00;
    Rect i;

    i.bot = yres/2 + 80;
    i.left = xres/2;
    i.center = 0;
    ggprint17(&i, 20, yellow, "   2");
}

void countDown1(const int xres, const int yres)
{
    unsigned int yellow = 0x00dddd00;
    Rect i;

    i.bot = yres/2 + 80;
    i.left = xres/2;
    i.center = 0;
    ggprint17(&i, 20, yellow, "   1");
}

void countDown0(const int xres, const int yres)
{
    unsigned int yellow = 0x00dddd00;
    Rect i;

    i.bot = yres/2 + 80;
    i.left = xres/2;
    i.center = 0;
    ggprint17(&i, 20, yellow, "START!!");
}
