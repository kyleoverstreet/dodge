#ifndef SHARED_H
#define SHARED_H

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%a)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                      (c)[1]=(a)[1]-(b)[1]; \
                      (c)[2]=(a)[2]-(b)[2]
#define MakeVector(x, y, z, v) (v)[0]=(x), (v)[1]=(y), (v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]


typedef struct t_player {
    Vec pos;
    Vec vel;
    Vec lastpos;
    bool LR = true;
} Player;

typedef struct t_raindrop {
	int type;
	int linewidth;
	int sound;
	Vec pos;
	Vec lastpos;
	Vec vel;
	Vec maxvel;
	Vec force;
	float length;
	struct t_raindrop *prev;
	struct t_raindrop *next;
} Raindrop;


typedef struct t_umbrella {
	int shape;
	Vec pos;
	Vec lastpos;
	float width;
	float width2;
	float radius;
} Umbrella;

#endif
