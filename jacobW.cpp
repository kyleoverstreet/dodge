#include <iostream>
#include <cstring>
using namespace std;

/*void cleanupPPM(void) {
    system("rm ./images/standL.ppm");
    system("rm ./images/PixelBG.ppm");
}

typedef struct t_player {
    Vec pos;
    Ved vel;
    Vec lastpos;
} Player;
Player player;

void movePlayer(int xres) {

    //Check if edge left
    if (player.pos[0] <= 0) {
	player.pos[0] = 0;
    }

    //Check if edge right
    if (player.pos[0] >= xres) {
	player.pos[0] = xres;
    }
}

void checkKeys(XEvent *e) {
    static int shift=0;
    int key = XLookupKeysym(&e->xkey, 0);
    if (e->type == KeyRelease) {
	if (key == XK_Shift_L || key == XK_Shift_R)
	    shift = 0;
	return;
    }
    if (e->type == KeyPress) {
	if (key == XK_Shift_L || key == XK_Shift_R) {
	    shift = 1;
	    return;
	}
    } else {
	return;
    }
    switch(key) {
	case XK_b:
	    //toggle character
	    break;
	case XK_Left:
	    //VecCopy(bigfoot.pos, bigfoot.lastpos);
	    player.pos[0] -= 8.0;
	case XK_Right:
	    //VecCopy(bigfoot.pos, bigfoot.lastpos);
	    player.pos[0] += 8.0;

void physics(void) {
    
    //This is defined above for boundaries
    movePlayer();
    
    //This will be for checking the items falling
    //checkItems();
}
*/
void jacob () {
    cout << "This is Jacob's test function" << endl;
    return;
}
