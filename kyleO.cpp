// Kyle Overstreet
// CMPS 3350
// Dodge Project
// Individual source code

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void gamelog(string, int);
void upload_scores(const char *);

/*int main()
{
    const char *file_path = "/home/stu/koverstreet/public_html/3350/dodge.html";
   
    // game scores have been hard-coded into gamelog.txt
    // until gamelog function can be implemented with our game
    
    //gamelog(player1, score1);
    
    upload_scores(file_path);

    return 0;
}*/

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
// (Hard-coded file path for function testing purpose)
void upload_scores(const char *path)
{
    cout << "Ran Kyle's upload_scores function." << endl
        << "See cs.csubak.edu/~koverstreet/3350/dodge.html for sample" << endl;

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

    ofstream ofs(path);
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
