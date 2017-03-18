#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <stdlib.h>
#include "log.h"
#include "ppm.h"
#include "shared.h"

extern Raindrop *ihead;
extern Ppmimage *spikeImage;
extern GLuint spikeTexture;
extern int totrain;

extern void createRaindrop(const int n, const int xres, const int yres)
{
	//create new rain drops...
	int i;
	for (i = 0; i < n; i++) {
		Raindrop *node = (Raindrop *)malloc(sizeof(Raindrop));
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
		++totrain;
	}
}

extern void drawRaindrops(void)
{
	Raindrop *node = ihead;
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