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
#include </usr/include/AL/alut.h>
#include <stdlib.h>
#include <vector>
#include "src/shared.h"
#include <GL/gl.h>
#include <time.h>

extern "C" {
	#include "src/fonts.h"
}

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
#endif

extern void spike_bounce(Spike* spike) {
	srand(time(NULL));
	spike->pos[1] += 3;
	spike->vel[1] = 11;
	spike->vel[0] = rand() % 21 + (-10);
}

/* end credits scene

extern void end_credits(int xres, int yres) {
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

	Rect r;
	r.bot = 400 - 30;
	r.left = 300 - 100;
	r.center = 0;
	unsigned int color = 0x00dddd00;
	ggprint13(&r, 16, color, "This is a test");
}
*/
