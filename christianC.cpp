//Created by: Christian Chavez
//Dodge project
//Individual coode
//Last Update: 4/27/2017

/*
                    *********Week 13 update*********
for previous weeks I tried to implement a menu, but that was having issues
when trying to implement it to the game. So, the previous method was scratched
and a new one was started in order to get it wrking with the game. The Menu is
designed to apper in the window when the game is started up and give the user
a place for navigation, whether thats how to play the game, look at the high
scores or pick what game mode they would like to do. On the 23rd I added the
ability to bring up a menu and added limited functionality to navigate throuh
the menu. On the 24th I solved an issue that orevented the menu bix popping up
when more text was added inside the box. April 27th: Added the fucntionallity to this file
and the main file to be able to select the game mode from the menu screen; I
added the primitive menu for the audio settings and made the game over message
apper on screen when the game is over.
*/

#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <time.h>
#include <vector>
#include "src/ppm.h"
using namespace std;

extern "C" {
#include "src/fonts.h"
}

//Structures for a box
struct Vec {
    float x, y, z;
};

struct Shape {
    float width, height;
    Vec center;
};

void menu(const int, const int);
void startGame(const int, const int);
void oneArrow(const int, const int);
void twoArrow(const int, const int);
void onArrow(const int, const int);
void offArrow(const int, const int);
void audioSettings (const int, const int);
void gameOver (const int, const int);
extern void tutorial(const int xres, const int yres);

//Colors for the test in themenus and the sub menus
//unsigned int red = 0xff0000;
//unsigned int green = 0x00ff00;
unsigned int yellow = 0xffff00;
//unsigned int blue = 0x0000ff;
//unsigned int aqua = 0x00ffff;
unsigned int white = 0xffffff;

void menu(const int xres, const int yres)
{
    int y = yres/2;
    int x = xres/2;


    //Welcome message Text
    Rect menu;
/*    menu.bot = y + 225;
    menu.left = x;
    menu.center = y;
    ggprint8b(&menu, 16, white, "DODGE MAIN MENU");
*/    
    //Change/view the audio settings
    menu.bot = y + 175;
    menu.left = x;
    menu.center = y;
    ggprint8b(&menu, 16, yellow, "<t> Toggle Audio");

    //View how tro play the game
    menu.bot = y + 158;
    menu.left = x;
    menu.center = y;
    ggprint8b(&menu, 16, yellow, "<h> How to Play");

    //Chnage view the game mode
    menu.bot = y + 141;
    menu.left = x;
    menu.center = y;
    ggprint8b(&menu, 16, yellow, "<s> Start Game");

}

void startGame (const int xres, const int yres)
{
    int y = yres/2;
    int x = xres/2;

    Rect mode;
    mode.bot = y + 225;
    mode.left = x;
    mode.center = y;
    ggprint8b(&mode, 16, white, "Select Game Mode");

    //For the one player mode
    mode.bot = y + 175;
    mode.left = x;
    mode.center = y;
    ggprint8b(&mode, 16, yellow, "1 Player");

    //For the two player mode
    mode.bot = y + 158;
    mode.left = x;
    mode.center = y;
    ggprint8b(&mode, 16, yellow, "2 Players");

}

void oneArrow (const int xres, const int yres)
{
    int y = yres/2;
    int x = xres/2;

    Rect one_arrow;
    one_arrow.bot = y + 175;
    one_arrow.left = x - 35;
    one_arrow.center = y;
    ggprint8b(&one_arrow, 16, yellow, ">>");
    
}

void twoArrow (const int xres, const int yres)
{
    int y = yres/2;
    int x = xres/2;

    Rect two_arrow;
    two_arrow.bot = y + 158;
    two_arrow.left = x - 40;
    two_arrow.center = y;
    ggprint8b(&two_arrow, 16, yellow, ">>");
    
}

void audioSettings (const int xres, const int yres)
{
    int y = yres/2;
    int x = xres/2;

    Rect audio;
    audio.bot = y + 225;
    audio.left = x;
    audio.center = y;
    ggprint8b(&audio, 16, white, "Select Audio Mode");

    //For on mode
    audio.bot = y + 175;
    audio.left = x;
    audio.center = y;
    ggprint8b(&audio, 16, yellow, "Audio On");

    //For off mode
    audio.bot = y + 158;
    audio.left = x;
    audio.center = y;
    ggprint8b(&audio, 16, yellow, "Audio Off");

}

void onArrow (const int xres, const int yres)
{
    int y = yres/2;
    int x = xres/2;

    Rect on_arrow;
    on_arrow.bot = y + 175;
    on_arrow.left = x - 35;
    on_arrow.center = y;
    ggprint8b(&on_arrow, 16, yellow, ">>");
    
}

void offArrow (const int xres, const int yres)
{
    int y = yres/2;
    int x = xres/2;

    Rect off_arrow;
    off_arrow.bot = y + 158;
    off_arrow.left = x - 40;
    off_arrow.center = y;
    ggprint8b(&off_arrow, 16, yellow, ">>");
    
}

void gameOver (const int xres, const int yres)
{
    int y = yres/2;
    int x = xres/2;

    Rect over;
    over.bot = y + 175;
    over.left = x;
    over.center = y;
    ggprint8b(&over, 16, white, "GAME OVER!");

}
