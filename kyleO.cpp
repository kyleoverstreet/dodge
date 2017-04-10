// Kyle Overstreet
// CMPS 3350
// Dodge Project
// Individual source code
// Last edit: 4/09/17

/*
====================WEEK 11====================
Made many changes to our main source file (src/dodge.cpp).
Cleaned up project's repo. Removed files, renamed files, etc.
Helped Young with his create items functions (dodge.cpp and youngO.cpp)

====================SPRING BREAK====================
Removed large sound and ppm files from project repo.
Fixed all compiling errors.
Removed unnecessary code from src/dodge.cpp.
Implemented collision detection between player and items.
Added star item.
*/

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include "src/shared.h"

extern "C" {
#include "src/fonts.h"
}

using namespace std;

void deleteSpike(Spike *node);
void deleteStar(Star *node);
void display_score(int, int);
void display_collisions(int, int, int, int, int);
void display_player_status(int, int, bool, bool);
void gamelog(string, int);
void upload_scores();

extern Spike *sphead;
extern Star *sthead;
int score = 0;

// Deletes off-screen items and keeps track of player score
void deleteSpike(Spike *node)
{
    if (node->prev == NULL && node->next == NULL) {
        sphead = NULL;
    }
    else if (node->prev == NULL) {
        sphead = node->next;
        node->next->prev = NULL;
    }
    else if (node->next == NULL) {
        node->prev->next = NULL;
    }
    else {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }

	// Free the node's memory and set node to NULL
    delete node;
	node = NULL;

    score++;
}

void deleteStar(Star *node)
{
    if (node->prev == NULL && node->next == NULL) {
        sthead = NULL;
    }
    else if (node->prev == NULL) {
        sthead = node->next;
        node->next->prev = NULL;
    }
    else if (node->next == NULL) {
        node->prev->next = NULL;
    }
    else {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }

	// Free the node's memory and set node to NULL
    delete node;
	node = NULL;
}

// Displays the player score at top-right of screen
void display_score(int xres, int yres)
{
	Rect r;
	r.bot = yres - 30;
	r.left = xres - 100;
	r.center = 0;
	unsigned int color = 0x00dddd00;
	ggprint8b(&r, 16, color, "Score: %i", score);
}

// Displays collisions with player (for testing purposes)
void display_collisions(int xres, int yres, int spike, int helm, int star)
{
	Rect r;
	r.bot = yres - 70;
	r.left = xres - 198;
	r.center = 0;
	unsigned int white = 0xffffff;
	ggprint8b(&r, 16, white, "Collisions (for testing):");
	ggprint8b(&r, 16, white, "Spikes - %i", spike);
	ggprint8b(&r, 16, white, "Stars - %i", star);
	ggprint8b(&r, 16, white, "Helmets - %i", helm);
}

void display_player_status(int xres, int yres, bool helm_status, bool invincibility)
{
	Rect r;
	r.bot = yres - 160;
	r.left = xres - 198;
	r.center = 0;
	unsigned int red = 0xff0000;
	unsigned int green = 0x00ff00;
	unsigned int yellow = 0x00dddd00;
	if (helm_status == true) {
		ggprint8b(&r, 16, green, "Helmet ON");
	} else {
		ggprint8b(&r, 16, red, "No helmet");
	}
	if (invincibility == true) {
		ggprint8b(&r, 16, yellow, "Invincible");
	} else {
		ggprint8b(&r, 16, red, "Not invincible");
	}
}

// Append player name, score, and date to gamelog
void gamelog(string p1, int score1)
{
    // Setup for date output
    tm *my_time;
    time_t t = time(NULL);
    my_time = localtime(&t);

    ofstream f;
    f.open("gamelog.txt", ofstream::app);
    f << p1 << " "
        << score1 << " "
        << my_time->tm_mon+1 << "/"
        << my_time->tm_mday << "/"
        << my_time->tm_year+1900 << endl;
    f.close();
}

// Upload the gamelog to an html webpage
void upload_scores()
{
    cout << "Ran Kyle's upload_scores function." << endl
        << "See \"scores.html\". This file will eventually upload to web server" << endl
        << "Webpage will look like this: http://cs.csubak.edu/~koverstreet/3350/dodge/scores.html" << endl;

    vector<string> players;
    vector<int> scores;
    vector<string> dates;

    string p;
    int s;
    string d;

    // Store data from gamelog into vectors
    ifstream ifs("gamelog.txt");
    while (true) {
        ifs >> p >> s >> d;
        if (ifs.eof()) {
            break;
        }
        players.push_back(p);
        scores.push_back(s);
        dates.push_back(d);
    }
    ifs.close();

    ofstream ofs("scores.html");
    ofs << 
        "<html>\n"
            "<head>\n"
                "<script src=\"sorttable.js\" type=\"text/javascript\"></script>\n"
                "<title>Dodge Scores</title>\n"
                "<style>\n"
                "body {\n"
                "    background-color: #75b3ad;\n"
                "}\n"
                "table {\n"
                "    font-family: arial, sans-serif;\n"
                "    border-collapse: collapse;\n"
                "    width: 100%\n"
                "}\n"
                "td, th {\n"
                "    border: 1px solid #dddddd;\n"
                "    text-align: left;\n"
                "    padding: 8px\n"
                "}\n"
                "tr:nth-child(even) {\n"
                "    background-color: #dddddd;"
                "}\n"
                "</style>\n"
            "</head>\n"
        "<body>\n"
            "<h2>Dodge Scores</h2>\n"
            "<table>\n"
                "<table class=\"sortable\">\n"
                "<tr>\n"
                    "<th>Player</th>\n"
                    "<th>Score</th>\n"
                    "<th>Date</th>\n"
                "</tr>\n";
                // Setup table row for each line in gamelog
                for (int i = players.size()-1; i >= 0; i--) {
                    ofs << 
                    "<tr>\n"
                        "<td>" << players[i] << "</td>\n"
                        "<td>" << scores[i] << "</td>\n"
                        "<td>" << dates[i] << "</td>\n"
                    "</tr>\n";
                }
                ofs <<
            "</table>\n"
        "</body>\n"
        "</html>";
    ofs.close();
}
