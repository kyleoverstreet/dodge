#ifndef SHARED_H
#define SHARED_H


#define MakeVector(x, y, z, v) (v)[0]=(x), (v)[1]=(y), (v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]

typedef double Flt;
typedef double Vec[3];
typedef struct t_player {
    Vec pos;
    Vec vel;
    Vec lastpos;
    bool LR = true;
} Player;

#endif
