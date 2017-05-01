//Created by: Christian Chavez
//Dodge project
//Individual coode
//Last Update: 4/30/2017

/*
                    *********Week 14 update*********
this code asks for the user's name and takes in string inputs from the screen.
These values are then used to protray the character information that appears on
the screen when the game is played. It asks for one player's name after the next
when two player mode is selected.
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
void introMessage(const int, const int);
void gameOverScores(const int, const int, char [], char []);
extern int p1_score;
extern int p2_score;
extern bool two_player;

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

void introMessage(const int xres, const int yres)
{
    unsigned int yellow = 0x00dddd00;
    Rect i;
    
    i.bot = yres/2 + 80;
    i.left = xres/2;
    i.center = yres/2;
    ggprint13(&i, 20, yellow, "Press 'm' for start menu");
}

void gameOverScores (const int xres, const int yres, char p1_name[], char p2_name[])
{
    unsigned int yellow = 0x00dddd00;
    Rect s;

    if (!two_player) {
        s.bot = yres/2 + 100;
        s.left = xres/2 - 40;
        s.center = yres/2;
        ggprint13(&s, 20, yellow, "%s's score: ", p1_name);

        s.bot = yres/2 + 100;
        s.left = xres/2 + 100;
        s.center = yres/2;
        ggprint13(&s, 20, yellow, "%i", p1_score);
    
        s.bot = yres/2 + 75;
        s.left = xres/2;
        s.center = yres/2;
        ggprint13(&s, 20, yellow, "Press c to continue");

    }

    if (two_player) {
        s.bot = yres/2 + 100;
        s.left = xres/2 - 40;
        s.center = yres/2;
        ggprint13(&s, 20, yellow, "%s's score: ", p1_name);

        s.bot = yres/2 + 100;
        s.left = xres/2 + 100;
        s.center = yres/2;
        ggprint13(&s, 20, yellow, "%i", p1_score);

        s.bot = yres/2 + 80;
        s.left = xres/2 - 40;
        s.center = yres/2;
        ggprint13(&s, 20, yellow, "%s's score: ", p2_name);

        s.bot = yres/2 + 80;
        s.left = xres/2 + 100;
        s.center = yres/2;
        ggprint13(&s, 20, yellow, "%i", p2_score);

        if (p1_score > p2_score) {
            s.bot = yres/2 + 60;
            s.left = xres/2;
            s.center = yres/2;
            ggprint13(&s, 20, yellow, "%s Wins!", p1_name);
        } else {
            s.bot = yres/2 + 60;
            s.left = xres/2;
            s.center = yres/2;
            ggprint13(&s, 20, yellow, "%s Wins!", p2_name);
        }
        
        s.bot = yres/2 + 40;
        s.left = xres/2;
        s.center = yres/2;
        ggprint13(&s, 20, yellow, "Press c to continue");

        
    }
}














