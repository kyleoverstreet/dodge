// Author: Kory Despot 
// sound functions for dodge program
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
	usleep(50000);

	//alDeleteSources(1, &alSource);

	//alDeleteBuffers(1, &alBuffer);
	
	//ALCcontext *Context = alcGetCurrentContext();
	//ALCdevice *Device = alcGetContextsDevice(Context);

	//alcMakeContextCurrent(NULL);

	//alcDestroyContext(Context);

	//alcCloseDevice(Device);
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

	//alDeleteSources(1, &alSource);

	//alDeleteBuffers(1, &alBuffer);
	
	//ALCcontext *Context = alcGetCurrentContext();
	//ALCdevice *Device = alcGetContextsDevice(Context);

	//alcMakeContextCurrent(NULL);

	//alcDestroyContext(Context);

	//alcCloseDevice(Device);
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

	//alDeleteSources(1, &alSource);

	//alDeleteBuffers(1, &alBuffer);
	
	//ALCcontext *Context = alcGetCurrentContext();
	//ALCdevice *Device = alcGetContextsDevice(Context);

	//alcMakeContextCurrent(NULL);

	//alcDestroyContext(Context);

	//alcCloseDevice(Device);
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
