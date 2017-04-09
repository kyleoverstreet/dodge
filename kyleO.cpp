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

Implemented collision detection. Currently just displays collision count
to screen but will soon have functionality. Most of this was done in main
source file (dodge.cpp). I will try to transfer some work to my individual file.
*/

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "src/shared.h"

extern "C" {
#include "src/fonts.h"
}

using namespace std;

void deleteSpike(Spike *node);
void display_score(int, int);
void display_collisions(int, int, int, int, bool);
void gamelog(string, int);
void upload_scores();

// TO DO:
// Set up gamelog and upload_scores functions to communicate with a webpage
// Create score display for 2-Player mode (after it's implemented)

extern Spike *sphead;
int score = 0;

// Deletes off-screen items and keeps track of player score
void deleteSpike(Spike *node)
{
    //only
    if (node->prev == NULL && node->next == NULL){
        sphead = NULL;
    }
    //beginning
    else if (node->prev == NULL){
        sphead = node->next;
        node->next->prev = NULL;
    }
    //end
    else if (node->next == NULL){
        node->prev->next = NULL;
    }
    //node is somewhere else
    else{
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }

	// Free the node's memory and set node to NULL
    delete node;
	node = NULL;

    score++;
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

// Displays collision with player - for testing purposes
void display_collisions(int xres, int yres, int spike, int helm, bool status)
{
	Rect r;
	r.bot = yres - 70;
	r.left = xres - 198;
	r.center = 0;
	unsigned int color = 0x00dddd00;
	ggprint8b(&r, 16, color, "Collisions (for testing):");
	ggprint8b(&r, 16, color, "Spikes - %i", spike);
	ggprint8b(&r, 16, color, "Helmets - %i", helm);
	if (status == true) {
		ggprint8b(&r, 16, 0x00ff00, "Helmet ON");
	} else {
		ggprint8b(&r, 16, 0xff0000, "No helmet");
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
