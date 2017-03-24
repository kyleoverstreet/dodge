//Created by: Christian Chavez
//Group project
//Last Update: 3/2/2017

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "src/fonts.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
//X Windows variables
GLXContext glc;
//Structures
struct Vec {
	float x, y, z;
};

struct Shape{
    float width, height;
    Vec center;
};

struct Menu {
	Shape menu;
};

//Function prototypes
extern void renderMenu(Menu *myMenu);
extern void buildMenu();

extern void buildMenu(Display *dpy, Window win)
{
	int done=0;
	srand(time(NULL));
	//declare game object
	Menu myMenu;

	//declare a menu
	myMenu.menu.width = 200;
	myMenu.menu.height = 150;
	myMenu.menu.center.x = 400;
	myMenu.menu.center.y = 300;

	//start animation
	while (!done) {
		while (XPending(dpy)) {
			XEvent e;
			XNextEvent(dpy, &e);
		}
		renderMenu(&myMenu);
		glXSwapBuffers(dpy, win);
	}
	return;
}

extern void renderMenu(Menu *myMenu)
{
	float w, h;
	glClear(GL_COLOR_BUFFER_BIT);
	//Draw shapes...

	//draw box
	Shape *s;
	glColor3ub(49, 86, 145);
	s = &myMenu->menu;
	glPushMatrix();
	glTranslatef(s->center.x, s->center.y, s->center.z);
	w = s->width;
	h = s->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
		glEnd();
		glPopMatrix();
	Rect r;
	r.bot = s->center.y + 80;
	r.left = s->center.x;
	r.center = s->center.y;
	unsigned int color = 0xc13c47;
	//ggprint13(&r, 10, color, "Welcome to the DODGE game menu!");
	glEnd();

}



