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

//X Windows variables
Display *dpy;
Window win;
Player player;
Player player2;
Spike *sphead = NULL;
Helmet *hhead = NULL;
Star *sthead = NULL;
Heart *hearthead = NULL;

void initXWindows(void);
void initOpengl(void);
void cleanupXWindows(void);
void physics(void);
void render(void);
void checkResize(XEvent *e);
void checkKeys(XEvent *e);
extern void cleanupPPM(void);
extern void tutorial(const int, const int);
extern void menu(const int, const int);
extern void credits(const int, const int);
extern void init(int, int, Player*);
//extern void init2(int, int, Player*);
extern void keypressR(Player *player);
extern void keypressL(Player *player);
//extern void keypressA(Player *player2);
//extern void keypressD(Player *player2);
extern int movePlayer(int xres, Player *player);
extern int movePlayer2(int xres, Player *player2);
extern void dropItems(int, const int, const int);
extern void deleteSpike(Spike *node);
extern void deleteStar(Star *node);
extern void deleteHeart(Heart *node);
extern void display_health(int, int);
extern void display_score(int, int);
extern void display_collisions(int, int);
extern void display_player_status(int, int);
//extern void end_credits(int xres, int yres);
#ifdef USE_OPENAL_SOUND
extern void initialize_sounds();
extern void play_helmet_hit();
extern void play_powerup();
extern void play_GetShield();
extern void cleanup_sounds();
extern void check_sound();
extern void play_theme();
#endif
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
bool two_player = true;

Ppmimage *playerImage = NULL;
Ppmimage *playerhelmImage = NULL;
Ppmimage *playerInvincibleImage = NULL;
Ppmimage *playerImage2 = NULL;
Ppmimage *bgImage = NULL;
Ppmimage *bgTransImage = NULL;
Ppmimage *spikeImage = NULL;
Ppmimage *helmetImage = NULL;
Ppmimage *starImage = NULL;
Ppmimage *heartImage = NULL;

GLuint playerTexture;
GLuint playerhelmTexture;
GLuint playerInvincibleTexture;
GLuint playerTexture2;
GLuint silhouetteSpike;
GLuint silhouetteHelm;
GLuint silhouetteStar;
GLuint silhouetteHeart;
GLuint bgTexture;
GLuint bgTransTexture;

int display_tutorial = 0;
int display_menu = 0;
int display_credits = 0;
int showPlayer = 0;
string player_name;
int background = 1;
int silhouette = 1;
int trees = 1;
extern int spike_collisions;
extern int helm_collisions;
extern int star_collisions;
extern bool helm_status;
extern bool invincible;

int main(void)
{
	cout << "(TEMPORARY TO TEST KYLE'S GAMELOG SCRIPT)" << endl;
	cout << "Please enter player name: ";
	cin >> player_name;
	cout << endl;
#ifdef USE_OPENAL_SOUND
	initialize_sounds();
#endif
	logOpen();
	initXWindows();
	initOpengl();
	init(xres, yres, &player);
//	if (two_player) {
//		init2(xres, yres, &player2);
//	}
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
#ifdef USE_OPENAL_SOUND
	play_theme();
#endif
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
#ifdef USE_OPENAL_SOUND
	cleanup_sounds();
#endif
	cleanupPPM();
	cleanupXWindows();
	cleanup_fonts();
	logClose();
	cout << endl << "To view a list of all game scores, go to:" << endl
		<< "cs.csubak.edu/~koverstreet/3350/dodge/scores.html" << endl;
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

	//OLD RES WAS 1280 X 960
	setupScreenRes(900, 600);
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
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//load the images file into a ppm structure.
	//Character1
	system("convert ./images/standL.png ./images/standL.ppm");
	playerImage      = ppm6GetImage("./images/standL.ppm");
	//Character1 with Helm
	system("convert ./images/standhelmL.png ./images/standhelmL.ppm");
	playerhelmImage   = ppm6GetImage("./images/standhelmL.ppm");
	//Character1 with Invincibility
	system("convert ./images/starplayer.png ./images/starplayer.ppm");
	playerInvincibleImage = ppm6GetImage("./images/starplayer.ppm");

	if (two_player) {
		//Character2 Image
		system("convert ./images/player2.png ./images/player2.ppm");
		playerImage2      = ppm6GetImage("./images/player2.ppm");
	}

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
	//Star Image
	system("convert ./images/Star.png ./images/Star.ppm");
	starImage = ppm6GetImage("./images/Star.ppm");
	//Heart Image
	system("convert ./images/heart.png ./images/heart.ppm");
	heartImage = ppm6GetImage("./images/heart.ppm");
	//create opengl texture elements
	glGenTextures(1, &playerTexture);
	glGenTextures(1, &playerhelmTexture);
	glGenTextures(1, &playerInvincibleTexture);
	if (two_player) {
		glGenTextures(1, &playerTexture2);
	}
	glGenTextures(1, &silhouetteSpike);
	glGenTextures(1, &silhouetteHelm);
	glGenTextures(1, &silhouetteStar);
	glGenTextures(1, &silhouetteHeart);
	glGenTextures(1, &bgTexture);

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
	int w = bgTransImage->width;
	int h = bgTransImage->height;
	unsigned char *ftData = buildAlphaData(bgTransImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, ftData);
	free(ftData);

	//player
	w = playerImage->width;
	h = playerImage->height;
	glBindTexture(GL_TEXTURE_2D, playerTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *silhouetteData = buildAlphaData(playerImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);

	//player with helm
	w = playerhelmImage->width;
	h = playerhelmImage->height;
	glBindTexture(GL_TEXTURE_2D, playerhelmTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(playerhelmImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);

	//player with invincibility
	w = playerInvincibleImage->width;
	h = playerInvincibleImage->height;
	glBindTexture(GL_TEXTURE_2D, playerInvincibleTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(playerInvincibleImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);

	if (two_player) {
		//player2
		w = playerImage2->width;
		h = playerImage2->height;
		glBindTexture(GL_TEXTURE_2D, playerTexture2);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		silhouetteData = buildAlphaData(playerImage2);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	}

	//spike
	w = spikeImage->width;
	h = spikeImage->height;	
	glBindTexture(GL_TEXTURE_2D, silhouetteSpike);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(spikeImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);

	//helmet
	w = helmetImage->width;
	h = helmetImage->height; 
	glBindTexture(GL_TEXTURE_2D, silhouetteHelm);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(helmetImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);

	//star
	w = starImage->width;
	h = starImage->height; 
	glBindTexture(GL_TEXTURE_2D, silhouetteStar);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(starImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);

	//heart
	w = heartImage->width;
	h = heartImage->height; 
	glBindTexture(GL_TEXTURE_2D, silhouetteHeart);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(heartImage); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);

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
				player.pos[1] = 30;
				if (two_player) {
					player2.pos[0] = xres/1.4;
					player2.pos[1] = 30;
				}
			}
			break;
		case XK_t:
			display_tutorial ^= 1;
			break;
		case XK_m:
			display_menu ^= 1;
			break;
		case XK_c:
			display_credits ^= 1;
			break;
		case XK_p:
			//end_credits(xres, yres);
			break;
		case XK_Left:
			keypressL(&player);
			break;
		case XK_Right:
			keypressR(&player);
			break;
		case XK_a:
			//keypressA(&player2);
			break;
		case XK_d:
			//keypressD(&player2);
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

void physics(void)
{
	int player_position;
	//int player2_position;
	if (showPlayer) {
		player_position = movePlayer(xres, &player);
		//player2_position = movePlayer2(xres, &player2);
		dropItems(player_position, xres, yres);
	}
}

void render(void)
{
	Rect r;

	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//draw a quad with texture
	float wid = 30.0f;
	glColor3f(1.0, 1.0, 1.0);

	//set background
	glBindTexture(GL_TEXTURE_2D, bgTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
	glEnd();

	if (showPlayer) {
		glPushMatrix();
		glTranslatef(player.pos[0], player.pos[1], player.pos[2]);

		//set up a timer depending on keypress
		//use an animation span for a certain amount of seconds to display each image
		//combine images into one, and use vertices to display each image individually
		//if stand then keys are not pressed 
		//if left check for left keypress
		//if right check for right keypress
		//at end of render grab time and add to animation span

		if (helm_status == true) {
			// display character with helmet
			glBindTexture(GL_TEXTURE_2D, playerhelmTexture);
		} else if (invincible == true) {
			// display invincible char
			glBindTexture(GL_TEXTURE_2D, playerInvincibleTexture);
		} else {
			// display character without helmet
			glBindTexture(GL_TEXTURE_2D, playerTexture);
		}
		
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);

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

		/*if (two_player) {
			glTranslatef(player2.pos[0], player2.pos[1], player2.pos[2]);
			glBindTexture(GL_TEXTURE_2D, playerTexture2);

			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(255,255,255,255);

			glBegin(GL_QUADS);
			if (player2.LR == false) {
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
		}*/

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
	drawSpikes();
	drawHelmets();
	drawStars();
	drawHeart();

	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	r.bot = yres - 20;
	r.left = 10;
	r.center = 0;
	unsigned int color = 0x00dddd00;
	ggprint8b(&r, 16, color, "B - Start");
	ggprint8b(&r, 16, color, "T - Tutorial");
	ggprint8b(&r, 16, color, "M - Menu");

	if (display_tutorial && !showPlayer) {
		tutorial(xres, yres);
	}

	if (display_menu && !display_tutorial && !showPlayer) {
		menu(xres, yres);
        if (display_credits) {
            credits(xres, yres);
        }
	}

	if (!display_menu && !display_tutorial && showPlayer) {
	// Display player info to screen
		display_health(xres, yres);
		display_score(xres, yres);
		display_collisions(xres, yres);
		display_player_status(xres, yres);
	}
}
