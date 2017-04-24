//Created by: Christian Chavez
//Dodge project
//Individual coode
//Last Update: 4/23/2017

/*
                    *********Week 13 update*********
for previous weeks I tried to implement a menu, but that was having issues
when trying to implement it to the game. So, the previous method was scratched
and a new one was started in order to get it wrking with the game. The Menu is
designed to apper in the window when the game is started up and give the user
a place for navigation, whether thats how to play the game, look at the high
scores or pick what game mode they would like to do. On the 23rd I added the
ability to bring up a menu and added limited functionality to navigate throuh
the menu.
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
void credits(const int, const int);

//Colors for the test in themenus and the sub menus
unsigned int red = 0xff0000;
//unsigned int green = 0x00ff00;
unsigned int yellow = 0xffff00;
unsigned int blue = 0x0000ff;
//unsigned int aqua = 0x00ffff;
//unsigned int white = 0xffffff;

void menu(const int xres, const int yres)
{
    float w, h;
    glClear(GL_COLOR_BUFFER_BIT);
    
    Shape s;
    s.width = 300;
    s.height = 225;
    s.center.x = xres/2;
    s.center.y = yres/2 + 20;
    glColor3ub(0,0,0);
    glPushMatrix();
    glTranslatef(s.center.x, s.center.y, s.center.z);
    w = s.width;
    h = s.height;
    glBegin(GL_QUADS);
        glVertex2i(-w, -h);
        glVertex2i(-w, h);
        glVertex2i(w, h);
        glVertex2i(w, -h);
        glEnd();
        glPopMatrix();
    //Welcome message
    Rect welcome;
    welcome.bot = s.center.y + 90;
    welcome.left = s.center.x;
    welcome.center = s.center.y;
    ggprint8b(&welcome, 16, blue, "WELCOME TO THE DODGE MENU!!!");
    
    Rect keys;
    keys.bot = s.center.y + 73;
    keys.left = s.center.x;
    keys.center = s.center.y;
    ggprint8b(&keys, 16, red, "Use the keyboard to navigate through the menu!");
    
    Rect resume;
    resume.bot = s.center.y;
    resume.left = s.center.x - 91;
    resume.center = s.center.y;
    ggprint8b(&resume, 16, yellow, "<m> Exit Menu");

    Rect credits;
    credits.bot = s.center.y - 17;
    credits.left = s.center.x - 100;
    credits.center = s.center.y;
    ggprint8b(&credits, 16, yellow, "<c> Credits");



}


void credits(const int xres, const int yres)
{
    float w, h;
    glClear(GL_COLOR_BUFFER_BIT);
    
    Shape s;
    s.width = 300;
    s.height = 225;
    s.center.x = xres/2;
    s.center.y = yres/2 + 20;
    glColor3ub(0,0,0);
    glPushMatrix();
    glTranslatef(s.center.x, s.center.y, s.center.z);
    w = s.width;
    h = s.height;
    glBegin(GL_QUADS);
        glVertex2i(-w, -h);
        glVertex2i(-w, h);
        glVertex2i(w, h);
        glVertex2i(w, -h);
        glEnd();
        glPopMatrix();
    //Welcome message
    Rect christian;
    christian.bot = s.center.y + 90;
    christian.left = s.center.x;
    christian.center = s.center.y;
    ggprint8b(&christian, 16, blue, "MENU DEVELOPER - Christian Chavez");
   
}


