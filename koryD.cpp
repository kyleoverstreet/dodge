

#include <iostream>
#include <AL/al.h>
#include <string.h>
#include <AL/alc.h>


#ifdef LIBAUDIO
#include <audio/wave.h>
#define BACKEND "libaudio"
#else
#include <AL/alut.h>
#define BACKEND "alut"
#endif


using namespace std;


void kory() 
{
	cout << "This is a test of kory's function\n";
	return;
}


static void list_audio_devices(const ALCchar *devices)
{
	const ALCchar *device = devices, *next = devices + 1;
	size_t len = 0;
	
	while (device && *device != '\0' && next && *next != '\0') 
	{
		fprintf(stdout, "%s\n", device);
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
	}
}



