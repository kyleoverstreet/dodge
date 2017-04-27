// Author: Kory Despot 
// sound functions
// One function to bounce spike when the player is hit with the helmet
// Working on end credits screen
//
//
//
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif

#include <stdlib.h>
#include <vector>
#include "src/shared.h"
#include <GL/gl.h>
#include <time.h>

unsigned int black = 0xffffff;
const float gravity = -0.1f;

extern "C" {
	#include "src/fonts.h"
}

timespec helm_start, helm_current;
timespec power_start, power_current;

#ifdef USE_OPENAL_SOUND
ALuint alSource;
ALuint alBuffer;

extern void initialize_sounds() 
{
	alutInit(0, NULL);
	if ( alGetError() != AL_NO_ERROR ) {
		printf("error initializing sound\n");
		return;
	}
	alGetError();

	float vec[6] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);

}
extern void cleanup_sounds()
{
	ALCcontext *Context = alcGetCurrentContext();
	ALCdevice *Device = alcGetContextsDevice(Context);

	alcMakeContextCurrent(NULL);

	alcDestroyContext(Context);

	alcCloseDevice(Device);

}
/*extern void check_sound() 
{
	ALint state;
	alGetSourcei(alSource, AL_SOURCE_STATE, &state);
	if(state != AL_PLAYING) {
	printf("deleting buffer and source\n");
	alDeleteSources(1, &alSource);
	alDeleteBuffers(1, &alBuffer);
	}
}
*/
extern void play_helmet_hit()
{
	//ALuint alBuffer;
	alBuffer = alutCreateBufferFromFile("./sounds/metal-clang1.wav");

	//ALuint alSource;
	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER, alBuffer);
	
	alSourcef(alSource, AL_GAIN, 1.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR setting sound source\n");
		return;
	}
	
	alSourcePlay(alSource);
}



extern void play_powerup() 
{
	//ALuint alBuffer;
	alBuffer = alutCreateBufferFromFile("./sounds/powerup01.wav");

	//ALuint alSource;
	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER, alBuffer);
	
	alSourcef(alSource, AL_GAIN, 1.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR setting sound source\n");
		return;
	}
	
	alSourcePlay(alSource);
}

extern void play_GetShield() 
{
	if ( alGetError() != AL_NO_ERROR ) {
		printf("error initializing sound\n");
		return;
	}
	alGetError();


	float vec[6] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);

	//ALuint alBuffer;
	alBuffer = alutCreateBufferFromFile("./sounds/pickup_shield.wav");

	//ALuint alSource;
	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER, alBuffer);
	
	alSourcef(alSource, AL_GAIN, 1.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR setting sound source\n");
		return;
	}
	
	alSourcePlay(alSource);
}

extern void play_theme()
{
	alBuffer = alutCreateBufferFromFile("./sounds/theme.wav");

	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER, alBuffer);
	
	alSourcef(alSource, AL_GAIN, 1.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_TRUE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR setting sound source\n");
		return;
	}
	
	alSourcePlay(alSource);
}
extern void play_game_over()
{
	alBuffer = alutCreateBufferFromFile("./sounds/game_over.wav");

	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER, alBuffer);
	
	alSourcef(alSource, AL_GAIN, 1.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR setting sound source\n");
		return;
	}
	
	alSourcePlay(alSource);
}
extern void play_health_pickup()
{
	alBuffer = alutCreateBufferFromFile("./sounds/health-pickup.wav");

	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER, alBuffer);
	
	alSourcef(alSource, AL_GAIN, 1.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR setting sound source\n");
		return;
	}
	
	alSourcePlay(alSource);
}
#endif

extern void spike_bounce(Spike* spike) {
	srand(time(NULL));
	spike->pos[1] += 3;
	spike->vel[1] = 11;
	spike->vel[0] = rand() % 21 + (-10);
}

extern bool start_helm_timer() {
	clock_gettime(CLOCK_REALTIME, &helm_start);
	return true;
}
extern bool check_helm_timer(bool helm) {
	if(helm == false) 
		return false;
	clock_gettime(CLOCK_REALTIME, &helm_current);
	int timediff = helm_current.tv_sec - helm_start.tv_sec;
	if( timediff < 5) 
		return true;
	else 
		return false;
}
extern bool start_powerup_timer() {
	clock_gettime(CLOCK_REALTIME, &power_start);
	return true;
}
extern bool check_powerup_timer(bool powerup) {
	if(powerup == false) 
		return false;
	clock_gettime(CLOCK_REALTIME, &power_current);
	int timediff = power_current.tv_sec - power_start.tv_sec;
	if( timediff < 5) 
		return true;
	else 
		return false;	
}



// end credits scene

extern void end_credits(int xres, int yres) {
	/*
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	float wid = 40.0f;
        glColor3f(1.0, 1.0, 1.0);

	glBindTexture(GL_TEXTURE_2D, bdTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
        glEnd();
	*/

	
	Rect r;
	for(int i=0; i < 8000; i++) {
	r.bot = yres/2;
	r.left = xres/2 + 20;
	r.center = 0;
	
	ggprint13(&r, 16, black, "This is a test");
	}
}

