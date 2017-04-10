//DODGE.CPP
//Jacob West
//Kory Despot
//Christian Chavez
//Young Soo
//Kyle Overstreet

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "ppm.h"
#include "shared.h"
#include "youngO.h"
using namespace std;

extern "C" {
#include "fonts.h"
}

const float timeslice = 1.0f;
const float gravity = -0.2f;

//X Windows variables
Display *dpy;
Window win;
Player player;
Spike *sphead = NULL;
Helmet *hhead = NULL;

void initXWindows(void);
void initOpengl(void);
void cleanupXWindows(void);
void physics(void);
void render(void);
void checkResize(XEvent *e);
void checkKeys(XEvent *e);
extern void cleanupPPM(void);
extern void init(int, int, Player*);
extern void keypressR(Player *player);
extern void keypressL(Player *player);
extern int movePlayer(int xres, Player *player);
extern void deleteSpike(Spike *node);
extern void display_score(int, int);
extern void display_collisions(int, int, int, int, bool);
extern void upload_scores();
extern void initialize_sounds();
extern void play_helmet_hit();
extern void play_powerup();
extern void play_GetShield();
extern void cleanup_sounds();
extern void check_sound();
extern void play_theme();
//-----------------------------------------------------------------------------
//Setup timers
const double physicsRate = 1.0 / 30.0;
const double oobillion = 1.0 / 1e9;
struct timespec timeStart, timeCurrent;
struct timespec timePause;
double physicsCountdown=0.0;
double timeSpan=0.0;
unsigned int upause=0;
double timeDiff(struct timespec *start, struct timespec *end) {
	return (double)(end->tv_sec - start->tv_sec ) +
		(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void timeCopy(struct timespec *dest, struct timespec *source) {
	memcpy(dest, source, sizeof(struct timespec));
}
//-----------------------------------------------------------------------------

int done = 0;
int xres = 800, yres = 600;

Ppmimage *playerImage = NULL;
Ppmimage *playerImageMv1 = NULL;
Ppmimage *playerImageMv2 = NULL;
Ppmimage *bgImage = NULL;
Ppmimage *bgTransImage = NULL;
Ppmimage *spikeImage = NULL;
Ppmimage *helmetImage = NULL;

GLuint playerTexture;
GLuint playerMv1Texture;
GLuint playerMv2Texture;
GLuint silhouetteTexture;
GLuint bgTexture;
GLuint bgTransTexture;
GLuint spikeTexture;
GLuint helmetTexture;

int showPlayer = 0;
int background = 1;
int silhouette = 1;
int trees = 1;
int ndrops = 1;
int spike_collisions = 0;
int helm_collisions = 0;
bool helm_status = false;

int main(void)
{
	initialize_sounds();
	logOpen();
	initXWindows();
	initOpengl();
	init(xres, yres, &player);
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	play_theme();
	while (!done) {
		while (XPending(dpy)) {
			XEvent e;
			XNextEvent(dpy, &e);
			checkKeys(&e);
		}
		
		//Below is a process to apply physics at a consistent rate.
		//1. Get the time right now.
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		//2. How long since we were here last?
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		//3. Save the current time as our new starting time.
		timeCopy(&timeStart, &timeCurrent);
		//4. Add time-span to our countdown amount.
		physicsCountdown += timeSpan;
		//5. Has countdown gone beyond our physics rate? 
		//       if yes,
		//           In a loop...
		//              Apply physics
		//              Reducing countdown by physics-rate.
		//              Break when countdown < physics-rate.
		//       if no,
		//           Apply no physics this frame.
		while (physicsCountdown >= physicsRate) {
			//6. Apply physics
			physics();
			//7. Reduce the countdown by our physics-rate
			physicsCountdown -= physicsRate;
		}
		//check_sound();
		//Always render every frame.
		render();
		glXSwapBuffers(dpy, win);
	}
	cleanup_sounds();
	cleanupPPM();
	cleanupXWindows();
	cleanup_fonts();
	logClose();
	return 0;
}

void cleanupXWindows(void) {
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

void setTitle(void)
{
	//Set the window title bar.
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "Dodge");
}

void setupScreenRes(const int w, const int h)
{
	xres = w;
	yres = h;
}

void initXWindows(void)
{
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	XSetWindowAttributes swa;

	setupScreenRes(1280, 960);
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		printf("\n\tcannot connect to X server\n\n");
		exit(EXIT_FAILURE);
	}
	Window root = DefaultRootWindow(dpy);
	XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	if (vi == NULL) {
		printf("\n\tno appropriate visual found\n\n");
		exit(EXIT_FAILURE);
	} 
	Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
		StructureNotifyMask | SubstructureNotifyMask;
	win = XCreateWindow(dpy, root, 0, 0, xres, yres, 0,
			vi->depth, InputOutput, vi->visual,
			CWColormap | CWEventMask, &swa);
	GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
	setTitle();
}

unsigned char *buildAlphaData(Ppmimage *img)
{
	//add 4th component to RGB stream...
	int i;
	int a,b,c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = (a|b|c);
		ptr += 4;
		data += 3;
	}
	return newdata;
}

void initOpengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, xres, yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, xres, 0, yres, -1, 1);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);

	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//load the images file into a ppm structure.
	//Character Image Left
	system("convert ./images/standL.png ./images/standL.ppm");
	playerImage      = ppm6GetImage("./images/standL.ppm");
	//Character Image Move1
	system("convert ./images/walking1.png ./images/walking1.ppm");
	playerImageMv1   = ppm6GetImage("./images/walking1.ppm");
	//Character Image Move2
	system("convert ./images/walking2.png ./images/walking2.ppm");
	playerImageMv2   = ppm6GetImage("./images/walking2.ppm");
	//Background Image
	system("convert ./images/background1.jpg ./images/background1.ppm");
	bgImage      = ppm6GetImage("./images/background1.ppm");
	//Transparent Image (since it messes up if I delete it)	
	bgTransImage = ppm6GetImage("./images/transparent.ppm");
	//Spike Image
	system("convert ./images/Spike.png ./images/Spike.ppm");
	spikeImage = ppm6GetImage("./images/Spike.ppm");
	//Helmet Image
	system("convert ./images/helmet.png ./images/helmet.ppm");
	helmetImage = ppm6GetImage("./images/helmet.ppm");

	//create opengl texture elements
	glGenTextures(1, &playerTexture);
	glGenTextures(1, &playerMv1Texture);
	glGenTextures(1, &playerMv2Texture);
	glGenTextures(1, &silhouetteTexture);
	glGenTextures(1, &bgTexture);
	glGenTextures(1, &spikeTexture);
	glGenTextures(1, &helmetTexture);

	//-------------------------------------------------------------------------
	//player
	int w = playerImage->width;
	int h = playerImage->height;	
	glBindTexture(GL_TEXTURE_2D, playerTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, playerImage->data);
	//-------------------------------------------------------------------------
	//playerMv1
	w = playerImageMv1->width;
	h = playerImageMv1->height;	
	glBindTexture(GL_TEXTURE_2D, playerMv1Texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, playerImageMv1->data);
	//-------------------------------------------------------------------------
	//playerMv2
	w = playerImageMv2->width;
	h = playerImageMv2->height;	
	glBindTexture(GL_TEXTURE_2D, playerMv2Texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, playerImageMv2->data);
	//-------------------------------------------------------------------------
	//silhouette
	//this is similar to a sprite graphic
	glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *silhouetteData = buildAlphaData(playerImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//-------------------------------------------------------------------------
	//background
	glBindTexture(GL_TEXTURE_2D, bgTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
			bgImage->width, bgImage->height,
			0, GL_RGB, GL_UNSIGNED_BYTE, bgImage->data);
	//-------------------------------------------------------------------------
	//background transparent part
	glBindTexture(GL_TEXTURE_2D, bgTransTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//must build a new set of data...
	w = bgTransImage->width;
	h = bgTransImage->height;
	unsigned char *ftData = buildAlphaData(bgTransImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, ftData);
	free(ftData);
	//-------------------------------------------------------------------------
	//spike
	w = spikeImage->width;
	h = spikeImage->height;	
	glBindTexture(GL_TEXTURE_2D, spikeTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, spikeImage->data);
	//helmet
	w = helmetImage->width;
	h = helmetImage->height; 
	glBindTexture(GL_TEXTURE_2D, helmetTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, helmetImage->data);
}

void checkKeys(XEvent *e)
{
	int key = XLookupKeysym(&e->xkey, 0);
	
	if (e->type == KeyRelease) {
		if (key == XK_Shift_L || key == XK_Shift_R) {
			return;
		}
	}
	if (e->type == KeyPress) {
		if (key == XK_Shift_L || key == XK_Shift_R) {
			return;
		}
	} else {
		return;
	}
	switch(key) {
		case XK_b:
			showPlayer ^= 1;
			if (showPlayer) {
				player.pos[0] = xres/2;
				player.pos[1] = yres-920;
			}
			break;
		case XK_p:
			play_powerup();
			break;
		case XK_Left:
			keypressL(&player);
			break;
		case XK_Right:
			keypressR(&player);
			break;
		case XK_Escape:
			done=1;
			break;
	}
}

Flt VecNormalize(Vec vec)
{
	Flt len, tlen;
	Flt xlen = vec[0];
	Flt ylen = vec[1];
	Flt zlen = vec[2];
	len = xlen*xlen + ylen*ylen + zlen*zlen;
	if (len == 0.0) {
		MakeVector(0.0,0.0,1.0,vec);
		return 1.0;
	}
	len = sqrt(len);
	tlen = 1.0 / len;
	vec[0] = xlen * tlen;
	vec[1] = ylen * tlen;
	vec[2] = zlen * tlen;
	return(len);
}

void dropItems(int player_pos)
{
	//create items
	if (random(100) < 15) {
		createSpikes(ndrops, xres, yres);
	}
	if (random(200) < 2) {
		createHelmets(ndrops, xres, yres);
	}

	//move items
	Spike *spike = sphead;
	while (spike) {
		//force is toward the ground
		spike->vel[1] += gravity;
		VecCopy(spike->pos, spike->lastpos);
		{
			spike->pos[0] += spike->vel[0] * timeslice;
			spike->pos[1] += spike->vel[1] * timeslice;
			if (fabs(spike->vel[1]) > spike->maxvel[1])
				spike->vel[1] *= 0.96;
			spike->vel[0] *= 0.999;
		}
		spike = spike->next;
	}

	Helmet *helmet = hhead;
	while (helmet) {
		//force is toward the ground
		helmet->vel[1] += gravity;
		VecCopy(helmet->pos, helmet->lastpos);
		{
			helmet->pos[0] += helmet->vel[0] * timeslice;
			helmet->pos[1] += helmet->vel[1] * timeslice;
			if (fabs(helmet->vel[1]) > helmet->maxvel[1])
				helmet->vel[1] *= 0.96;
			helmet->vel[0] *= 0.999;
		}
		helmet = helmet->next;
	}

	//check item positions
	spike = sphead;
	while (spike) {
		if (((spike->pos[1] > 0 && spike->pos[1] < 80)) &&
			((spike->pos[0] > player_pos-40) &&
			(spike->pos[0] < player_pos+40))) {
			//spike has hit player
			spike_collisions++;
			deleteSpike(spike);
			helm_status = false;
		}
		if (spike->pos[1] < -20.0f) {
			//spike has hit ground
			Spike *savespike = spike->next;
			deleteSpike(spike);
			spike = savespike;
			continue;
		}
		spike = spike->next;
	}

	helmet = hhead;
	while (helmet) {
		if (((helmet->pos[1] > 0 && helmet->pos[1] < 80)) &&
			((helmet->pos[0] > player_pos-40) &&
			(helmet->pos[0] < player_pos+40))) {
			//helmet has hit player
			play_helmet_hit();
			helm_collisions++;
			helm_status = true;
			deleteHelmet(helmet);
		}
		if (helmet->pos[1] < -20.0f) {
			//helmet has hit ground
			Helmet *savehelm = helmet->next;
			deleteHelmet(helmet);
			helmet = savehelm;
			continue;
		}
		helmet = helmet->next;
	}
}

void physics(void)
{
	int player_position;
	if (showPlayer) {
		player_position = movePlayer(xres, &player);
		dropItems(player_position);
	}
}

void render(void)
{
	Rect r;

	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//draw a quad with texture
	float wid = 40.0f;
	glColor3f(1.0, 1.0, 1.0);
	if (background) {
		glBindTexture(GL_TEXTURE_2D, bgTexture);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
		glEnd();
	}
	if (showPlayer) {
		glPushMatrix();
		glTranslatef(player.pos[0], player.pos[1], player.pos[2]);
		if (!silhouette) {
			glBindTexture(GL_TEXTURE_2D, playerMv1Texture);
		}
		//set up a timer depending on keypress
		//use an animation span for a certain amount of seconds to display each image
		//combine images into one, and use vertices to display each image individually
		//if stand then keys are not pressed 
		//if left check for left keypress
		//if right check for right keypress
		//at end of render grab time and add to animation span

		else {
			glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(255,255,255,255);
		} 
		glBegin(GL_QUADS);
		if (player.LR == false) {
			glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
			glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
			glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
		} else {
			glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
			glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
			glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
		}
		glEnd();
		glPopMatrix();

		if (trees && silhouette) {
			glBindTexture(GL_TEXTURE_2D, bgTransTexture);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
			glEnd();
		}
		glDisable(GL_ALPHA_TEST);
	}
	drawHelmets();
	drawSpikes();

	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	r.bot = yres - 20;
	r.left = 10;
	r.center = 0;
	unsigned int color = 0x00dddd00;
	ggprint8b(&r, 16, color, "B - Start");
	//ggprint8b(&r, 16, color, "N - Sounds");

	// Display score to top right of screen
	display_score(xres, yres);

	// Display collision count (for testing)
	display_collisions(xres, yres, spike_collisions, helm_collisions, helm_status);
}
