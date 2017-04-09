#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <stdlib.h>
#include "src/log.h"
#include "src/ppm.h"
#include "src/shared.h"
#include <iostream>

extern Item *ihead;
extern Helmet *hhead;
extern Ppmimage *spikeImage;
extern Ppmimage *helmetImage;
extern GLuint spikeTexture;
extern GLuint helmetTexture;

extern void createItems(const int n, const int xres, const int yres)
{
	//create new rain drops...
	int i;
	for (i = 0; i < n; i++) {
		Item *node = (Item *)malloc(sizeof(Item));
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
		node->next = ihead;
		if (ihead != NULL)
			ihead->prev = node;
		ihead = node;
	}
}

extern void drawItems(void)
{
	Item *node = ihead;
	while (node) {
		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		glTranslatef(node->pos[0],node->pos[1],node->pos[2]);
		glBindTexture(GL_TEXTURE_2D, spikeTexture);
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
		glBindTexture(GL_TEXTURE_2D, helmetTexture);
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
