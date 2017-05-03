/******** DODGE ********

  Created by:
  Kyle Overstreet
  Jacob West
  Young Soo Oh
  Kory Despot
  Christian Chavez

  Framework by:
  Gordon Griesel

 ************************/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "ppm.h"
#include "shared.h"
#include "youngO.h"
#include "christianC.h"
using namespace std;

extern "C" {
#include "fonts.h"
}

// X Windows variables
Display *dpy;
Window win;

void initXWindows(void);
void initOpengl(void);
void cleanupXWindows(void);
void physics(void);
void render(void);
void checkResize(XEvent *e);
void checkKeys(XEvent *e);
extern void convertpng2ppm(void);
extern void getImage(void);
extern void cleanupPPM(void);
extern void generateTextures(void);

extern bool start_text_timer();
extern bool start_notext_timer();
extern bool check_text_timer(bool);
extern bool check_notext_timer(bool);
extern void start_countDown_timer();
extern bool check_countDown_timer();

extern void start_menu(const int, const int);
extern void mode_menu(const int, const int);
extern void audio_menu(const int, const int);
extern void tutorial(const int, const int);
extern void end_menu(const int, const int);
extern void onePlayerStart(const int, int, char[], Player* player, Input &input);
extern void twoPlayerStart(const int, int, char[], char[], Player* player, 
                            Player* player2, Input &input);
extern void player1Name(const int, const int, Input &input);
extern void getName_player1(int, Input &input);
extern void player2Name(const int, const int, Input &input);
extern void getName_player2(int, Input &input);
extern void credits(const int, const int);
extern void end_credits(int xres, int yres);
extern void gamestart1p(Player *player, int);
extern void gamestart2p(Player *player, Player *player2, int);
extern void keypressA(Player *player);
extern void keypressD(Player *player);
extern void keypressR(Player *player2);
extern void keypressL(Player *player2);
extern int movePlayer(int xres, Player *player);
extern void dropItems(int, int, const int, const int);
extern void deleteSpike(Spike *node);
extern void deleteStar(Star *node);
extern void deleteHeart(Heart *node);
extern void display_health(int, int);
extern void display_score(int, int);
extern void tombstone(int);
extern void logo(int, int);
extern void introMessage(int, int);
extern void mainmenu(int, int);
extern void gameover(int, int);
extern void gameOver(const int, const int);
extern void view_scores();
#ifdef USE_OPENAL_SOUND
extern void play_health_loss();
extern void initialize_sounds();
extern void play_helmet_hit();
extern void play_powerup();
extern void play_GetShield();
extern void cleanup_sounds();
extern void check_sound();
extern void play_theme();
extern void play_health_pickup();
extern void assign_namep1(char[], Input &input);
extern void assign_namep2(char[], Input &input);
extern void gameOverScores(const int, const int, char[], char[]);
extern void play_menu_select();
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

bool creds = 0;
int done = 0;
int xres = 800, yres = 600;
bool two_player = true;

Ppmimage *bgImage = NULL;
Ppmimage *bgTransImage = NULL;
Ppmimage *logoImage = NULL;
Ppmimage *mainmenuImage = NULL;
Ppmimage *playerImage = NULL;
Ppmimage *playerHelmImage = NULL;
Ppmimage *playerInvincibleImage = NULL;
Ppmimage *playerHelmInvincImage = NULL;
Ppmimage *player2Image = NULL;
Ppmimage *player2HelmImage = NULL;
Ppmimage *player2InvincibleImage = NULL;
Ppmimage *player2HelmInvincImage = NULL;
Ppmimage *spikeImage = NULL;
Ppmimage *helmetImage = NULL;
Ppmimage *starImage = NULL;
Ppmimage *heartImage = NULL;
Ppmimage *hp4Image = NULL;
Ppmimage *hp3Image = NULL;
Ppmimage *hp2Image = NULL;
Ppmimage *hp1Image = NULL;
Ppmimage *hp0Image = NULL;
Ppmimage *hpiImage = NULL;
Ppmimage *deathImage = NULL;
Ppmimage *gameoverImage = NULL;

GLuint bgTexture;
GLuint bgTransTexture;
GLuint logoTexture;
GLuint mainmenuTexture;
GLuint playerTexture;
GLuint playerHelmTexture;
GLuint playerInvincibleTexture;
GLuint playerHelmInvincTexture;
GLuint player2Texture;
GLuint player2HelmTexture;
GLuint player2InvincibleTexture;
GLuint player2HelmInvincTexture;
GLuint silhouetteSpike;
GLuint silhouetteHelm;
GLuint silhouetteStar;
GLuint silhouetteHeart;
GLuint hp4Texture;
GLuint hp3Texture;
GLuint hp2Texture;
GLuint hp1Texture;
GLuint hp0Texture;
GLuint hpiTexture;
GLuint deathTexture;
GLuint gameoverTexture;

Player player;
Player player2;
Spike *sphead = NULL;
Helmet *hhead = NULL;
Star *sthead = NULL;
Heart *hearthead = NULL;
Input input;

/*int display_tutorial = 0;
int display_menu = 1;
int display_credits = 0;
int display_gameModes = 0;
int display_audioSettings = 0;
int display_oneArrow = 1;
int display_twoArrow = 0;*/

bool intro = true;
bool blinkon = true;
bool blinkoff = false;
bool countdown_started;
bool countdown_done;
bool show_logo = true;
bool intro_message = true;
int menu_position = 1;
bool display_startmenu = false;
bool display_modemenu = false;
bool display_audiomenu = false;
bool display_tutorial = false;
bool display_endmenu = false;
bool display_playername = false;
bool display_playername2 = false;
bool display_gameoverscores = false;
bool audio_on = true;
bool start_game = false;
bool game_over = false;
char p1_name[100];
//string p1_name;
bool reset_game = false;
//string p1_name;
extern bool p1_helm;
extern bool p1_invincible;
extern bool p1_dead;
extern int p1_deadpos;
char p2_name[100];
//string p2_name;
extern bool p2_helm;
extern bool p2_invincible;
extern bool p2_dead;
extern int p2_deadpos;
extern bool entering_one;
extern bool entering_two;
extern int menu_count;

int keys[65536];

int main(void)
{
	// Temporary console prompt until implemented in game window
    /*
	cout << "Please enter player1 name: ";
	cin >> p1_name;
	cout << "Please enter player2 name: ";
	cin >> p2_name;
	cout << endl;
    */
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

    blinkon = start_text_timer();
	// Display both players for intro animation
	gamestart2p(&player, &player2, xres);
	while (!done) {
		while (XPending(dpy)) {
			XEvent e;
			XNextEvent(dpy, &e);
			checkKeys(&e);
		}

		// Below is a process to apply physics at a consistent rate.
		// 1. Get the time right now.
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		// 2. How long since we were here last?
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		// 3. Save the current time as our new starting time.
		timeCopy(&timeStart, &timeCurrent);
		// 4. Add time-span to our countdown amount.
		physicsCountdown += timeSpan;
		// 5. Has countdown gone beyond our physics rate? 
		//       if yes,
		//           In a loop...
		//              Apply physics
		//              Reducing countdown by physics-rate.
		//              Break when countdown < physics-rate.
		//       if no,
		//           Apply no physics this frame.
		while (physicsCountdown >= physicsRate) {
			// 6. Apply physics
			physics();
			// 7. Reduce the countdown by our physics-rate
			physicsCountdown -= physicsRate;
		}
		// Always render every frame.
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
	//view_scores();
	return 0;
}

void cleanupXWindows(void) {
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

void setTitle(void)
{
	// Set the window title bar
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
	// Add 4th component to RGB stream...
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
	// OpenGL initialization
	glViewport(0, 0, xres, yres);
	// Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	// This sets 2D mode (no perspective)
	glOrtho(0, xres, 0, yres, -1, 1);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);

	// Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	// Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();

	// Load the images file into a ppm structure.
	convertpng2ppm();

	// Load images into each individual Ppmimage structure
	getImage();

	// Create opengl texture elements
	generateTextures();

	// Background
	glBindTexture(GL_TEXTURE_2D, bgTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
			bgImage->width, bgImage->height,
			0, GL_RGB, GL_UNSIGNED_BYTE, bgImage->data);

	// Background (transparent)
	glBindTexture(GL_TEXTURE_2D, bgTransTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	int w = bgTransImage->width;
	int h = bgTransImage->height;
	unsigned char *ftData = buildAlphaData(bgTransImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, ftData);
	free(ftData);

	// Dodge logo
	w = logoImage->width;
	h = logoImage->height;	
	glBindTexture(GL_TEXTURE_2D, logoTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *silhouetteData = buildAlphaData(logoImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);

	// Main menu
	w = mainmenuImage->width;
	h = mainmenuImage->height;	
	glBindTexture(GL_TEXTURE_2D, mainmenuTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(mainmenuImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	// Player1
	w = playerImage->width;
	h = playerImage->height;
	glBindTexture(GL_TEXTURE_2D, playerTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(playerImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);

	// Player1 with helmet
	w = playerHelmImage->width;
	h = playerHelmImage->height;
	glBindTexture(GL_TEXTURE_2D, playerHelmTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(playerHelmImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);

	// Player1 with invincibility
	w = playerInvincibleImage->width;
	h = playerInvincibleImage->height;
	glBindTexture(GL_TEXTURE_2D, playerInvincibleTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(playerInvincibleImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);

	// Player1 with helmet and invincibility
	w = playerHelmInvincImage->width;
	h = playerHelmInvincImage->height;
	glBindTexture(GL_TEXTURE_2D, playerHelmInvincTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(playerHelmInvincImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);

	// Player2
	w = player2Image->width;
	h = player2Image->height;
	glBindTexture(GL_TEXTURE_2D, player2Texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(player2Image);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);

	// Player2 with helmet
	w = player2HelmImage->width;
	h = player2HelmImage->height;
	glBindTexture(GL_TEXTURE_2D, player2HelmTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(player2HelmImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);

	// Player2 with invincibility
	w = player2InvincibleImage->width;
	h = player2InvincibleImage->height;
	glBindTexture(GL_TEXTURE_2D, player2InvincibleTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(player2InvincibleImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);

	// Player2 with helmet and invincibility
	w = player2HelmInvincImage->width;
	h = player2HelmInvincImage->height;
	glBindTexture(GL_TEXTURE_2D, player2HelmInvincTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(player2HelmInvincImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);

	// Spike
	w = spikeImage->width;
	h = spikeImage->height;	
	glBindTexture(GL_TEXTURE_2D, silhouetteSpike);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(spikeImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);

	// Helmet
	w = helmetImage->width;
	h = helmetImage->height; 
	glBindTexture(GL_TEXTURE_2D, silhouetteHelm);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(helmetImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);

	// Star
	w = starImage->width;
	h = starImage->height; 
	glBindTexture(GL_TEXTURE_2D, silhouetteStar);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(starImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);

	// Heart
	w = heartImage->width;
	h = heartImage->height; 
	glBindTexture(GL_TEXTURE_2D, silhouetteHeart);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(heartImage); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);

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

	// Tombstone
	w = deathImage->width;
	h = deathImage->height;	
	glBindTexture(GL_TEXTURE_2D, deathTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(deathImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);

	// Game Over
	w = gameoverImage->width;
	h = gameoverImage->height;	
	glBindTexture(GL_TEXTURE_2D, gameoverTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	silhouetteData = buildAlphaData(gameoverImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
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
    if (display_playername2) {
        twoPlayerStart(xres, key, p1_name, p2_name, &player, &player2, input);
    }
    if (display_playername) {
        onePlayerStart(xres, key, p1_name, &player, input);
    }
	switch(key) {
        case XK_m:
			// Start menu
			if (!start_game && !display_modemenu
                && !display_audiomenu && !display_tutorial
                && !display_endmenu && !display_playername 
                && !display_playername2) {
				display_startmenu = true;
                intro_message = false;
                blinkon = false;
                blinkoff =false;
			}
			
			break;
        case XK_c:
            if (game_over && display_gameoverscores) {
                display_gameoverscores = false;
                display_endmenu = true;
            }
            break;
		case XK_Down:
			// Keep menu position accurate
			if (display_startmenu && menu_position != 3) {
				menu_position++;
				play_menu_select();
			} else if ((display_modemenu || display_audiomenu) && menu_position != 2) {
				menu_position++;
				play_menu_select();
			} else if (display_endmenu && menu_position != 4) {
				menu_position++;
				play_menu_select();
			}
			break;
		case XK_Up:
			// Keep menu position accurate
			if ((display_startmenu || display_modemenu || display_audiomenu || display_endmenu)
				&& menu_position != 1) {
				menu_position--;
			}
			break;
		case XK_Return:
			// Game mode menu
			if (display_modemenu) {
				// 1-Player
				if (menu_position == 1) {
					display_modemenu = false;
                    display_playername = true;
                    two_player = false;
				// 2-Player
				} else {
					display_modemenu = false;
                    display_playername = true;
                    two_player = true;
				}
			}
			// Audio settings
			if (display_audiomenu) {
				// Enable audio
				if (menu_position == 1) {
					audio_on = true;
				// Disable audio
				} else {
					audio_on = false;
				}
			}
			// Game over menu
			if (display_endmenu) {
				// Play again
				if (menu_position == 1) {
					display_endmenu = false;
					start_game = true;
					if (!two_player) {
						gamestart1p(&player, xres);
                        start_countDown_timer();
                        countdown_started = true;
                        countdown_done = false;
					} else {
						gamestart2p(&player, &player2, xres);
                        start_countDown_timer();
                        countdown_started = true;
                        countdown_done = false;
					}
				// Change game mode
				} else if (menu_position == 2) {
					display_endmenu = false;
					display_modemenu = true;
					menu_position = 1;
				// Audio settings
				} else if (menu_position == 3) {
					display_endmenu = false;
					display_audiomenu = true;
					menu_position = 1;
				// View scores
				} else if (menu_position == 4) {
					view_scores();
				}
			}
			break;
		case XK_Escape:
			start_game = false;
			if(creds) {
				done=1;
				break;
			}
			creds = 1;
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
	// Introduction animation
	if (intro) {
		moveRandomly(&player, &player2);
	}

	// Game started - activate player movement controls and item drops
	if (start_game) {
		intro = false;
		int p1_pos;
		int p2_pos = 0;
		if (!two_player) {
			p1_pos = movePlayer(xres, &player);
			dropItems(p1_pos, p2_pos, xres, yres);
		} else {
			p1_pos = movePlayer(xres, &player);
			p2_pos = movePlayer(xres, &player2);
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

	// Delete all items after game is over
	if (reset_game) {
		while (sphead) {
			deleteSpike(sphead);
		}
		while (hhead) {
			deleteHelmet(hhead);
		}
		while (sthead) {
			deleteStar(sthead);
		}
		while (hearthead) {
			deleteHeart(hearthead);
		}
		reset_game = false;
	}
}

void render(void)
{
	// Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

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

	// Display Player1
	if ((start_game && !p1_dead) || intro) {
		glPushMatrix();
		glTranslatef(player.pos[0], player.pos[1], player.pos[2]);

		// Check Player1 status to display appropriate texture
		if (p1_helm && !p1_invincible) {
			glBindTexture(GL_TEXTURE_2D, playerHelmTexture);
		} else if (p1_helm && p1_invincible) {
			glBindTexture(GL_TEXTURE_2D, playerHelmInvincTexture);
		} else if (!p1_helm && p1_invincible) {
			glBindTexture(GL_TEXTURE_2D, playerInvincibleTexture);
		} else {
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
	}

	// Display Player2 (intro animation and 2P mode only)
	if (intro || (two_player && !p2_dead)) {
		glPushMatrix();
		glTranslatef(player2.pos[0], player2.pos[1], 0);

		// Check Player2 status to display appropriate texture
		if (p2_helm && !p2_invincible) {
			glBindTexture(GL_TEXTURE_2D, player2HelmTexture);
		} else if (p2_helm && p2_invincible) {
			glBindTexture(GL_TEXTURE_2D, player2HelmInvincTexture);
		} else if (!p2_helm && p2_invincible) {
			glBindTexture(GL_TEXTURE_2D, player2InvincibleTexture);
		} else {
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

	glBindTexture(GL_TEXTURE_2D, bgTransTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
	glEnd();
	glDisable(GL_ALPHA_TEST);

	drawSpikes();
	drawHelmets();
	drawStars();
	drawHeart();

	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

    if (countdown_started) {
        countdown_done = check_countDown_timer();
    }

	// Menus (new)
	if (show_logo && !start_game && !game_over) {
		logo(xres, 500);
	}
	
    blinkon = check_text_timer(blinkon);
    blinkoff = check_notext_timer(blinkoff);
    if (blinkon){
         introMessage(xres, yres);
    }

	if (display_startmenu) {
		start_menu(xres, yres);
	}
	if (display_modemenu) {
		mode_menu(xres, yres);
	}
    if (display_playername) {
        player1Name(xres, yres, input);
    }
    if (display_playername2) {
        player2Name(xres, yres, input);
    }
	if (display_audiomenu) {
		audio_menu(xres, yres);
	}
	if (display_tutorial) {
		tutorial(xres, yres);
	}
	if (display_gameoverscores) {
        gameOverScores(xres, yres, p1_name, p2_name);
	}
	if (display_endmenu) {
        logo(xres, 500);
		end_menu(xres, yres);
	}

	// Display health and score to screen once game is started
	if (start_game) {
		//display_gameModes = 0;
		//display_menu = 0;
		display_health(xres, yres);
		display_score(xres, yres);
	}

	// Display tombstone at position of death(s)
	if (p1_dead) {
		tombstone(p1_deadpos);
	}
	if (p2_dead) {
		tombstone(p2_deadpos);
	}

	// Display "Game Over" after player(s) death 
	if (game_over && display_gameoverscores) {
		gameover(xres, 490);
	}

	// Display credits
	if(creds) {
		end_credits(xres, yres);
	}
}
