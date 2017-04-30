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
#include <cstring>
#include <time.h>
#include <vector>
#include "src/ppm.h"
#include "src/christianC.h"
using namespace std;

extern "C" {
#include "src/fonts.h"
}

struct Vec {
    float x, y, z;
};

struct Shape {
    float width, height;
    Vec center;
};

void player1Name(const int, const int, Input);
void getName_player1(int, Input);
void assign_namep1(char[], Input);
extern bool display_playername;
void player2Name(const int, const int, Input);
void getName_player2(int, Input);
void assign_namep2(char[], Input);
extern bool display_playername2;
extern bool display_modemenu;
bool entering_one = true;
bool entering_two = false;


unsigned int black_ = 0x000000;

void player1Name (const int xres, const int yres, Input &input)
{
    float w, h;

    Shape s;

    s.width = 100;
    s.height = 15;
    s.center.x = xres/2;
    s.center.y = yres/2 + 75;
    glColor3ub(244, 241, 29);
    glPushMatrix();
    glTranslatef(s.center.x, s.center.y, s.center.z);
    w = s.width;
    h = s.height;
    glBegin(GL_QUADS);
    glVertex2i(-w,-h);
    glVertex2i(-w, h);
    glVertex2i( w, h);
    glVertex2i( w,-h);
    glEnd();
    glPopMatrix();
    
    Rect p;
    
    p.bot = s.center.y + 30;
    p.left = s.center.x;
    p.center = s.center.y;
    ggprint13(&p, 20, black_, "Enter player1 name");

    p.bot = s.center.y - 8;
    p.left = s.center.x;
    p.center = s.center.y;
    ggprint13(&p, 20, black_, "%s", input.player1);

}

void getName_player1 (int key, Input &input)
{
    if (entering_one) {
        if (key >= XK_a && key <= XK_z) {
            char k[2];
            k[0] = key;
            k[1] = '\0';
            strcat(input.player1, k);
            return;
        }
        if (key == XK_BackSpace) {
            int slen = strlen(input.player1);
            if (slen > 0)
                input.player1[slen - 1] = '\0';
            return;
        }
        if (key == XK_Left) {
            display_playername = false;
            display_playername2 = false;
            display_modemenu = true;
            return;
        }
    }
}

void assign_namep1 (char p1_name[], Input &input)
{
    for (int i = 0; i < 100; i++)
        p1_name[i] = input.player1[i];
}

void player2Name (const int xres, const int yres, Input &input)
{
    float w, h;

    Shape s;

    s.width = 100;
    s.height = 15;
    s.center.x = xres/2;
    s.center.y = yres/2 + 75;
    glColor3ub(244, 241, 29);
    glPushMatrix();
    glTranslatef(s.center.x, s.center.y, s.center.z);
    w = s.width;
    h = s.height;
    glBegin(GL_QUADS);
    glVertex2i(-w,-h);
    glVertex2i(-w, h);
    glVertex2i( w, h);
    glVertex2i( w,-h);
    glEnd();
    glPopMatrix();
    
    Rect p;
    
    p.bot = s.center.y + 30;
    p.left = s.center.x;
    p.center = s.center.y;
    ggprint13(&p, 20, black_, "Enter player2 name");

    p.bot = s.center.y - 8;
    p.left = s.center.x;
    p.center = s.center.y;
    ggprint13(&p, 20, black_, "%s", input.player2);

}

void getName_player2 (int key, Input &input)
{
    if (entering_two) {
        if (key >= XK_a && key <= XK_z) {
            char k[2];
            k[0] = key;
            k[1] = '\0';
            strcat(input.player2, k);
            return;
        }
        if (key == XK_BackSpace) {
            int slen = strlen(input.player2);
            if (slen > 0)
                input.player2[slen - 1] = '\0';
            return;
        }
        if (key == XK_Left) {
            display_playername = false;
            display_playername2 = false;
            display_modemenu = true;
            return;
        }
    }
}

void assign_namep2 (char p2_name[], Input &input)
{
    for (int i = 0; i < 100; i++)
        p2_name[i] = input.player2[i];
}



