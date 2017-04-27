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
extern void convertpng2ppm(void);
extern void cleanupPPM(void);
extern void tutorial(const int, const int);
extern void menu(const int, const int);
extern void credits(const int, const int);
extern void startGame(const int, const int);
extern void oneArrow(const int, const int);
extern void twoArrow(const int, const int);
extern void end_credits(int xres, int yres);
extern void gamestart1p(Player *player, int);
extern void gamestart2p(Player *player, Player *player2, int);
extern void keypressA(Player *player);
extern void keypressD(Player *player);
extern void keypressR(Player *player2);
extern void keypressL(Player *player2);
extern int movePlayer(int xres, Player *player);
extern int movePlayer2(int xres, Player *player2);
extern void dropItems(int, int, const int, const int);
extern void deleteSpike(Spike *node);
extern void deleteStar(Star *node);
extern void deleteHeart(Heart *node);
extern void display_health(int, int);
extern void display_score(int, int);
extern void display_collisions(int, int);
extern void display_player_status(int, int);
#ifdef USE_OPENAL_SOUND
extern void initialize_sounds();
extern void play_helmet_hit();
extern void play_powerup();
extern void play_GetShield();
extern void cleanup_sounds();
extern void check_sound();
extern void play_theme();
extern void play_health_pickup();
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
Ppmimage *playerHelmImage = NULL;
Ppmimage *playerInvincibleImage = NULL;
Ppmimage *playerHelmInvincImage = NULL;

Ppmimage *player2Image = NULL;
Ppmimage *player2HelmImage = NULL;
Ppmimage *player2InvincibleImage = NULL;
Ppmimage *player2HelmInvincImage = NULL;

Ppmimage *deathImage = NULL;

Ppmimage *hp4Image = NULL;
Ppmimage *hp3Image = NULL;
Ppmimage *hp2Image = NULL;
Ppmimage *hp1Image = NULL;
Ppmimage *hp0Image = NULL;
Ppmimage *hpiImage = NULL;

Ppmimage *bgImage = NULL;
Ppmimage *bgTransImage = NULL;

Ppmimage *spikeImage = NULL;
Ppmimage *helmetImage = NULL;
Ppmimage *starImage = NULL;
Ppmimage *heartImage = NULL;

GLuint playerTexture;
GLuint playerHelmTexture;
GLuint playerInvincibleTexture;
GLuint playerHelmInvincTexture;

GLuint player2Texture;
GLuint player2HelmTexture;
GLuint player2InvincibleTexture;
GLuint player2HelmInvincTexture;

GLuint deathTexture;

GLuint hp4Texture;
GLuint hp3Texture;
GLuint hp2Texture;
GLuint hp1Texture;
GLuint hp0Texture;
GLuint hpiTexture;

GLuint bgTexture;
GLuint bgTransTexture;

GLuint silhouetteSpike;
GLuint silhouetteHelm;
GLuint silhouetteStar;
GLuint silhouetteHeart;

int display_tutorial = 0;
int display_menu = 0;
int display_credits = 0;
int display_gameModes = 0;
int display_oneArrow = 1;
int display_twoArrow = 0;
int showPlayer = 0;
string p1_name;
string p2_name;
int background = 1;
int silhouette = 1;
int trees = 1;
extern bool p1_helm;
extern bool p1_invincible;
extern bool p2_helm;
extern bool p2_invincible;
bool hardmode = false;
int keys[65536];

int main(void)
{
	cout << "Please enter player1 name: ";
	cin >> p1_name;
	cout << "Please enter player2 name: ";
	cin >> p2_name;
	cout << endl;
#ifdef USE_OPENAL_SOUND
	initialize_sounds();
#endif
	logOpen();
	initXWindows();
	initOpengl();
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
	convertpng2ppm();
	
	// Player1 images
	playerImage = ppm6GetImage("./images/p1.ppm");
	playerHelmImage  = ppm6GetImage("./images/p1Helm.ppm");
	playerInvincibleImage = ppm6GetImage("./images/p1Invinc.ppm");
	playerHelmInvincImage = ppm6GetImage("./images/p1HelmInvinc.ppm");

	// Player2 images
	if (two_player) {
		player2Image = ppm6GetImage("./images/p2.ppm");
		player2HelmImage = ppm6GetImage("./images/p2Helm.ppm");
		player2InvincibleImage = ppm6GetImage("./images/p2Invinc.ppm");
		player2HelmInvincImage = ppm6GetImage("./images/p2HelmInvinc.ppm");
	}

	// Death image
	deathImage = ppm6GetImage("./images/death.ppm");

	// Background images
	bgImage = ppm6GetImage("./images/background1.ppm");
	bgTransImage = ppm6GetImage("./images/transparent.ppm");
	
	// Health bar images
	hp4Image = ppm6GetImage("./images/hp4.ppm");
	hp3Image = ppm6GetImage("./images/hp3.ppm");
	hp2Image = ppm6GetImage("./images/hp2.ppm");
	hp1Image = ppm6GetImage("./images/hp1.ppm");
	hp0Image = ppm6GetImage("./images/hp0.ppm");
	hpiImage = ppm6GetImage("./images/hpi.ppm");
	
	// Item images
	spikeImage = ppm6GetImage("./images/Spike.ppm");
	helmetImage = ppm6GetImage("./images/helmet.ppm");
	starImage = ppm6GetImage("./images/Star.ppm");
	heartImage = ppm6GetImage("./images/heart.ppm");
	
	//create opengl texture elements
	glGenTextures(1, &playerTexture);
	glGenTextures(1, &playerHelmTexture);
	glGenTextures(1, &playerInvincibleTexture);
	glGenTextures(1, &playerHelmInvincTexture);
	if (two_player) {
		glGenTextures(1, &player2Texture);
		glGenTextures(1, &player2HelmTexture);
		glGenTextures(1, &player2InvincibleTexture);
		glGenTextures(1, &player2HelmInvincTexture);
	}
	glGenTextures(1, &deathTexture);
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
	w = playerHelmImage->width;
	h = playerHelmImage->height;
	glBindTexture(GL_TEXTURE_2D, playerHelmTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(playerHelmImage);	
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

	//player with helm and invincibility
	w = playerHelmInvincImage->width;
	h = playerHelmInvincImage->height;
	glBindTexture(GL_TEXTURE_2D, playerHelmInvincTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(playerHelmInvincImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);

	if (two_player) {
		//player2
		w = player2Image->width;
		h = player2Image->height;
		glBindTexture(GL_TEXTURE_2D, player2Texture);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		silhouetteData = buildAlphaData(player2Image);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);

		//player2 with helm
		w = player2HelmImage->width;
		h = player2HelmImage->height;
		glBindTexture(GL_TEXTURE_2D, player2HelmTexture);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		silhouetteData = buildAlphaData(player2HelmImage);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
		
		//player2 with invincibility
		w = player2InvincibleImage->width;
		h = player2InvincibleImage->height;
		glBindTexture(GL_TEXTURE_2D, player2InvincibleTexture);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		silhouetteData = buildAlphaData(player2InvincibleImage);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);

		//player2 with helm and invincibility
		w = player2HelmInvincImage->width;
		h = player2HelmInvincImage->height;
		glBindTexture(GL_TEXTURE_2D, player2HelmInvincTexture);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		silhouetteData = buildAlphaData(player2HelmInvincImage);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	}

	//death
	w = deathImage->width;
	h = deathImage->height;	
	glBindTexture(GL_TEXTURE_2D, deathTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(deathImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);

	// Full HP
	w = hp4Image->width;
	h = hp4Image->height;	
	glBindTexture(GL_TEXTURE_2D, hp4Texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(hp4Image);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);

	// 3/4 HP
	w = hp3Image->width;
	h = hp3Image->height; 
	glBindTexture(GL_TEXTURE_2D, hp3Texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(hp3Image);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);

	// Half HP
	w = hp2Image->width;
	h = hp2Image->height; 
	glBindTexture(GL_TEXTURE_2D, hp2Texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(hp2Image);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);

	// 1/4 HP
	w = hp1Image->width;
	h = hp1Image->height;	
	glBindTexture(GL_TEXTURE_2D, hp1Texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(hp1Image);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);

	// No HP
	w = hp0Image->width;
	h = hp0Image->height; 
	glBindTexture(GL_TEXTURE_2D, hp0Texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(hp0Image);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);

	// Invincible HP
	w = hpiImage->width;
	h = hpiImage->height; 
	glBindTexture(GL_TEXTURE_2D, hpiTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(hpiImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);

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
		keys[key] = 0;	
		if (key == XK_Shift_L || key == XK_Shift_R) {
			return;
		}
	}
	if (e->type == KeyPress) {
		keys[key] = 1;	    
		if (key == XK_Shift_L || key == XK_Shift_R) {
			return;
		}
	} else {
		return;
	}
	switch(key) {
		case XK_h:
			display_tutorial ^= 1;
			break;
		case XK_m:
			display_menu ^= 1;
			break;
		case XK_s:
			display_gameModes ^= 1;
			break;
		case XK_Down:
			display_oneArrow ^= 1;
			display_twoArrow ^= 1;
			break;
		case XK_Up:
			display_oneArrow ^= 1;
			display_twoArrow ^= 1;
			break;
		case XK_p:
			break;
		case XK_Left: 
			break;
		case XK_Right:
			break;
		case XK_a:
			break;
		case XK_d:
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
	int p1_pos;
	int p2_pos = 0;
	if (showPlayer) {
		if (!two_player) {
			p1_pos = movePlayer(xres, &player);
			dropItems(p1_pos, p2_pos, xres, yres);
		} else {
			p1_pos = movePlayer(xres, &player);
			p2_pos = movePlayer2(xres, &player2);
			dropItems(p1_pos, p2_pos, xres, yres);
		}
		if (keys[XK_Left]) {
			keypressL(&player2);
		}
		if (keys[XK_Right]) {
			keypressR(&player2);
		}
		if (keys[XK_a]) {
			keypressA(&player);
		}
		if (keys[XK_d]) {
			keypressD(&player);
		}
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

	// Set background
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

		if (p1_helm && !p1_invincible) {
			// Player1 - Helmet
			glBindTexture(GL_TEXTURE_2D, playerHelmTexture);
		} else if (p1_helm && p1_invincible) {
			// Player1 - Helmet & invincible
			glBindTexture(GL_TEXTURE_2D, playerHelmInvincTexture);
		} else if (!p1_helm && p1_invincible) {
			// Player1 - Invincible
			glBindTexture(GL_TEXTURE_2D, playerInvincibleTexture);
		} else {
			// Player1
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

		if (two_player) {
			glPushMatrix();
			glTranslatef(player2.pos[0], player2.pos[1], 0);
			
			if (p2_helm && !p2_invincible) {
				// Player2 - Helmet
				glBindTexture(GL_TEXTURE_2D, player2HelmTexture);
			} else if (p2_helm && p2_invincible) {
				// Player2 - Helmet & invincible
				glBindTexture(GL_TEXTURE_2D, player2HelmInvincTexture);
			} else if (!p2_helm && p2_invincible) {
				// Player2 - Invincible
				glBindTexture(GL_TEXTURE_2D, player2InvincibleTexture);
			} else {
				// Player2
				glBindTexture(GL_TEXTURE_2D, player2Texture);
			}
		
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
			glEnd();
			glPopMatrix();
		}

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

	if (!showPlayer) {
		r.bot = yres - 20;
		r.left = 10;
		r.center = 0;
		unsigned int color = 0x00dddd00;
		ggprint8b(&r, 16, color, "T - Tutorial");
		ggprint8b(&r, 16, color, "M - Menu");
	}

	if (display_menu && !showPlayer) {
		menu(xres, yres);
	}

	if (display_gameModes && !showPlayer) {
	    	display_menu = 0;	
		startGame(xres, yres);
		if (display_oneArrow && !display_twoArrow) {
		    oneArrow(xres, yres);
		    if (keys[XK_Return]) {
			two_player = false;
			showPlayer ^= 1;
			if (showPlayer) {
				gamestart1p(&player, xres);
			}
		    }
		}
		if (display_twoArrow & !display_oneArrow) {
		    twoArrow(xres, yres);
		    if (keys[XK_Return]) {
		    	two_player = true;
			showPlayer ^= 1 ;
			if (showPlayer) {
				gamestart2p(&player, &player2, xres);
			}
		    }
		}
	}

	if (display_tutorial && !showPlayer) {
	    	display_menu = 0;
		tutorial(xres, yres);
	}

	if (!display_menu && !display_tutorial && display_gameModes && showPlayer) {
		// Display player info to screen
		display_health(xres, yres);
		display_score(xres, yres);
	}
		}
	}
}
