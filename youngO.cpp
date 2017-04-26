// Young Soo Oh
// CMPS 3350
// Dodge Project

#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <stdlib.h>
#include "src/log.h"
#include "src/ppm.h"
#include "src/shared.h"
#include <iostream>

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

extern void createSpikes(const int n, const int xres, const int yres)
{
	//create new rain drops...
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
		//put raindrop into linked list
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

extern void createHelmets(const int n, const int xres, const int yres)
{
	//create new rain drops...
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
		//put raindrop into linked list
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

extern void createStars(const int n, const int xres, const int yres)
{
	//create new rain drops...
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
		//put raindrop into linked list
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
extern void createHeart(const int n, const int xres, const int yres)
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
