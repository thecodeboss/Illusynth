/************************************************************************/
/* Illusynth Example: Complex Procedural                                */
/*                                                                      */
/* This example demonstrates a more advanced procedural sound generator */
/************************************************************************/
#include <Illusynth.h>

int main()
{
	// Get the audio device for making sound
	AudioDevice* Audio = AudioDevice::GetInstance();
	if (Audio == nullptr) return -1;

	// Initialize the audio device, quitting execution on failure
	if (!Audio->Init()) return -1;

	return 0;
}
